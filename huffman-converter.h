#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <queue>
#include <string>
#include <bitset>

struct HuffmanNode {
    char symbol;
    unsigned freq;
    HuffmanNode *left;
    HuffmanNode *right;
    HuffmanNode(unsigned char c, unsigned f);
    HuffmanNode(const HuffmanNode& hn);
    HuffmanNode(int f, HuffmanNode *l, HuffmanNode *r);
};
bool operator< (const HuffmanNode &lhs, const HuffmanNode &rhs);
typedef std::map<unsigned char, unsigned int> FreqTable;
typedef std::map<unsigned char, std::string> EncodeTable;
typedef std::map<unsigned char, std::string> invEncodeTable;
typedef std::priority_queue<HuffmanNode> priQ;
typedef unsigned long long Bytes;

const char *loc_text = "texts/";
const char* path_freq = "./frequency-table/";
const char* path_encoded = "./encoded-files/";
const char *postfix_tab =".tab";
const char *postfix_huf =".huf";

unsigned long long get_file_size(std::ifstream &is);
class HuffmanConverter {
public:
    HuffmanConverter() {}
    HuffmanConverter(std::ifstream &);
    HuffmanConverter(std::string &);
    HuffmanConverter(const char *);
    ~HuffmanConverter() {}
private:
    Bytes build_freq_table(std::ifstream &);
    HuffmanNode* build_prefix_tree();
    void encode_symbol();
    void encode_symbol_util(HuffmanNode *, EncodeTable &, std::string);
    void write_to_binary(std::ifstream&, std::ofstream &);
    void write_freq_table(std::ofstream &);
public:
    void encode_file(const char *, const char *);
    void decode_file(const char *, const char *);
    double compare_rate(std::ifstream &, std::ifstream &);
public:
    void print_huffman_table(std::ostream &);
    void print_enocde_table(std::ostream &);
private:
    FreqTable fTab;
    EncodeTable eTab;
    HuffmanNode *root;
};

