## What is Huffman Encoding?
Huffman encoding is an algorithm which outputs compressed data from input file(usually ascii format). By using greedy approach, it encodes most ouccuring character as shortest one in binary format. This technique is based on binary tree, priority queue.

---


## How to use?
### Encoding
The original files to be encoded should be in texts folder. and then,  
> ./huffman-converter -e [file-name]
Encoding process generates 2 files: the encoded file, table file. Each has its location at encoded-files and frequency folder. 

#### Table
Table information is important for decoding process. Table file has ".tab" as its extension. And It consists of three part.
> N(Number of different characters)
> Pairs of Character Frequency 
> ....
> ....
> K(Last ending Position)

I want to mention that why K is needed. Huffman encoding actually encodes characters byte by byte as binary. But, the last byte might not be fully filled. So if you want to decode the compressed binary file, you have to know which bit in last byte end.

### Decoding
Decoding process needs 2 files as input: .huf, .tab 
you can use like that.
> ./huffman-converter -d [file-name]

### Etc
you can see also the version of program and help information as follow commnad.
> ./huffman-converter -v
> ./huffman-converter -h

## Blah Blah
This is the improved version of my old program, which was written at my junior class assigment. Back then, I had removed Line feed character for mistake because it wasn't show up when I print it. I thought it was buggy. Now I understand why it was not. Just looking into the ascii table and printing it as a number is enough.
