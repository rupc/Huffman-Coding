#include "huffman-converter.h"
// read raw file and build the fTab of pairs of character and its frequency
void HuffmanConverter::build_freq_table(std::ifstream &inFile) {
    unsigned char ch; 
    while(inFile >> std::noskipws >> ch) {
        ++fTab[ch];
    }
}
HuffmanNode* HuffmanConverter::build_prefix_tree() {
    PQ pq;
    for(auto &w : fTab) {
        pq.push(HuffmanNode(w.first, w.second));
    }
    int total = pq.size();
    for(int i = 0 ; i < total - 1; ++i) {
        HuffmanNode *h1 = new HuffmanNode(pq.top()); pq.pop();
        HuffmanNode *h2 = new HuffmanNode(pq.top()); pq.pop();
        HuffmanNode *new_node = new HuffmanNode(h1->freq + h2->freq, h1, h2);
        pq.push(*new_node);
    }
    root = new HuffmanNode(pq.top()); pq.pop();
    return root;
}

// call this function for encode 
void HuffmanConverter::encode_symbol() {
    encode_symbol_util(root, eTab, std::string());
}
void HuffmanConverter::encode_symbol_util(HuffmanNode *node, EncodeTable &eTab, std::string encStr) {
    if (node->left == nullptr && node->right == nullptr) {
        eTab[node->symbol] = encStr;
    } else {
        encode_symbol_util(node->left, eTab, encStr + "0");
        encode_symbol_util(node->right, eTab, encStr + "1");
    }
}

void HuffmanConverter::write_to_binary(std::ifstream& inFile, std::ofstream &outFile) {
    inFile.clear();
    inFile.seekg(0, std::ios::beg);

    std::string bit_sequence = "";
    bit_sequence.reserve(64);
    unsigned char ch; 
    while(inFile >> std::noskipws >> ch) {
        bit_sequence += eTab[ch];
    }
    //auto 
    std::cout << bit_sequence << "\n";
}

// wrapper function, ordering huffman basic functions
void encode_files(std::ifstream &file) {

}
