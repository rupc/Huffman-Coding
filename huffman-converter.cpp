#include "huffman-converter.h"
#include <iostream>
#include <fstream>

const char *medium_file = "medium.txt";
const char *small_file = "small.txt";
const char *large_file = "large.txt";

int main(int argc, const char *argv[]) {
    HuffmanConverter hConverter;
    hConverter.encode_file(large_file, nullptr);
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

unsigned long long get_file_size(std::ifstream &is) {
    is.seekg(0, std::ios::end);
    unsigned long long blk_sz = is.tellg();
    is.seekg(0, std::ios::beg);
    return blk_sz;
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
