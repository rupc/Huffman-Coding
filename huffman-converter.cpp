#include "huffman-converter.h"
#include <iostream>
#include <fstream>

int main(int argc, const char *argv[]) {
    const char *file1 = "texts/random-text1.txt";
    const char *small_file = "texts/small.txt";
    const char *output = "huff";
    std::ifstream inFile(small_file);
    std::ofstream outFile(output);
    HuffmanConverter hConverter;
    hConverter.build_freq_table(inFile);
    hConverter.print_huffman_table(std::cout);
    hConverter.build_prefix_tree();
    hConverter.encode_symbol();
    hConverter.print_enocde_table(std::cout);
    hConverter.write_to_binary(inFile, outFile);
    return 0;
}
bool operator< (const HuffmanNode &h1, const HuffmanNode &h2) {
    return h1.freq > h2.freq;
}
HuffmanNode::HuffmanNode(unsigned char c, unsigned f)
    : symbol(c), freq(f) {
        left = right = nullptr;
}
HuffmanNode::HuffmanNode(const HuffmanNode& h) {
    symbol = h.symbol;
    freq = h.freq;
    left = h.left; right = h.right;
}
HuffmanNode::HuffmanNode(int f, HuffmanNode *l, HuffmanNode *r) 
    : freq(f), left(l), right(r) {}

unsigned get_file_size(std::ifstream &is) {
    /*is.seekg(0, is.end());
    unsigned blk_sz = is.tellg();
    is.seekg(0, is.beg());
    return blk_sz;*/
}
void HuffmanConverter::print_enocde_table(std::ostream &out) {
    if (eTab.empty()) {
        out << "Need to encode characters first\n";
        return;
    }
    out << "|------------------------|\n";
    out << "| Character Encode table |\n";
    out << "|------------------------|\n";
    for (const auto &p : eTab) {
        printf("| %3d(%c) -> ", (unsigned)p.first, p.first);
        for (const auto &q : p.second) {
            out << q;
        }
        out << "\n";
    }
}
void HuffmanConverter::print_huffman_table(std::ostream &out) {
    if (fTab.empty()) {
        out << "Need to build fTab first\n";
        return;
    }
    out << "|---------------------------|\n";
    out << "| Character Frequency table |\n";
    out << "|---------------------------|\n";
    for (const auto &p : fTab) {
        printf("| %3d : %3d\n", (unsigned)p.first, p.second);
        //out << (unsigned)p.first << ":" << p.second << "\n";
    }
}
