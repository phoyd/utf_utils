
# Benchmark results

created: Fri Nov  9 00:44:14 CET 2018 

command line: ./utf_utils_test -dd ../test_data -rx 31 -al phoyd,kewb-fast,kewb-fast-unrolled,kewb-sse

## Ryzen 1700


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 393 | 2117 | 1265 | 649 |
| chinese_wiki.txt | 1386 | 2559 | 1912 | 1355 |
| hindi_wiki.txt | 1860 | 2771 | 2182 | 1830 |
| japanese_wiki.txt | 1802 | 2454 | 1995 | 1618 |
| korean_wiki.txt | 1582 | 2888 | 2183 | 1678 |
| portuguese_wiki.txt | 668 | 2390 | 1525 | 893 |
| russian_wiki.txt | 1737 | 2835 | 2287 | 1706 |
| swedish_wiki.txt | 731 | 2404 | 1571 | 915 |
| stress_test_0.txt | 233 | 2021 | 1189 | 553 |
| stress_test_1.txt | 1885 | 1732 | 1587 | 1620 |
| stress_test_2.txt | 1451 | 1759 | 1409 | 3176 |
| hindi_wiki_in_english.txt | 427 | 2139 | 1315 | 671 |
| hindi_wiki_in_russian.txt | 1714 | 2885 | 2066 | 1726 |
| kermit.txt | 1010 | 2571 | 1696 | 1223 |
| z1_kosme.txt | 880 | 2528 | 1760 | 873 |
| z1_ascii.txt | 645 | 2760 | 1693 | 459 |
| **sum**| **18.404000**| **38.813000**| **27.635000**| **20.945000**|


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 297 | 1295 | 1221 | 663 |
| chinese_wiki.txt | 1454 | 2088 | 1949 | 1446 |
| hindi_wiki.txt | 1915 | 2531 | 2377 | 1945 |
| japanese_wiki.txt | 1896 | 2267 | 2251 | 1765 |
| korean_wiki.txt | 1639 | 2508 | 2272 | 1745 |
| portuguese_wiki.txt | 605 | 1575 | 1452 | 911 |
| russian_wiki.txt | 1737 | 2539 | 2355 | 1881 |
| swedish_wiki.txt | 694 | 1598 | 1418 | 938 |
| stress_test_0.txt | 174 | 1185 | 1091 | 566 |
| stress_test_1.txt | 2090 | 2053 | 2188 | 1841 |
| stress_test_2.txt | 1606 | 1969 | 1717 | 3384 |
| hindi_wiki_in_english.txt | 332 | 1404 | 1247 | 688 |
| hindi_wiki_in_russian.txt | 1740 | 2603 | 2290 | 1900 |
| kermit.txt | 1020 | 1999 | 1752 | 1310 |
| z1_kosme.txt | 1106 | 1815 | 1436 | 903 |
| z1_ascii.txt | 827 | 1641 | 1318 | 458 |
| **sum**| **19.132000s**| **31.070000s**| **28.334000s**| **22.344000s**|

## Opteron 3280


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 1029 | 3594 | 2372 | 1041 |
| chinese_wiki.txt | 2475 | 4123 | 3863 | 2357 |
| hindi_wiki.txt | 3181 | 4512 | 4615 | 3288 |
| japanese_wiki.txt | 3281 | 4148 | 4578 | 3163 |
| korean_wiki.txt | 2836 | 4685 | 4216 | 2921 |
| portuguese_wiki.txt | 1300 | 3799 | 2693 | 1229 |
| russian_wiki.txt | 3442 | 4547 | 4921 | 3017 |
| swedish_wiki.txt | 1371 | 3804 | 2742 | 1308 |
| stress_test_0.txt | 642 | 3469 | 2225 | 595 |
| stress_test_1.txt | 4239 | 3696 | 5441 | 3700 |
| stress_test_2.txt | 3855 | 3258 | 4381 | 7312 |
| hindi_wiki_in_english.txt | 998 | 3619 | 2392 | 949 |
| hindi_wiki_in_russian.txt | 3292 | 4474 | 4836 | 2935 |
| kermit.txt | 2189 | 4185 | 3659 | 2135 |
| z1_kosme.txt | 2118 | 4134 | 3213 | 1632 |
| z1_ascii.txt | 1892 | 3890 | 3019 | 809 |
| **sum**| **38.140000**| **63.937000**| **59.166000**| **38.391000**|


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 597 | 2291 | 2071 | 552 |
| chinese_wiki.txt | 2686 | 4515 | 3222 | 2445 |
| hindi_wiki.txt | 3407 | 5578 | 3827 | 3368 |
| japanese_wiki.txt | 3516 | 5789 | 3689 | 3186 |
| korean_wiki.txt | 3026 | 4858 | 3633 | 2980 |
| portuguese_wiki.txt | 1188 | 2789 | 2390 | 1001 |
| russian_wiki.txt | 3565 | 6081 | 3809 | 2989 |
| swedish_wiki.txt | 1313 | 2844 | 2406 | 1142 |
| stress_test_0.txt | 410 | 2200 | 1975 | 357 |
| stress_test_1.txt | 4478 | 7684 | 4008 | 3866 |
| stress_test_2.txt | 4018 | 6177 | 3483 | 7448 |
| hindi_wiki_in_english.txt | 709 | 2419 | 2136 | 661 |
| hindi_wiki_in_russian.txt | 3538 | 6175 | 3919 | 3028 |
| kermit.txt | 2198 | 4129 | 3204 | 2066 |
| z1_kosme.txt | 2138 | 4226 | 4278 | 2872 |
| z1_ascii.txt | 1794 | 2673 | 2917 | 712 |
| **sum**| **38.581000s**| **70.428000s**| **50.967000s**| **38.673000s**|

## Core i5-3320m


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 503 | 1452 | 1125 | 486 |
| chinese_wiki.txt | 1444 | 2368 | 2095 | 1337 |
| hindi_wiki.txt | 1964 | 2638 | 2312 | 1930 |
| japanese_wiki.txt | 1933 | 2462 | 2271 | 1818 |
| korean_wiki.txt | 1649 | 2648 | 2254 | 1600 |
| portuguese_wiki.txt | 663 | 1972 | 1575 | 628 |
| russian_wiki.txt | 1826 | 2719 | 2380 | 1777 |
| swedish_wiki.txt | 695 | 2082 | 1641 | 662 |
| stress_test_0.txt | 410 | 1326 | 1010 | 413 |
| stress_test_1.txt | 2261 | 2001 | 2142 | 2001 |
| stress_test_2.txt | 1986 | 1843 | 1973 | 3991 |
| hindi_wiki_in_english.txt | 495 | 1569 | 1136 | 496 |
| hindi_wiki_in_russian.txt | 1775 | 2768 | 2213 | 1774 |
| kermit.txt | 893 | 2148 | 1555 | 1039 |
| z1_kosme.txt | 1129 | 2477 | 2310 | 846 |
| z1_ascii.txt | 958 | 2802 | 2361 | 661 |
| **sum**| **20.584000**| **35.275000**| **30.353000**| **21.459000**|


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 314 | 1421 | 1083 | 305 |
| chinese_wiki.txt | 1514 | 2439 | 2059 | 1340 |
| hindi_wiki.txt | 2024 | 2873 | 2516 | 1977 |
| japanese_wiki.txt | 2096 | 2736 | 2465 | 1872 |
| korean_wiki.txt | 1671 | 2735 | 2292 | 1624 |
| portuguese_wiki.txt | 567 | 1836 | 1363 | 541 |
| russian_wiki.txt | 1809 | 2879 | 2558 | 1823 |
| swedish_wiki.txt | 609 | 1916 | 1334 | 574 |
| stress_test_0.txt | 197 | 1310 | 1007 | 201 |
| stress_test_1.txt | 2686 | 2576 | 2746 | 2021 |
| stress_test_2.txt | 2390 | 2149 | 2545 | 3138 |
| hindi_wiki_in_english.txt | 333 | 1452 | 1078 | 328 |
| hindi_wiki_in_russian.txt | 1777 | 2947 | 2486 | 1852 |
| kermit.txt | 863 | 2176 | 1513 | 987 |
| z1_kosme.txt | 1157 | 2421 | 1941 | 982 |
| z1_ascii.txt | 903 | 2367 | 1942 | 628 |
| **sum**| **20.910000s**| **36.233000s**| **30.928000s**| **20.193000s**|

## Celeron N2920


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 2444 | 4727 | 4074 | 2479 |
| chinese_wiki.txt | 4300 | 7281 | 7582 | 4922 |
| hindi_wiki.txt | 5244 | 8100 | 8563 | 6324 |
| japanese_wiki.txt | 5572 | 8607 | 9640 | 6728 |
| korean_wiki.txt | 4597 | 7467 | 7292 | 5477 |
| portuguese_wiki.txt | 2294 | 5075 | 4548 | 2400 |
| russian_wiki.txt | 5775 | 7587 | 7415 | 5830 |
| swedish_wiki.txt | 2279 | 5046 | 4586 | 2439 |
| stress_test_0.txt | 1018 | 4378 | 3610 | 1113 |
| stress_test_1.txt | 6837 | 10466 | 12608 | 9164 |
| stress_test_2.txt | 7029 | 11479 | 8894 | 16328 |
| hindi_wiki_in_english.txt | 1959 | 4775 | 4105 | 1979 |
| hindi_wiki_in_russian.txt | 5519 | 7440 | 7245 | 5626 |
| kermit.txt | 3670 | 6241 | 6080 | 3944 |
| z1_kosme.txt | 4789 | 7013 | 9184 | 4203 |
| z1_ascii.txt | 3159 | 5263 | 7205 | 1989 |
| **sum**| **66.485000**| **110.945000**| **112.631000**| **80.945000**|


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 1281 | 4585 | 4322 | 1233 |
| chinese_wiki.txt | 4246 | 7154 | 7403 | 4776 |
| hindi_wiki.txt | 5305 | 7944 | 8541 | 6180 |
| japanese_wiki.txt | 5720 | 8477 | 9183 | 6598 |
| korean_wiki.txt | 4521 | 7332 | 7471 | 5250 |
| portuguese_wiki.txt | 1837 | 4948 | 4655 | 1827 |
| russian_wiki.txt | 5531 | 7524 | 7674 | 5592 |
| swedish_wiki.txt | 1957 | 4970 | 4701 | 2003 |
| stress_test_0.txt | 712 | 4373 | 4107 | 631 |
| stress_test_1.txt | 7588 | 10452 | 11832 | 9454 |
| stress_test_2.txt | 7166 | 11440 | 9913 | 15906 |
| hindi_wiki_in_english.txt | 1261 | 4632 | 4421 | 1197 |
| hindi_wiki_in_russian.txt | 5390 | 7388 | 7525 | 5506 |
| kermit.txt | 3450 | 6162 | 6258 | 3629 |
| z1_kosme.txt | 4968 | 6899 | 9858 | 4195 |
| z1_ascii.txt | 3066 | 5086 | 8188 | 1800 |
| **sum**| **63.999000s**| **109.366000s**| **116.052000s**| **75.777000s**|

## Raspberry PI 3



