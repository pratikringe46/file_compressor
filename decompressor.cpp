#include "Huffman.hpp"
#include <iostream>
#include <fstream>
#include <bitset>
#include <unordered_map>
#include <vector>

using namespace std;

int decompress_file(const string& inputPath, const string& outputPath) {
    ifstream input(inputPath, ios::binary);
    if (!input.is_open()) {
        cerr << "Error: Could not open compressed file: " << inputPath << endl;
        return -1;
    }

    unsigned char symbol_cnt;    //count of the symbols
    input.read(reinterpret_cast<char*>(&symbol_cnt), sizeof(unsigned char));

    unordered_map<unsigned char, int> freqTable;
    for (int i = 0; i < symbol_cnt; ++i) {
       unsigned char ch;
        int freq;
        input.read(reinterpret_cast<char*>(&ch), sizeof(unsigned char));
        input.read(reinterpret_cast<char*>(&freq), sizeof(int));
        freqTable[ch] = freq;
    }

    char padding_bits_char;
    input.read(&padding_bits_char, sizeof(char));
    int paddingBits = static_cast<int>(padding_bits_char);

    vector<unsigned char> byteBuffer((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    string bitstream;
    for (unsigned char byte : byteBuffer) {
        bitset<8> bits(byte);
        bitstream += bits.to_string();
    }

    if (paddingBits > 0 && paddingBits <= 8) {
        bitstream = bitstream.substr(0, bitstream.size() - paddingBits);
    }

    Huffman huffman;
    huffman.build_tree_from_frequency(freqTable);
    huffman.generate_codes();

    string decodedText = huffman.decode(bitstream);

    ofstream output(outputPath, ios::binary);
    if (!output.is_open()) {
        cerr << "Error: Could not open output file: " << outputPath << endl;
        return -2;
    }

    output.write(decodedText.data(), decodedText.size());
    output.close();

    cout << "Decompression complete.\n";
    return 0;
}
