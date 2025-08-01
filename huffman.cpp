
#include "Huffman.hpp"
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

HuffmanNode::HuffmanNode(unsigned char data, int freq)
    : data(data), freq(freq), left(nullptr), right(nullptr) {}

HuffmanNode::~HuffmanNode() {
    delete left;
    delete right;
}

bool Compare::operator()(HuffmanNode* a, HuffmanNode* b) {
    return a->freq > b->freq;
}

Huffman::Huffman() : root(nullptr) {}

Huffman::~Huffman() {
    free_tree(root);
}

void Huffman::free_tree(HuffmanNode* node) {
    if (node) {
        free_tree(node->left);
        free_tree(node->right);
        delete node;
    }
}

void Huffman::build_tree(const string& input) {
    for (unsigned char ch : input) {
        freq_table[ch]++;
    }
    build_tree_from_frequency(freq_table);
}

void Huffman::build_tree_from_frequency(const unordered_map<unsigned char, int>& freq) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    for (auto& pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    if (pq.size() == 1) {
        HuffmanNode* only = pq.top(); pq.pop();
        root = new HuffmanNode('\0', only->freq);
        root->left = only;
        return;
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* internal = new HuffmanNode('\0', left->freq + right->freq);
        internal->left = left;
        internal->right = right;
        pq.push(internal);
    }

    root = pq.top();
}

void Huffman::generate_codes() {
    if (root) generate_codes_helper(root, "");
}

void Huffman::generate_codes_helper(HuffmanNode* node, const string& code) {
    if (!node) return;
    if (!node->left && !node->right) {
        code_table[node->data] = code.empty() ? "0" : code;
        return;
    }
    generate_codes_helper(node->left, code + "0");
    generate_codes_helper(node->right, code + "1");
}

string Huffman::encode(const string& input) {
    string encoded;
    for (unsigned char ch : input) {
        encoded += code_table[ch];
    }
    return encoded;
}

string Huffman::decode(const string& binaryData) {
    string decoded;
    HuffmanNode* current = root;
    for (char bit : binaryData) {
        if (bit == '0') current = current->left;
        else current = current->right;

        if (!current->left && !current->right) {
            decoded += current->data;
            current = root;
        }
    }
    return decoded;
}

unordered_map<unsigned char, string> Huffman::get_code_table() const {
    return code_table;
}

HuffmanNode* Huffman::get_root() const {
    return root;
}
