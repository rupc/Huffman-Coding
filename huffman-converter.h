#include <map>
#include <queue>
#include <iostream>
#include <string>

struct HuffmanNode {
    char symbol;
    unsigned freq;
    HuffmanNode *left;
    HuffmanNode *right;
    HuffmanNode(unsigned char c, unsigned f);
    HuffmanNode(const HuffmanNode& hn);
    HuffmanNode(int f, HuffmanNode *l, HuffmanNode *r);
};
//bool operator< (const HuffmanNode &lhs, const HuffmanNode &rhs);
typedef std::map<unsigned char, unsigned int> HuffmanTable;
typedef std::priority_queue<HuffmanNode> PQ;

class HuffmanConverter {
public:
    HuffmanConverter() {}
    HuffmanConverter(std::ifstream &);
    HuffmanConverter(std::string &);
    HuffmanConverter(const char *);
    ~HuffmanConverter() {}
    void huffman_build_table(std::ifstream &);
    void print_huffman_table(std::ostream &);
    HuffmanNode* huffman_build_tree();
    void huffman_encode();
    void huffman_decode();
private:
    HuffmanTable table;
    HuffmanNode *root;
};
