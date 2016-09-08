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
unsigned long long get_file_size(const std::string& address);

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
    unsigned write_to_binary(std::ifstream&, std::ofstream &);
    void write_freq_table(std::ofstream&, unsigned);
    std::string parse_bitstr(std::string&);
    unsigned parse_freq_table(std::ifstream&);
    void build_bit_string(char *, unsigned, std::string &, unsigned);
public:
    void encode_file(const char *, const char *);
    void decode_file(const char *, const char *);
    double compare_rate(const std::string &, const std::string &);
public:
// just concat 3 strings of path, input, postfix
    inline std::string format_path_name(const char *path, const char *input, const char *postfix) {
        return (std::string(path).append(input).append(postfix));
    }
    void print_huffman_table(std::ostream &);
    void print_enocde_table(std::ostream &);
private:
    const char *loc_text = "texts/";
    const char* path_freq = "./frequency-table/";
    const char* path_encoded = "./encoded-files/";
    const char* path_decoded = "./decoded-files/";
    const char *postfix_tab =".tab";
    const char *postfix_huf =".huf";
    FreqTable fTab;
    EncodeTable eTab;
    HuffmanNode *root;
};

