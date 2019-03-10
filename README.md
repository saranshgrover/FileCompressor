# File Compressor

A Unix based file compressor and decompressor written in C using huffman tree encoding

## Installation & Usage

**Important: Only works on Unix machines (for now)**

Download source code [on github](https://pip.pypa.io/en/stable/) to install the compression



```C
bin/huff -c -b 1024 < file/to/compress > file/compress/to // Compress with blocksize 1024.
bin/huff -d < compressed/file > output/file

```

Done for CSE 320.

