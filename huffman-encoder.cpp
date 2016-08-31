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
    std::string bit_buffer = ""; 
    bit_sequence.reserve(64);
    bit_buffer.reserve(64);
    unsigned char ch; 
    while(inFile >> std::noskipws >> ch) {
        //bit_sequence += "." + eTab[ch];
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
    /*std::cout << bit_sequence << "\n";
    std::cout << bit_buffer << "\n";*/
}
void HuffmanConverter::write_freq_table(std::ofstream &inFile) {
    for (auto &p : fTab) {
        inFile << (unsigned)p.first << " " << p.second << "\n";
    }
}

const char *loc_text = "texts/";
const char* path_freq = "./frequency-table/";
const char* path_output = "./encoded-files/";
const char *postfix_tab =".tab";
const char *postfix_huf =".huf";
// just concat 3 strings of path, input, postfix

std::string format_path_name(const char *path, const char *input, const char *postfix) {
    return (std::string(path).append(input).append(postfix));
}
// wrapper function, ordering huffman basic functions
void HuffmanConverter::encode_file(const char *input, const char *output = nullptr) {
    // get the pathes needed
    std::string ipath = std::string(loc_text) + input; // input file's path
    std::string opath = format_path_name(path_output, input, postfix_huf); // output file's path
    std::string tpath = format_path_name(path_freq, input, postfix_tab); // table file's path

    std::ifstream inFile(ipath);
    std::ofstream outFile(opath);
    std::ofstream outTable(tpath);

    if (!inFile.is_open()) {
        std::cerr << "File name:" << input << " doesn't exist." << std::endl;
        std::cerr << "Encoding fails!" << std::endl;
        return;
    }
    build_freq_table(inFile);
    build_prefix_tree();
    encode_symbol();
    write_to_binary(inFile, outFile);
    write_freq_table(outTable);
    //std::cout << "Size of original file:" << total << "\n";
    printf("%-20s : %s\n", "File Name", input);
    printf("%-20s : %u\n", "Different Characters", (unsigned)fTab.size());

    /*printf("%-20s : %llu\n", "Original Size", total);
    printf("%-20s : %llu -> %lld\n", "Change Info", total, after);
    printf("%-20s : %-4f%%\n", "Compress rate", (double)after/total*100.0);*/

    inFile.clear();
    inFile.seekg(0, std::ios_base::beg);
    std::ifstream afterFile(opath, std::ios::in | std::ios::binary);

    double zip_rate = compare_rate(inFile, afterFile);
    printf("%-20s : %-4.4f%%\n", "Compress rate", zip_rate);
    inFile.close();
    outFile.close();
    outTable.close();
    afterFile.close();
}
double HuffmanConverter::compare_rate(std::ifstream &before, std::ifstream &after) {
    unsigned long long before_sz = get_file_size(before);
    unsigned long long after_sz = get_file_size(after);
    //std::cout << before_sz << " " << after_sz << "\n";
    return ((double)after_sz/before_sz)*100.0;
}
