#include "huffman-converter.h"
void HuffmanConverter::decode_file(const char *inFile, const char *outFile) {
    // read from table and build frequency tree
    // build prefix tree
    // read .huf file and get bit string during reaching to leaf node of prefix tree
    // At each bit string, map it to one character in inverted encode table
    std::string epath = std::string(path_encoded).append(inFile); // input file's path
    std::string tpath = format_path_name(path_freq, input, postfix_tab); // table file's path
    std::string opath = format_path_name(path_encoded, input, postfix_huf); // input file's path
    std::ifstream enFile(epath);
    std::ifstream tabFile(tpath);
    std::ofstream deFile(opath, std::ios::binary);
    // empty 
    fTab.clear();
    eTab.clear();

    build_freq_table(tabFile);
    // read all binaries into memory
    unsigned long long bin_sz = get_file_size(enFile);
    char *buf = new char[bin_sz];
    enFile.read(buf, bin_sz);
    enFile.close();
}
