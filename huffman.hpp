
#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <string>
#include <unordered_map>
#include <queue>

// Struct to represent Huffman tree node
struct HuffmanNode {
    unsigned char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char data, int freq);
    ~HuffmanNode();
};

// Comparator for min-heap
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b);
};

// Core Huffman class
class Huffman {
public:
    Huffman();
    ~Huffman();

    void build_tree(const std::string& input);
    void build_tree_from_frequency(const std::unordered_map<unsigned char, int>& freq);
    void generate_codes();
    std::string encode(const std::string& input);
    std::string decode(const std::string& binaryData);

    std::unordered_map<unsigned char, std::string> get_code_table() const;
    HuffmanNode* get_root() const;

private:
    HuffmanNode* root;
    std::unordered_map<unsigned char, int> freq_table;
    std::unordered_map<unsigned char, std::string> code_table;

    void generate_codes_helper(HuffmanNode* node, const std::string& code);
    void free_tree(HuffmanNode* node);
};

#endif
