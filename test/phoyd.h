#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <stdexcept>

#include <emmintrin.h>

namespace xlang::impl::code_converter
{
//
// A simple, flexible, self contained and stateless UTF8<->UTF16 converter.
//
// Note:
// CCG=C++ Core Guidelines (https://github.com/isocpp/CppCoreGuidelines)
// GSL=CCG support library (https://github.com/Microsoft/GSL/)

#ifndef GSL_LIKELY
#if defined(__clang__) || defined(__GNUC__)
#define GSL_LIKELY(x) __builtin_expect(!!(x), 1)
#define GSL_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define GSL_LIKELY(x) (!!(x))
#define GSL_UNLIKELY(x) (!!(x))
#endif
#endif

// copied from gsl/gsl_assert:
//
// GSL_ASSUME(cond)
//
// Tell the optimizer that the predicate cond must hold. It is unspecified
// whether or not cond is actually evaluated.
//
#define GSL_ASSUME(X)
#ifndef GSL_ASSUME
#ifdef _MSC_VER
#define GSL_ASSUME(cond) __assume(cond)
#elif defined(__clang__) || defined(__GNUC__)
#define GSL_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#else
#define GSL_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
#endif
#endif
//
// GSL_ASSUME() is not a compiler hint. Instead it kills all code paths where the assumption would
// not hold, making the code undefined if the assumption were wrong. We turn ASSUME in to ASSERT for
// Debug build to check our assumptions at runtime.
//
#ifdef _DEBUG
#define XLANG_ASSUME(cond) assert(cond)
#else
#define XLANG_ASSUME(cond) GSL_ASSUME(cond)
#endif

#ifdef _MSC_VER
#define XLANG_FORCE_INLINE __forceinline
#elif defined(__clang__) || defined(__GNUC__)
#define XLANG_FORCE_INLINE __attribute__((always_inline))
#else
XLANG_FORCE_INLINE
#endif

//
// error codes returned by user visible conversion functions.
//
enum class converter_result
{
    OK,
    INVALID_INPUT_DATA,
    OUTPUT_TOO_SMALL
};

//
// Implementation details
//
namespace detail
{
//
// throw converter_result as exception (internal use)
//
[[noreturn]] inline void invalid() { throw converter_result::INVALID_INPUT_DATA; }
[[noreturn]] inline void buffer_error() { throw converter_result::OUTPUT_TOO_SMALL; }

//
// bounded<> is a single, checked, forward only "iterator" that is an alias
// to 'start' and checks against 'end'. Since std::iterator is deprecated, this
// is just enough iterator here to be useful here in the decoder and encoder routines.
//
template<class T, void Err()>
struct bounded
{
    T &start;
    T end;
    typedef typename std::iterator_traits<T>::value_type value_type;
    typedef typename std::iterator_traits<T>::reference reference;
    bounded(T &start, const T& end): start(start),end(end) {}
    reference operator *() { return *start; }
    T operator ++ (int)
    {
        if (GSL_UNLIKELY(start==end)) Err();
        auto r=start;
        start++;
        return r;
    }
};
//
// dev_num is an output iterator which does nothing at all. useful for measuring
// output sizes.
//
template<class T>
struct dev_null
{
    T dontcare;
    const T& operator *() const { return *dontcare; }
    T& operator *() { return dontcare; }
    dev_null& operator ++ (int)
    {
        return *this;
    }
};

//
// [CCG]: Make narrowing explicit
// must be correct at call size
//
template <class R, class A>
constexpr R narrow_cast(const A& a)
{
    return static_cast<R>(a);
}

//
// bit helper
//
// create a mask of Count bits.
template<unsigned Count>
auto constexpr mask()
{
 return ((1 << Count) - 1);
}
//
// deposit 'Count' bits into return value
//
template<unsigned Start, unsigned Count, class T>
auto constexpr deposit(T in)
{
    constexpr T m = mask<Count>();
    return (in & m)<<Start;
}
//
// extract 'Count' bits
//
template<unsigned Start, unsigned Count, class T>
auto constexpr extract(T in)
{
    constexpr auto m = mask<Count>();
    return (in>>Start)&m;
}

//
// Codepoints in the surrogate area or after U++10FFFF are invalid.
//
// constexpr bool is_invalid_cp(char32_t u)
//{
//    return ((u >= 0xd800) && (u <= 0xdfff)) || (u > 0x10ffff);
//}
constexpr bool is_valid_cp(char32_t u)
{
    return (u <= 0xd7ff) || ((u > 0xdfff) && (u <= 0x10ffff));
}
//
// They can only be used in the UTF16 encoding, to mark a "surrogate pair",
// an encoding to map code points above 0x10000 into UTF16.
//
constexpr bool is_high_surrogate(char32_t u) { return ((u >= 0xd800) && (u <= 0xdbff)); }
constexpr bool is_low_surrogate(char32_t u) { return ((u >= 0xdc00) && (u <= 0xdfff)); }
constexpr bool is_surrogate(char32_t u) { return ((u >= 0xd800) && (u <= 0xdfff)); }

//
// return codepoint, but only if it is valid, otherwise assume malformed data
// and rise exception
//
constexpr char32_t if_valid(char32_t u)
{
    if (is_valid_cp(u))
        return u;
    else
        invalid();
}

//
// The uft32_filter simply copies inputs and outputs after checking,
// because we are using UTF32 as the intermediate format.
//
class utf32_filter
{
public:
    // The code value type this format is using.
    typedef char32_t cvt;
    // Maximum number of code values per code point. This can be used
    // for optimizations (like omitting buffer checks)
    static constexpr size_t max_cv_len = 1;

    // Decode as single code point from input 'in'. 'b' is a lookahead,
    // so we don't need to read anything.
    template <class In>
    static char32_t XLANG_FORCE_INLINE decode(char32_t b, In&&)
    {
        return if_valid(b);
    }
    // Convert 'c' to the output format and write it to 'out'
    // Returns the number of cove values that have been written
    template <class Out>
    static int XLANG_FORCE_INLINE encode(char32_t c, Out&& out)
    {
        return encode_valid(if_valid(c), out);
    }
    // Same as above, except that it does not check if 'c' is a
    // valid code point (for example, because it has been checked before)
    template <class Out>
    static int XLANG_FORCE_INLINE encode_valid(char32_t c, Out&& out)
    {
        XLANG_ASSUME(is_valid_cp(c));
        *out++=c;
        return 1;
    }
};

//
// The utf16_filter encodes and decodes UTF16 code values in
// native endian.
//
class utf16_filter
{
private:
public:
    typedef char16_t cvt;                   // code value type
    static constexpr size_t max_cv_len = 2; // a surrgate pair is 2 values long

    // read up to 'max_cv_len' UTF-16 code values from 'h' and 'in' and try to make
    // a valid codepoint from it.
    template <class In>
    static char32_t XLANG_FORCE_INLINE decode(char16_t h, In&& in)
    {
        if (is_high_surrogate(h))
        {
            char16_t l = *in++;
            if (!is_low_surrogate(l)) { invalid(); }
            char32_t cp = ((h - 0xd800u) << 10) + (l - 0xdc00u) + 0x10000u;
            return if_valid(cp);
        }
        return if_valid(h);
    }

    // write up to two UTF-16 code values to 'out'. The output byte order
    // is the native byte order.
    template <class Out>
    static int XLANG_FORCE_INLINE encode(char32_t c, Out&& out)
    {
        return encode_valid(if_valid(c), out);
    }

    template <class Out>
    static int XLANG_FORCE_INLINE encode_valid_ext(char32_t c, Out&& out)
    {
        c -= 0x10000;
        XLANG_ASSUME(c <= 0xfffff); // because is_valid_cp(c);

        // 0xfffff>>10 == 0x3ff, 0xd800+0x3ff == 0xdbff
        // therefore h is a valid high surrogate.
        char16_t h = narrow_cast<char16_t>(0xd800 + (c >> 10));
        *out++=h;
        // 0xdc00 + 0x3ff == 0xdfff
        // therefore l is a valid low surrogate
        char16_t l = 0xdc00 + (c & 0x3ff);
        *out++=l;
        return 2;
    }
    template <class Out>
    static int XLANG_FORCE_INLINE encode_valid(char32_t c, Out&& out)
    {
        XLANG_ASSUME(is_valid_cp(c));
        if (c < 0x10000)
        {
            *out++=c;
            return 1;
        }
        else
        {
            return encode_valid_ext(c, std::forward<Out>(out));
        }
    }
};

//
// utf8 filter
//
class utf8_filter
{
    using char8_t = uint8_t;
public:
    typedef char8_t cvt;
    static constexpr size_t max_cv_len = 4;

private:
    // Helper function. Read 'Count' bits starting from 'Start' in cp
    // and put 'Mark' over the octet result.
    template <char8_t Mark, unsigned Start, unsigned Count>
    static constexpr char8_t fetch(const char32_t cp)
    {
        static_assert(Count < 8, "invalid bitcount");
        static_assert(Count + Start < 32, "invalid bitstart");
        return narrow_cast<char8_t>(Mark | extract<Start,Count>(cp));
    }
    // This writes 'Count' bits from 'b' to 'cp' starting at 'Start'
    // Returns a check value that is 0 if 'b' without the 'Count' bits
    // is 'Mark' or some other value if not.
    // return false or non-zero for failure (Mark not found in b)
    template <unsigned Mark, unsigned Start, unsigned Count>
    static auto constexpr store_ck(char32_t& cp, char8_t b)
    {
        static_assert(Count < 8, "invalid bitcount");
        static_assert(Count + Start < 32, "invalid bitstart");
        auto v=static_cast<char32_t>(deposit<Start,Count>(b));
        cp = (cp | v);
        constexpr auto m=mask<Count>();
        return (b & ~m ) ^ Mark;
    }
    template <unsigned Mark, unsigned Start, unsigned Count>
    static auto constexpr store_nck(char32_t& cp, char8_t b)
    {
        static_assert(Count < 8, "invalid bitcount");
        static_assert(Count + Start < 32, "invalid bitstart");
        auto v=static_cast<char32_t>(deposit<Start,Count>(b));
        cp = (cp | v);
        return 0u;
    }
public:
    // Read up to 4 input values as UTF-8 and produce a UTF-32 code point
    // in native byte order.
    // (http://doc.cat-v.org/bell_labs/utf-8_history)
    //
    //    Bits  Hex Min  Hex Max  Byte Sequence in Binary
    // 1    7  00000000 0000007f 0vvvvvvv
    // 2   11  00000080 000007FF 110vvvvv 10vvvvvv
    // 3   16  00000800 0000FFFF 1110vvvv 10vvvvvv 10vvvvvv
    // 4   21  00010000 001FFFFF 11110vvv 10vvvvvv 10vvvvvv 10vvvvvv
    // 5   26  00200000 03FFFFFF 111110vv 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv
    // 6   31  04000000 7FFFFFFF 1111110v 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv
    //
    // UTF-8 encoding for each Hex Max:
    // 1    0x7f
    // 2    0xdf 0xbf
    // 3    0xef 0xbf 0xbf
    // 4    0xf4 0x8f 0xbf 0xbf
    // these are out switch values below
#define BRANCHLESS 1
    template <class In>
    static char32_t XLANG_FORCE_INLINE decode(char8_t b, In&& in)
    {
        // ATTENTION:
        // * no-returns are falling through 'invalid' at the end.
        if (b <= 0x7f) // 0x00..0x7f (Hex Min and Hex Max in the table above)
        {
            GSL_ASSUME(b<=0x7f);
            return b; // always valid
        }
        else if (b <= 0xdf) // 0x80..0x7ff
        {
            char32_t cp = 0;
            char8_t c = *in++;
#if BRANCHLESS
            auto fail = (store_ck<0xc0, 6, 5>(cp, b) | store_ck<0x80, 0, 6>(cp, c));
            GSL_ASSUME(cp <= mask<5+6>());
            if (!fail && (cp >= 0x80)) return cp;
#else
            auto fail = (store_ck<0xc0, 6, 5>(cp, b) || store_ck<0x80, 0, 6>(cp, c));
            if (!fail && (cp >= 0x80)) return cp;
#endif
        }
        else if (b <= 0xef) // 0x800..0xffff
        {
            char32_t cp = 0;
            char8_t c = *in++;
            char8_t d = *in++;
#if BRANCHLESS
            auto fail = (store_nck<0xe0, 12, 4>(cp, b) | store_ck<0x80, 6, 6>(cp, c) |
                         store_ck<0x80, 0, 6>(cp, d));
            GSL_ASSUME(cp <= mask<4+6+6>());
            if (!fail && (cp >= 0x800) && is_valid_cp(cp)) return cp;
#else
            auto fail = (store_ck<0xe0, 12, 4>(cp, b) || store_ck<0x80, 6, 6>(cp, c) ||
                         store_ck<0x80, 0, 6>(cp, d));
            if (!fail && (cp >= 0x800) && is_valid_cp(cp)) return cp;
#endif
        }        
        else if (b <= 0xf4) // 0x10000-0x10ffff
        {
            char32_t cp = 0;
            char8_t c = *in++;
            char8_t d = *in++;
            char8_t e = *in++;
#if BRANCHLESS
            auto fail = (store_nck<0xf0, 18, 3>(cp, b) | store_ck<0x80, 12, 6>(cp, c) |
                         store_ck<0x80, 6, 6>(cp, d) | store_ck<0x80, 0, 6>(cp, e));
            GSL_ASSUME(cp <= mask<3+6+6+6>()); // 0x1FFFFF

            if (!fail && (cp >= 0x10000) && (cp <= 0x10ffff)) return cp;
#else
            auto fail = (store_ck<0xf0, 18, 3>(cp, b) || store_ck<0x80, 12, 6>(cp, c) ||
                         store_ck<0x80, 6, 6>(cp, d) || store_ck<0x80, 0, 6>(cp, e));
            if (!fail && (cp >= 0x10000) && (cp <= 0x10ffff)) return cp;
#endif
        }
        invalid();
    }

    // Convert a UTF-32 codepoint into up to 4 UTF-8 code units and
    // write them to 'out'.
    template <class Out>
    static int XLANG_FORCE_INLINE encode(char32_t cp, Out&& out)
    {
        return encode_valid(if_valid(cp), out);
    }

    template <class Out>
    static int XLANG_FORCE_INLINE encode_valid(char32_t cp, Out&& out)
    {
        XLANG_ASSUME(is_valid_cp(cp));
        if (cp <= 0x7f)
        {
            *out++=(narrow_cast<char8_t>(cp));
            return 1;
        }
        else if (cp <= 0x7ff)
        {
            *out++=(fetch<0xc0, 6, 5>(cp));
            *out++=(fetch<0x80, 0, 6>(cp));
            return 2;
        }
        else if (cp <= 0xffff)
        {
            *out++=(fetch<0xe0, 12, 4>(cp));
            *out++=(fetch<0x80, 6, 6>(cp));
            *out++=(fetch<0x80, 0, 6>(cp));
            return 3;
        }
        else if (cp <= 0x10FFFF)
        {
            *out++=(fetch<0xf0, 18, 3>(cp));
            *out++=(fetch<0x80, 12, 6>(cp));
            *out++=(fetch<0x80, 6, 6>(cp));
            *out++=(fetch<0x80, 0, 6>(cp));
            return 4;
        }
        else
        {
            invalid();
        }
    }
};

//
// encodings with a common ASCII Plane might copy code values
// without conversion. Specialize for EBCDIC, UTF-16
//
template<class S,class D>
struct conv_pair
{
    static bool constexpr is_passthrough(typename S::cvt v)
    {
        return v <= 0x7f; // ASCII plane
    }
};
template<>
struct conv_pair<utf16_filter, utf32_filter>
{
    static bool constexpr is_passthrough(typename utf16_filter::cvt v)
    {
        return (v <= 0xd7ff);
    }
};
template<>
struct conv_pair<utf32_filter, utf16_filter>
{
    static bool constexpr is_passthrough(typename utf32_filter::cvt v)
    {
        return (v <= 0xd7ff);
    }
};

//
// We can use SrcFilter and DestFilter as they are or join them in
// a 'transformer' which can be specialized on its type parameters, for
// example to implement an optimized conversion pair.
//


template <class SrcFilter, class DestFilter>
struct transformer
{
    SrcFilter&& src;
    DestFilter&& dst;

    template<class R, class W>
    bool fastfwd(R &r, W &w)
    {
        if constexpr (std::is_same<SrcFilter,utf8_filter>::value)
        {
            auto v=(uint32_t*)&*r;
            if ((*v & 0x80808080)==0)
            {
                w[0]=r[0];
                w[1]=r[1];
                w[2]=r[2];
                w[3]=r[3];
                w+=4;
                r+=4;
                return true;
            }
        }
        return false;
    }

    template <class R, class W>
    size_t XLANG_FORCE_INLINE transform_safe(R& reader, W& writer)
    {
        if (fastfwd(reader,writer)) return 4;
        return transform_one(*reader++,reader,writer);
    }
    template <class R, class W>
    size_t XLANG_FORCE_INLINE transform_one(typename SrcFilter::cvt b, R& reader, W& writer)
    {
#ifdef __clang__
     // TODO: The whole passthrough thing shouldn't be neccesary, if the
     // compiler would look into decode() and encode(). For example,
     // utf8_filter.decode(b)  returns b immediately if b<=0x7f
     // while utf16_filter.writer simply writes every cp < 0x7ff directly
     // to the writer. So, the compiler should see this (because of inlining)
     // and write b directly to the output.
     // In fact, gcc-7 seems to do this and the code runs faster
     // if this block is disabled. clang OTOH is a much weaker inliner
     // and seems to forget things it should have learned from the inlined
     // code paths.
        if (conv_pair<SrcFilter,DestFilter>::is_passthrough(b))
        {
            *writer++=(b);
            return 1;
        }
#endif
        return dst.encode_valid(src.decode(b,reader), writer);
    }
    template <int N, class R, class W>
    size_t XLANG_FORCE_INLINE transform_multi(std::integral_constant<int,N>,R& reader, W& writer)
    {
        if constexpr(N>=4)
        {
            size_t s=0;
            if (std::is_same<SrcFilter,utf8_filter>::value && std::is_same<DestFilter,utf16_filter>::value )
            {
                auto *src=&*reader;
                auto *dst=&*writer;

                __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&*reader));
                if (!_mm_movemask_epi8(chunk))
                {
                    // unpack the first 8 bytes, padding with zeros
                    __m128i firstHalf = _mm_unpacklo_epi8(chunk, _mm_set1_epi8(0));
                    // and store to the destination
                    _mm_storeu_si128(reinterpret_cast<__m128i*>(dst), firstHalf);

                    // do the same with the last 8 bytes
                    __m128i secondHalf = _mm_unpackhi_epi8 (chunk, _mm_set1_epi8(0));
                    _mm_storeu_si128(reinterpret_cast<__m128i*>(dst+8), secondHalf);

                    // Advance
                    reader += 16;
                    writer += 16;
                    s+=16;
                    goto rest;
                 }
            }
            s+=transform_safe(reader,writer);
            s+=transform_safe(reader,writer);
            s+=transform_safe(reader,writer);
            s+=transform_safe(reader,writer);
            rest:
            return s+transform_multi(std::integral_constant<int,N-4>{},reader,writer);
        }
        else
        {
            size_t s=0;
            switch(N)
            {
            case 3: s+=transform_safe(reader,writer); // fallthrough
            case 2: s+=transform_safe(reader,writer); // fallthrough
            case 1: s+=transform_safe(reader,writer); // fallthrough
            case 0: return s;
            }
        }
    }

};

// 'convert' tries to convert the *complete* range from 'in_start' to
// 'in_end' and writes the result to the 'out_start' iterator, after
// processing the data with 'src_filter' and 'dst_filter'.
//
// 'src_filter' reads data in input format from its supplied reader
// argument and produces a data representation that is understood by
// dst_filter (for example an UTF-32 code unit).
//
// 'dst_filter' converts the internmediate representation
// to the output format and writes the result to its supplied writer.
//
// if 'count_only' is true, then no data is written to the output
// iterator. The number of output characters however is counted
// and returned.
//
// For the return value see 'converter_result'
//
// The function returns the number of characters written to the
// output iterator to the reference parameter "result_size"
//
// If a complete conversion requires more input than available, then
// the input is considered malformed and invalid() is raised.
//
// if the conversion needs more output buffer than available, then
// buffer_error() is raised.

template <class In, class Out, class SrcFilter, class DestFilter,
          class InCat = typename std::iterator_traits<In>::iterator_category,
          class OutCat = typename std::iterator_traits<Out>::iterator_category>
class converter_spec;

template <class In, class SrcFilter, class DestFilter,
          class InCat = typename std::iterator_traits<In>::iterator_category>
class output_size_counter_spec;

//
//          Specialized Converter
//
//
// This is the default template and the general case.
// In and Out can move forward, compare for *(in-)equality* only,
// Out is mutable
//
template <class In, class Out, class SrcFilter, class DestFilter, class InCat, class OutCat>
class converter_spec
{

public:
    static size_t convert(In in_start, In in_end, Out out_start, Out out_end,
                          SrcFilter&& src_filter, DestFilter&& dst_filter)
    {
        transformer<SrcFilter, DestFilter> trans{std::forward<SrcFilter>(src_filter),
                                                 std::forward<DestFilter>(dst_filter)};

        bounded<In,invalid> reader_checked{in_start,in_end};
        bounded<Out,buffer_error> writer_checked{out_start,out_end};

        size_t write_count = 0;

        while (in_start != in_end)
        { //
            write_count += trans.transform_one(*in_start++, reader_checked, writer_checked);
        }
        return write_count;
    }
};
//
// Specialization: In and Out are random access iterators.
//
template <class In, class Out, class SrcFilter, class DestFilter>
class converter_spec<In, Out, SrcFilter, DestFilter, std::random_access_iterator_tag,
                     std::random_access_iterator_tag>
{
public:
    static size_t convert(In in_start, In in_end, Out out_start, Out out_end,
                          SrcFilter&& src_filter, DestFilter&& dst_filter)
    {
        transformer<SrcFilter, DestFilter> trans{std::forward<SrcFilter>(src_filter),
                                                 std::forward<DestFilter>(dst_filter)};

        auto out_start_org = out_start; // (out-start-out_start_org) => number of values written

        // optimization: find a safe range for unchecked access to in and out.
        for(;;)
        {
            auto in_len = in_end - in_start;
            auto out_len = out_end - out_start;
            auto safelen =
                std::min(in_len / SrcFilter::max_cv_len, out_len / DestFilter::max_cv_len);
            constexpr int SZ=4;
            if (safelen<SZ) break;
            size_t i=0;
            // unroll
            for (;(i+SZ-1)<safelen;i+=SZ)
            {
                trans.transform_multi(std::integral_constant<int,SZ>{}, in_start,out_start);
            }
            for (;i<safelen;i++)
            {
                trans.transform_one(*in_start++, in_start,out_start);
            }
        }

        bounded<In,invalid> reader_checked{in_start,in_end};
        bounded<Out,buffer_error> writer_checked{out_start,out_end};

        while (in_start < in_end)
        { //
            trans.transform_one(*in_start++, reader_checked, writer_checked);
        }
        return out_start - out_start_org;

    }
};
//
// This is the general case for the
// output_size calculator.
// In can move forward, compare for *(in-)equality* only,
template <class In, class SrcFilter, class DestFilter, class InCat>
class output_size_counter_spec
{
public:
    // Default case, In and Out are forward only, mutable and compare for ineqality.
    static size_t output_size(In in_start, In in_end, SrcFilter&& src_filter,
                              DestFilter&& dst_filter)
    {
        bounded<In,invalid> reader_checked{in_start,in_end};

        size_t write_count = 0;
        while (in_start != in_end)
        {
            auto b = *in_start++;
            auto cp = src_filter.decode(b, reader_checked);
            dev_null<typename DestFilter::cvt> null;
            write_count += dst_filter.encode_valid(cp, null);
        }
        return write_count;
    }
};

} // namespace xlang::impl::code_converter::detail

//
// Make these part of the exported interf
//
using utf8_filter=detail::utf8_filter;
using utf16_filter=detail::utf16_filter;
using utf32_filter=detail::utf32_filter;

//
// The user visible conversion routine
//
template <class In, class Out, class SrcFilter, class DestFilter>
inline converter_result convert(In in_start, In in_end, Out out_start, Out out_end,
                                SrcFilter&& src_filter, DestFilter&& dst_filter,
                                size_t& result_size) noexcept
{
    try
    {
        // dispatch conversion to an specialized overload.
        auto sz = detail::converter_spec<In, Out, SrcFilter, DestFilter>::convert(
            in_start, in_end, out_start, out_end, std::forward<SrcFilter>(src_filter),
            std::forward<DestFilter>(dst_filter));
        result_size = sz;
        return converter_result::OK;
    } catch (converter_result r)
    {
        return r;
    }
}
// forward to a specialization on the iterator types.
template <class In, class SrcFilter, class DestFilter>
inline converter_result output_size(In in_start, In in_end, SrcFilter&& src_filter,
                                    DestFilter&& dst_filter, size_t& result_size) noexcept
{
    try
    {
        // dispatch conversion to an specialized overload.
        auto sz = detail::output_size_counter_spec<In, SrcFilter, DestFilter>::output_size(
            in_start, in_end, std::forward<SrcFilter>(src_filter),
            std::forward<DestFilter>(dst_filter));
        result_size = sz;
        return converter_result::OK;
    } catch (converter_result r)
    {
        return r;
    }
}
} // namespace xlang::impl::code_converter
