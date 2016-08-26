#include "huffman-converter.h"
#include <iostream>
#include <fstream>

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
