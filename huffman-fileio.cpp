#include "huffman-converter.h"
#include <iostream>
#include <fstream>

// read raw file and build the table of pairs of character and its frequency
void HuffmanConverter::huffman_build_table(std::ifstream &inFile) {
    unsigned char ch; 
    while(inFile >> std::noskipws >> ch) {
        ++table[ch];
    }
}

void HuffmanConverter::print_huffman_table(std::ostream &out) {
    if (table.empty()) {
        out << "Need to build table first\n";
        return;
    }
    out << "Character Frequency Table\n";
    for (const auto &p : table) {
        out << (unsigned)p.first << ":" << p.second << "\n";
    }
}
