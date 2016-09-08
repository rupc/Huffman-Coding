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

unsigned HuffmanConverter::write_to_binary(std::ifstream& inFile, std::ofstream &outFile) {
    inFile.clear();
    inFile.seekg(0, std::ios::beg);

    std::string bit_sequence = "";
    std::string bit_buffer = ""; 
    bit_sequence.reserve(8);
    bit_buffer.reserve(8);
    unsigned char ch; 
    while(inFile >> std::noskipws >> ch) {
        bit_sequence += "." + eTab[ch];
        bit_buffer += eTab[ch];
        // assumes each encoded string is less than 8
        if (bit_buffer.size() >= 8) {
            std::bitset<8> bv(std::string(bit_buffer.begin(), bit_buffer.begin()+8));
            bit_buffer = std::string(bit_buffer.begin()+8, bit_buffer.end());
            unsigned char binary_form = (unsigned char)bv.to_ulong();
            outFile.write(reinterpret_cast<const char *>(&binary_form), sizeof(binary_form));
            //std::cout << std::hex << binary_form << "\n";
        }
    }
    std::cout << "bitsize:"<<bit_buffer.size() << "\n";
    // print remainder
    if (bit_buffer.size() > 0) {
        std::bitset<8> bv(std::string(bit_buffer.begin(), bit_buffer.end()));
        unsigned char binary_form = (unsigned char)bv.to_ulong();
        for (int i = 0; i < 8-bit_buffer.size(); i++) {
            binary_form = binary_form << 1;
        }
        outFile.write(reinterpret_cast<const char *>(&binary_form), sizeof(binary_form));
        //std::cout << std::hex << binary_form << "\n";
    }
    // print remainder
    //std::cout << bit_sequence << "\n";
    //std::cout << bit_buffer << "\n";
    return bit_buffer.size();
}
void HuffmanConverter::write_freq_table(std::ofstream &inFile, unsigned last_pos) {
    inFile << fTab.size() << "\n";
    for (auto &p : fTab) {
        inFile << (unsigned)p.first << " " << p.second << "\n";
    }
    inFile << last_pos;
}

// wrapper function, ordering huffman basic functions
void HuffmanConverter::encode_file(const char *input, const char *output = nullptr) {
    // get the pathes needed
    std::string ipath = std::string(loc_text) + input; // input file's path
    std::string opath = format_path_name(path_encoded, input, postfix_huf); // output file's path
    std::string tpath = format_path_name(path_freq, input, postfix_tab); // table file's path

    std::ifstream inFile(ipath);
    std::ofstream outFile(opath);
    std::ofstream outTable(tpath);
    std::ifstream afterFile;
    if (!inFile.is_open()) {
        std::cerr << "File name:" << input << " medium_file't exist." << std::endl;
        std::cerr << "Encoding fails!" << std::endl;
        return;
    }
    // core functions
    build_freq_table(inFile);
    build_prefix_tree();
    encode_symbol();
    unsigned last_pos = write_to_binary(inFile, outFile);
    write_freq_table(outTable, last_pos);

    inFile.clear();
    inFile.seekg(0, std::ios_base::beg);
    afterFile.open(opath, std::ios::in | std::ios::binary);
    if (!afterFile.is_open()) {
        std::cerr << "Cannot find encoded files" << std::endl;
        std::cerr << "Encoding fails" << std::endl;
        return;
    }
    afterFile.clear();
    afterFile.seekg(0, std::ios_base::beg);

    /*printf("%-20s : %llu\n", "Original Size", total);
    printf("%-20s : %llu -> %lld\n", "Change Info", total, after);
    printf("%-20s : %-4f%%\n", "Compress rate", (double)after/total*100.0);*/

    printf("%-20s : %s\n", "File Name", input);
    printf("%-20s : %u\n", "Different Characters", (unsigned)fTab.size());

    /*std::cout << ipath << "\n";
    std::cout << opath << "\n";*/
    inFile.close();
    outFile.close();
    outTable.close();
    afterFile.close();
    compare_rate(ipath, opath);
}
double HuffmanConverter::compare_rate(const std::string &before, const std::string &after) {
    unsigned before_sz = get_file_size(before);
    unsigned after_sz = get_file_size(after);
    double zip_rate = 100.0 - ((double)after_sz/before_sz)*100.0;
    printf("%-20s : %d -> %d\n","Size Change(bytes)", before_sz, after_sz);
    printf("%-20s : %-4.2f%%\n", "Compression Rate", zip_rate);
    return  zip_rate;
}
