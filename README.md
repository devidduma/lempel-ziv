# lempel-ziv
Simple yet efficient Lempel-Ziv-78 and Lempel-Ziv-Welch algorithms.

ID numbers of lookup tables are saved in base 128 as characters. The first bit of each characters denotes whether that character is the last one representing the ID number or not.

Compression ratios tested once or twice:
Lempel-Ziv-78 achieved circa 230% compression ratio
Lempel-Ziv-Welch achieved circa 300% compression ratio
