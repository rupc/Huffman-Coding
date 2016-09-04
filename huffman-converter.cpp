#include "huffman-converter.h"
#include <iostream>
#include <fstream>

const char *large_file = "large.txt";
const char *very_large = "very-large.txt";
const char *medium_file = "medium.txt";
const char *small_file = "small.txt";

int main(int argc, const char *argv[]) {
    HuffmanConverter hConverter;
    hConverter.encode_file(medium_file, nullptr);
    return 0;
}
unsigned get_file_size(const std::string& address) {
  std::fstream motd(address.c_str(), std::ios::binary|std::ios::in|std::ios::ate);
  if(motd) {
    std::fstream::pos_type size = motd.tellg();
    //std::cout << address << " " << size << "\n";
    return (unsigned)size;
  } else {
    perror(address.c_str());
    return 0;
  }
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
