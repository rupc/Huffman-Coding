#include "huffman-converter.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

#define NFILE 4
const char *files[NFILE] = {"small.txt","medium.txt", "large.txt", "very-large.txt"};

void help();
void version();
int main(int argc, char *argv[]) {
    HuffmanConverter hConverter;
    int opt;
    while ((opt = getopt(argc, argv, "hve:d:")) != -1) {
        switch (opt) {
            case 'h':
                help();
                break;
            case 'v':
                version();
                break;
            case 'e':
                std::cout << optarg << "\n";
                break;
            case 'd':
                std::cout << optarg << "\n";
                break;
            default:
               break; 
        }
    }
    /*if (argc == 1) {
        //hConverter.encode_file(files[3], nullptr);
        hConverter.decode_file(files[3], nullptr);
    } else {
        std::cout << "ya" << "\n";
    }*/
    //hConverter.print_enocde_table(std::cout);
    return 0;
}
void help() {
    printf("Encode option : -e [file-path]\nexample) ./huffman-converter -e texts/large.txt\n");
    printf("Decode option : -d [file-path]\nexample) ./huffman-converter -e texts/large.txt\n");
    exit(0);
}
void version() {
    printf("Huffman Converter\nVersion : 1.0\n");
    exit(0);
}
unsigned long long get_file_size(const std::string& address) {
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
        if (p.first == 10) {
            printf("| %3d(%s) -> ", (unsigned)p.first, "ws");
        } else {
            printf("| %3d(%c) -> ", (unsigned)p.first, p.first);
        }
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
