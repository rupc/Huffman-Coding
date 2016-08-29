#include "huffman-converter.h"
// read raw file and build the fTab of pairs of character and its frequency
Bytes HuffmanConverter::build_freq_table(std::ifstream &inFile) {
    unsigned char ch; 
    Bytes bytes = 0;
    while(inFile >> std::noskipws >> ch) {
        ++fTab[ch];
        ++bytes;
    }
    return bytes;
}
HuffmanNode* HuffmanConverter::build_prefix_tree() {
    priQ pq;
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
    std::string bit_buffer = ""; 
    bit_buffer.reserve(64);
    unsigned char ch; 

    while(inFile >> std::noskipws >> ch) {
        bit_sequence += "." + eTab[ch];
        bit_buffer += eTab[ch];
        // assumes each encoded string is less than 64
        if (bit_buffer.size() >= 64) {
            std::bitset<64> bv(std::string(bit_buffer.begin(), bit_buffer.begin()+64));
            bit_buffer = std::string(bit_buffer.begin()+64, bit_buffer.end());
            unsigned long long binary_form = bv.to_ullong();
            outFile.write(reinterpret_cast<const char *>(&binary_form), sizeof(binary_form));
            //std::cout << std::hex << binary_form << "\n";
        }
    }
    // print remainder
    if (bit_buffer.size() > 0) {
            std::bitset<64> bv(std::string(bit_buffer.begin(), bit_buffer.end()));
            unsigned long long binary_form = bv.to_ullong();
            outFile.write(reinterpret_cast<const char *>(&binary_form), sizeof(binary_form));
            //std::cout << std::hex << binary_form << "\n";
    }
    // print remainder
    //std::cout << bit_sequence << "\n";
    //std::cout << bit_buffer << "\n";
}
void HuffmanConverter::write_freq_table(std::ofstream &inFile) {
    for (auto &p : fTab) {
        inFile << (unsigned)p.first << " " << p.second << "\n";
    }
}

// wrapper function, ordering huffman basic functions
void HuffmanConverter::encode_file(const char *input, const char *output = nullptr) {
    std::ifstream inFile(input);
    std::ofstream outFile(output);
    std::ofstream outTable(std::string(output) + ".tab");
    if (!inFile.is_open()) {
        std::cerr << "File name:" input << "doesn't exist." << std::endl;
        return;
    }
    build_freq_table(inFile);
    build_prefix_tree();
    encode_symbol();
    write_to_binary(inFile, outFile);
    write_freq_table(outTable);

    inFile.close();
    outFile.close();
    outTable.close();
}
