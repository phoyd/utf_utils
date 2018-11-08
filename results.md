
# Benchmark results

created: Fri Nov  9 00:13:28 CET 2018 
command line: ./utf_utils_test -dd ../test_data -rx 31 -al phoyd,kewb-fast,kewb-fast-unrolled,kewb-sse
## Ryzen 1700


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 391 | 2157 | 1298 | 652 |
| chinese_wiki.txt | 1428 | 2516 | 1916 | 1360 |
| hindi_wiki.txt | 1899 | 2789 | 2187 | 1828 |
| japanese_wiki.txt | 1832 | 2442 | 2004 | 1619 |
| korean_wiki.txt | 1624 | 2894 | 2186 | 1676 |
| portuguese_wiki.txt | 676 | 2384 | 1535 | 896 |
| russian_wiki.txt | 1882 | 2825 | 2289 | 1708 |
| swedish_wiki.txt | 735 | 2396 | 1567 | 916 |
| stress_test_0.txt | 234 | 2024 | 1196 | 555 |
| stress_test_1.txt | 1861 | 1722 | 1588 | 1616 |
| stress_test_2.txt | 1937 | 1762 | 1444 | 3182 |
| hindi_wiki_in_english.txt | 438 | 2144 | 1303 | 671 |
| hindi_wiki_in_russian.txt | 1860 | 2910 | 2075 | 1733 |
| kermit.txt | 1051 | 2614 | 1722 | 1230 |
| z1_kosme.txt | 894 | 2532 | 1738 | 886 |
| z1_ascii.txt | 759 | 2790 | 1667 | 463 |
| **sum**| **19501**| **38901**| **27715**| **20991**|


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 303 | 1271 | 1159 | 663 |
| chinese_wiki.txt | 1474 | 2099 | 1952 | 1438 |
| hindi_wiki.txt | 1959 | 2535 | 2416 | 1975 |
| japanese_wiki.txt | 1916 | 2266 | 2250 | 1764 |
| korean_wiki.txt | 1673 | 2488 | 2223 | 1745 |
| portuguese_wiki.txt | 609 | 1552 | 1381 | 912 |
| russian_wiki.txt | 1733 | 2538 | 2360 | 1883 |
| swedish_wiki.txt | 694 | 1639 | 1440 | 951 |
| stress_test_0.txt | 175 | 1199 | 1097 | 571 |
| stress_test_1.txt | 2217 | 2063 | 2090 | 1809 |
| stress_test_2.txt | 1814 | 2076 | 1873 | 3512 |
| hindi_wiki_in_english.txt | 338 | 1381 | 1184 | 694 |
| hindi_wiki_in_russian.txt | 1724 | 2577 | 2289 | 1919 |
| kermit.txt | 1033 | 2042 | 1750 | 1315 |
| z1_kosme.txt | 1049 | 1771 | 1427 | 903 |
| z1_ascii.txt | 785 | 1649 | 1323 | 460 |
| **sum**| **19496**| **31146**| **28214**| **22514**|

## Opteron 3280


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 997 | 3567 | 2358 | 1046 |
| chinese_wiki.txt | 2738 | 4146 | 3869 | 2358 |
| hindi_wiki.txt | 3504 | 4509 | 4636 | 3382 |
| japanese_wiki.txt | 3612 | 4181 | 4585 | 3180 |
| korean_wiki.txt | 3050 | 4737 | 4379 | 2976 |
| portuguese_wiki.txt | 1311 | 3791 | 2687 | 1212 |
| russian_wiki.txt | 3474 | 4587 | 4900 | 3024 |
| swedish_wiki.txt | 1378 | 3780 | 2727 | 1296 |
| stress_test_0.txt | 607 | 3459 | 2305 | 647 |
| stress_test_1.txt | 5250 | 3708 | 5430 | 3757 |
| stress_test_2.txt | 4729 | 3254 | 4388 | 7284 |
| hindi_wiki_in_english.txt | 1024 | 3642 | 2400 | 956 |
| hindi_wiki_in_russian.txt | 3349 | 4525 | 4832 | 2988 |
| kermit.txt | 2269 | 4156 | 3671 | 2145 |
| z1_kosme.txt | 1931 | 4279 | 3249 | 1617 |
| z1_ascii.txt | 1495 | 3877 | 2727 | 831 |
| **sum**| **40718**| **64198**| **59143**| **38699**|


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 614 | 2300 | 2037 | 561 |
| chinese_wiki.txt | 2914 | 4495 | 3212 | 2419 |
| hindi_wiki.txt | 3776 | 5535 | 3824 | 3314 |
| japanese_wiki.txt | 3955 | 5763 | 3688 | 3222 |
| korean_wiki.txt | 3296 | 4888 | 3619 | 2947 |
| portuguese_wiki.txt | 1156 | 2775 | 2387 | 1003 |
| russian_wiki.txt | 3453 | 6098 | 3951 | 2968 |
| swedish_wiki.txt | 1261 | 2887 | 2393 | 1123 |
| stress_test_0.txt | 420 | 2179 | 1980 | 345 |
| stress_test_1.txt | 5200 | 7889 | 3992 | 3919 |
| stress_test_2.txt | 4925 | 6161 | 3450 | 7414 |
| hindi_wiki_in_english.txt | 712 | 2414 | 2163 | 662 |
| hindi_wiki_in_russian.txt | 3491 | 6150 | 3846 | 3049 |
| kermit.txt | 2238 | 4134 | 3170 | 2103 |
| z1_kosme.txt | 2118 | 4318 | 4294 | 2940 |
| z1_ascii.txt | 1790 | 2668 | 3001 | 712 |
| **sum**| **41319**| **70654**| **51007**| **38701**|

## Core i5-3320m


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 513 | 1487 | 1138 | 484 |
| chinese_wiki.txt | 1514 | 2372 | 2111 | 1340 |
| hindi_wiki.txt | 2049 | 2647 | 2313 | 1942 |
| japanese_wiki.txt | 2285 | 2478 | 2290 | 1848 |
| korean_wiki.txt | 1723 | 2655 | 2279 | 1616 |
| portuguese_wiki.txt | 703 | 1991 | 1578 | 636 |
| russian_wiki.txt | 1912 | 2741 | 2432 | 1841 |
| swedish_wiki.txt | 734 | 2101 | 1662 | 665 |
| stress_test_0.txt | 429 | 1329 | 1090 | 432 |
| stress_test_1.txt | 2443 | 2035 | 2161 | 2009 |
| stress_test_2.txt | 2330 | 1878 | 2012 | 3994 |
| hindi_wiki_in_english.txt | 518 | 1472 | 1160 | 505 |
| hindi_wiki_in_russian.txt | 1860 | 2772 | 2211 | 1782 |
| kermit.txt | 981 | 2136 | 1538 | 1037 |
| z1_kosme.txt | 1151 | 2566 | 2336 | 862 |
| z1_ascii.txt | 985 | 2821 | 2395 | 683 |
| **sum**| **22130**| **35481**| **30706**| **21676**|


### Convert UTF8->UTF16

| file\algo| phoyd | kewb-fast | kewb-fast-unrolled | kewb-sse |
| --- | --- | --- | --- | --- |
| english_wiki.txt | 334 | 1447 | 1086 | 304 |
| chinese_wiki.txt | 1684 | 2448 | 2076 | 1355 |
| hindi_wiki.txt | 2183 | 2900 | 2531 | 1978 |
| japanese_wiki.txt | 2347 | 2758 | 2488 | 1878 |
| korean_wiki.txt | 1822 | 2728 | 2307 | 1622 |
| portuguese_wiki.txt | 624 | 1847 | 1362 | 545 |
| russian_wiki.txt | 1900 | 2913 | 2569 | 1858 |
| swedish_wiki.txt | 640 | 1924 | 1356 | 575 |
| stress_test_0.txt | 211 | 1332 | 1018 | 200 |
| stress_test_1.txt | 3043 | 2604 | 3053 | 2040 |
| stress_test_2.txt | 2730 | 2171 | 2557 | 3161 |
| hindi_wiki_in_english.txt | 364 | 1446 | 1089 | 343 |
| hindi_wiki_in_russian.txt | 1880 | 2977 | 2496 | 1862 |
| kermit.txt | 942 | 2188 | 1525 | 985 |
| z1_kosme.txt | 1228 | 2505 | 1925 | 964 |
| z1_ascii.txt | 975 | 2351 | 1939 | 627 |
| **sum**| **22907**| **36539**| **31377**| **20297**|

## Celeron N2920



## Raspberry PI 3



