
#include "huffman-converter.h"
#include <iostream>
#include <fstream>
bool operator< (const HuffmanNode &h1, const HuffmanNode &h2) {
    return h1.freq > h2.freq;
}
HuffmanNode::HuffmanNode(unsigned char c, unsigned f)
    : symbol(c), freq(f) {
        left = right = nullptr;
}
HuffmanNode::HuffmanNode(HuffmanNode& hn) {
    symbol = hn.symbol;
    freq = hn.freq;
    left = hn.left; right = hn.right;
}
HuffmanNode::HuffmanNode(int f, HuffmanNode *l, HuffmanNode *r) 
    : freq(f), left(l), right(r) {}

unsigned get_file_size(std::ifstream &is) {
    /*is.seekg(0, is.end());
    unsigned blk_sz = is.tellg();
    is.seekg(0, is.beg());
    return blk_sz;*/
}
int main(int argc, const char *argv[]) {
    const char *file1 = "texts/random-text1.txt";
    std::ifstream inFile(file1);

    HuffmanConverter hConverter;
    hConverter.huffman_build_table(inFile);
    //hConverter.print_huffman_table(std::cout);
    //hConverter.huffman_build_tree();
    return 0;
}
