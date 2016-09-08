#include "huffman-converter.h"
std::string get_bit_string(unsigned char c) {
    std::string BitString = "";
    unsigned char shift = 0x1; shift <<= 7;
    for(int i = 0 ; i < 8 ; ++i) {
        if(c & shift) BitString += "1";
        else BitString += "0";
        shift >>= 1;
    }
    return BitString;
}
std::string HuffmanConverter::parse_bitstr(std::string& bit_string) {
    int cnt = 0, sz = bit_string.size();
    HuffmanNode *h_node = root;
    std::string result = "";
    while(cnt != sz) {
        if(bit_string[cnt] == '1') {
            h_node = h_node->right;
        } else {
            h_node = h_node->left;
        }
        if(h_node->right == nullptr && h_node->left == nullptr) {
            result += h_node->symbol;
            h_node = root;
        }
        ++cnt;
    }
    return result;
}
unsigned HuffmanConverter::parse_freq_table(std::ifstream& tabFile) {
    unsigned nOfChars, ch, cnt, last_pos;
    tabFile >> nOfChars;
    while (nOfChars--) {
        tabFile >> ch >> cnt;
        fTab[(unsigned char)ch] = cnt;
    }
    tabFile >> last_pos;
    return last_pos;
}
// fill bit string from binary buffer
void HuffmanConverter:: build_bit_string(char *buf, unsigned bSize, std::string &bit_string, unsigned last_pos) {
    unsigned char x = 0;
    for(int i = 0 ; i < bSize ; ++i) {
        x |= buf[i];
        bit_string += get_bit_string(buf[i]);
        x = 0x0;
    }
    for (int i = 0; i < 8-last_pos; i++) {
        bit_string.pop_back();
    }
}
void HuffmanConverter::decode_file(const char *inFile, const char *outFile) {
    // read from table and build frequency tree
    // build prefix tree
    // read .huf file and get bit string during reaching to leaf node of prefix tree
    // At each bit string, map it to one character in inverted encode table
    // Nessasary : .huf .tab
    std::string tpath = format_path_name(path_freq, inFile, postfix_tab); // table file's path
    std::string hpath = format_path_name(path_encoded, inFile, postfix_huf); // input file's path
    std::string dpath = std::string(path_decoded).append(inFile); // output file's path
    std::ifstream hufFile(hpath, std::ios::binary);
    std::ifstream tabFile(tpath);
    std::ofstream deFile(dpath);
    /*std::cout << tpath << "\n";
    std::cout << hpath << "\n";
    std::cout << dpath << "\n";*/
    if (!tabFile.is_open())  {
        std::cerr << "tab file doesn't exist" << std::endl;
        return;
    }
    if (!hufFile.is_open()) {
        std::cerr << "target file doesn't exist" << std::endl;
        return;
    }

    // empty 
    fTab.clear();
    eTab.clear();
    unsigned last_pos = parse_freq_table(tabFile);

    build_prefix_tree();
    encode_symbol();
    // read all binaries into memory
    unsigned long long bin_sz = get_file_size(hpath);
    char *buf = new char[bin_sz];
    hufFile.read(buf, bin_sz);

    std::string bit_string = "";
    build_bit_string(buf, bin_sz, bit_string, last_pos);
    //std::cout << parse_bitstr(bit_string);
    deFile << parse_bitstr(bit_string);
    tabFile.close();
    hufFile.close();
}

