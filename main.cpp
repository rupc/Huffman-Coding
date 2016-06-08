#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <queue>
#include <map>
#include <algorithm>
#include <string>
#include <string.h>
#include <bitset>
#include <iterator>
using namespace std;
string getBitString(unsigned char c);
int getFileSize(ifstream &in);
class huff_node {
    public:
        char symbol;
        int freq;
        huff_node *left;
        huff_node *right;
        huff_node() {}
        huff_node(const huff_node& hn) {
            symbol = hn.symbol;
            freq = hn.freq;
            left = hn.left; right = hn.right;
        }
        huff_node(char s, int f) : symbol(s), freq(f) {
            left = right = nullptr;
        }
        huff_node(int f, huff_node *l, huff_node *r) {
            freq = f;
            left = l; right = r;
        }
};
huff_node root;
void HuffmanCompress(char []);
void HuffmanDecompress(char []);
bool operator < (const huff_node &hc1, const huff_node &hc2) {
    return hc1.freq > hc2.freq;
}
void what_to_process(int argc, char *argv[]) {
    int opt;
    while((opt = getopt(argc, argv, "c:d:")) != -1) {
        switch(opt) {
            case 'c':
                HuffmanCompress(optarg);
                break;
            case 'd':
                HuffmanDecompress(optarg);
                break;
            default:
                cerr << "No matched option exist." << endl;
                break;
        }
    }
}
void PrintCompressionInfo(unsigned int cTotal, map<unsigned char, unsigned int> &ch_freq, unsigned int byteCount) {
    cout << "\n*************** Huffman Compression Information(START) ***************" << endl;
    cout << "Befor Compression\t:\t" << cTotal << endl;
    cout << "After Compression\t:\t" << byteCount << endl;
    cout << "Frequency for each character" << endl;
    for(const auto &w : ch_freq) {
        cout << w.first << " : " << w.second << endl;
    }
    double comp_rate = ((double)byteCount / (double)cTotal) * 100;
    cout << "Compression Rate : " << comp_rate << "%" << endl;
    cout << "*************** Huffman Compression Information(END) ***************" << endl;
}
int PrintSymbolEncoding(map<unsigned char, string> &ch_str) {
    int lineno = 0;
    cout << "\n*************** Huffman Symbol Encoding(START) ***************" << endl;
    for(const auto &w : ch_str) {
        cout << w.first << " " << w.second << endl; ++lineno;
    }
    cout << "*************** Huffman Symbol Encoding(END) ***************" << endl;
    return lineno;
}
huff_node& BuildHuffmanTree(map<unsigned char, unsigned int> &HMT, priority_queue<huff_node> &PQ, bool dec) {
    for(const auto &w : HMT) {
        huff_node huff(w.first, w.second);
//        cout << "Huf Tree Symbol > " << w.first << endl;
        PQ.push(huff);
    }
    int total = PQ.size();
    for(int i = 0 ; i < total - 1; ++i) {
        huff_node *h1 = new huff_node(PQ.top()); PQ.pop();
        huff_node *h2 = new huff_node(PQ.top()); PQ.pop();
        huff_node *new_node = new huff_node(h1->freq + h2->freq, h1, h2);
        PQ.push(*new_node);
    }
    root = PQ.top(); PQ.pop();
    return root;
}
void BuildHuffmanSymbolEncoding(huff_node *root, string symStr, map<unsigned char, string > &table) {
    if(root->left == nullptr && root->right == nullptr) {
        table[root->symbol] = symStr;
  //      cout << "huff symbol> " << root->symbol << endl;
    } else {
        BuildHuffmanSymbolEncoding(root->left, symStr + "0", table);
        BuildHuffmanSymbolEncoding(root->right, symStr + "1", table);
    }
}
void zipFiles(string rawfile, const int &headLineno, const int &byteCount) {
    string file = rawfile;
    ofstream osZip((file + ".zip").c_str(), ios::app | ios::binary);
    ifstream isHuf((file + ".huftable").c_str(), ios::binary);
    ifstream isComp((file + ".hufcomp").c_str(), ios::binary);
    char ch, ws;
    int freq;
    osZip << headLineno << " " << byteCount << endl;
    while(isHuf >> noskipws >> ch >> ws >> freq >> ws) {
        osZip << ch  << " " << freq << endl;
    }
    char *buf = new char[byteCount];
    isComp.read((char *)buf, byteCount); isComp.close();
    osZip.write(buf, byteCount);
}

void unzipFiles(string cfile) {
    string file(cfile);
    cout << cfile << endl;

    ifstream isZip((file + ".zip").c_str(), ios::binary);
    ofstream osComp((file + ".hufcomp").c_str(), ios::binary);
    ofstream osTable((file + ".huftable").c_str());
    char ch, ws; int freq;
    int byteCount; int headLineno;
    isZip >> noskipws >> headLineno >> ws >> byteCount >> ws;
    cout << byteCount << " " <<  headLineno << endl;

    for(int i = 0 ; i < headLineno-1; ++i) {
        isZip >> noskipws >> ch >> ws >> freq >> ws;
        cout << ch << " " << freq << endl;
        osTable << ch << " " << freq << endl;
    }

    char *buf = new char[byteCount];
    isZip.read((char *)buf, byteCount); isZip.close();
    osComp.write(buf, byteCount);
}
int WriteObjectFile(ofstream &ObjectFile, string BitString) {
    int byteCount = 0;
//    cout << "comp BitString > " << BitString << endl;
    unsigned char ul = 0x0;
    bitset<8> bChar(BitString);
    ul = bChar.to_ulong();
//    cout << "first byte : " << getBitString(ul) << endl;
    char ch = (char)ul;
    ObjectFile.write(&ch,1);

    while(BitString.size() > 8) {
        bitset<8> bChar(BitString, 8);
        char ul = bChar.to_ulong();
//        cout << getBitString(ul) << endl;
        ObjectFile.write(&ul,1);
        BitString.erase(0, 8);
        ++byteCount;
    }

    return byteCount;
}
void HuffmanCompress(char rawfile []) {
    ifstream AsciiFile(rawfile);
    string FileName(rawfile);
    ofstream ObjectFile((FileName + ".hufcomp").c_str(), ios::binary);
    ofstream HuffmanFile((FileName + ".huftable").c_str(), ios::binary);
    priority_queue<huff_node> PQ;
    map<unsigned char, unsigned int> HuffmanMapTable;
    map<unsigned char, string > HuffmanSymbolEncoding;
    /*
    At first, We need to find out which chracter set constructs input file and the frequency of each
    chracter occurred
    */
    unsigned char ch; unsigned int cTotal = 0;
    while(AsciiFile >> noskipws >> ch) {
        if(isascii(ch)) {
            ++HuffmanMapTable[ch]; ++cTotal;
        }
    }
    map<unsigned char, unsigned int> ::iterator it = HuffmanMapTable.begin();
    it = HuffmanMapTable.erase(it);
    ++it;
//    cout << "first it > " << it->first << " " << it->second <<  endl;
    string symStr;

    BuildHuffmanTree(HuffmanMapTable, PQ, false);
    BuildHuffmanSymbolEncoding(&root, symStr, HuffmanSymbolEncoding);
    PrintSymbolEncoding(HuffmanSymbolEncoding);
    for(it = HuffmanMapTable.begin(); it != HuffmanMapTable.end(); ++it) {
        HuffmanFile << it->first << " " << it->second << endl;
//        cout << it->first << " " << it->second << endl;
    }
    AsciiFile.clear();
    AsciiFile.seekg(0, ios::beg);
    string BitString = "";

    while(AsciiFile >> noskipws >> ch) {
        if(isascii(ch)) {
            BitString += HuffmanSymbolEncoding[ch];
        }
    }

    int byteCount = WriteObjectFile(ObjectFile, BitString);
    PrintCompressionInfo(cTotal, HuffmanMapTable, byteCount);
    zipFiles(rawfile, HuffmanSymbolEncoding.size(), byteCount);
}

int getFileSize(ifstream &in) {
    in.seekg(0, ios::end);
    int ObjSize = in.tellg();
    in.seekg(0, ios::beg);
    return ObjSize;
}
string getBitString(unsigned char c) {
    string BitString = "";
    unsigned char shift = 0x1; shift <<= 7;
    for(int i = 0 ; i < 8 ; ++i) {
        if(c & shift) BitString += "1";
        else BitString += "0";
        shift >>= 1;
    }
    return BitString;
}
string DecodeHuffmanCode(string &BitString) {
    int cnt = 0;
    huff_node *h_node = &root;
    string result = "";

    while(cnt != BitString.size()) {
        if(BitString[cnt] == '1') {
            h_node = h_node->right;
//            cout << "h_node->right> " << h_node->freq << endl;
        } else {
            h_node = h_node->left;
//            cout << "h_node->left> " << h_node->freq << endl;
        }
        if(h_node->right == nullptr && h_node->left == nullptr) {
            result += h_node->symbol;
//            cout << " >leaf node : " << h_node->symbol << endl;
            h_node = &root;
        }
        ++cnt;
    }
    return result;
}
void HuffmanDecompress(char decomp_file[])  {
    string file_name(decomp_file);
    file_name = file_name.substr(0, file_name.size() - 4);
//    unzipFiles(file_name);
    ifstream InHuffmanFile((file_name + ".huftable").c_str());
    ifstream InObjectFile((file_name + ".hufcomp").c_str(), ios::binary);
    ofstream OutAsciiFile("OriginalFile");
    int ObjSize = getFileSize(InObjectFile);
    cout << "Object file size : " << ObjSize << endl;
    map<unsigned char, unsigned int> HuffmanMapTable;
    map<unsigned char, string> table;
    priority_queue<huff_node> PQ;
    unsigned char ch; unsigned int ui; unsigned char whitespace;
    while(InHuffmanFile >> noskipws >> ch >> whitespace >> ui >> whitespace) {
        HuffmanMapTable[ch] = ui;
        cout << ch << " " << ui << endl;
    }
    unsigned char *buf = new unsigned char[ObjSize];
    InObjectFile.read((char *)buf, ObjSize); InObjectFile.close();
    unsigned int x = 0;
    string BitString = "";

    for(int i = 0 ; i < ObjSize ; ++i) {
        x |= buf[i];
        BitString += getBitString(buf[i]);
        x <<= 8;
    }
    cout << "BitString >" << BitString << endl;
    string symStr;
    BuildHuffmanTree(HuffmanMapTable, PQ, false);
    BuildHuffmanSymbolEncoding(&root, symStr, table);
    for(const auto &w : table) {
        cout << w.first << " " << w.second << endl;
    }

    OutAsciiFile << DecodeHuffmanCode(BitString);
    delete [] buf;

}
int main(int argc, char *argv[]) {
    what_to_process(argc, argv);
    return 0;
}
