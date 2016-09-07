#include "huffman-converter.h"
std::string getBitString(unsigned char c) {
    std::string BitString = "";
    unsigned char shift = 0x1; shift <<= 7;
    for(int i = 0 ; i < 8 ; ++i) {
        if(c & shift) BitString += "1";
        else BitString += "0";
        shift >>= 1;
    }
    return BitString;
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

    build_freq_table(tabFile);
    // read all binaries into memory
    unsigned long long bin_sz = get_file_size(hpath);
    //std::cout << bin_sz << "\n";
    char *buf = new char[bin_sz];
    hufFile.read(buf, bin_sz);

    unsigned char x = 0;
    std::string BitString = "";

    for(int i = 0 ; i < bin_sz ; ++i) {
        x |= buf[i];
        BitString += getBitString(buf[i]);
        x = 0x0;
    }
    std::cout << "BitString >" << BitString << std::endl;
    std::cout << BitString.size() << "\n";

    hufFile.close();
}
