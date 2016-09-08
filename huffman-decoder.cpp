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
            //std::cout << "h_node->right> " << h_node->freq << std::endl;
        } else {
            h_node = h_node->left;
            //std::cout << "h_node->left> " << h_node->freq << std::endl;
        }
        if(h_node->right == nullptr && h_node->left == nullptr) {
            result += h_node->symbol;
            //std::cout << " >leaf node : " << h_node->symbol << std::endl;
            h_node = root;
        }
        ++cnt;
    }
    return result;
}
void HuffmanConverter::decode_file(const char *inFile, const char *outFile) {
    // read from table and build frequency tree
    // build prefix tree
    // read .huf file and get bit string during reaching to leaf node of prefix tree
    // At each bit string, map it to one character in inverted encode table
    // Nessasary : .huf .tab
    std::string tpath = format_path_name(path_freq, inFile, postfix_tab); // table file's path
    std::string hpath = format_path_name(path_encoded, inFile, postfix_huf); // input file's path
    std::string dpath = std::string(path_decoded).append(inFile); // input file's path
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
    unsigned ch;
    unsigned cnt = 0;
    while (tabFile >> ch >> cnt) {
        std::cout << ch << " " << cnt << "\n";
        fTab[(unsigned char)ch] = cnt;
    }
    build_prefix_tree();
    encode_symbol();
    // read all binaries into memory
    unsigned long long bin_sz = get_file_size(hpath);
    //std::cout << bin_sz << "\n";
    char *buf = new char[bin_sz];
    hufFile.read(buf, bin_sz);

    unsigned char x = 0;
    std::string bit_string = "";

    for(int i = 0 ; i < bin_sz ; ++i) {
        x |= buf[i];
        bit_string += get_bit_string(buf[i]);
        x = 0x0;
    }
    std::cout << "bit_string >" << bit_string << std::endl;
    std::cout << bit_string.size() << "\n";

    std::cout << "\n";
    std::cout << parse_bitstr(bit_string);
    std::cout << "\n";
    hufFile.close();
}
