#include "Huffman.hpp"
#include <iostream>
#include <fstream>
#include <bitset>
#include <unordered_map>
#include <vector>

using namespace std;

int compress_file(const string& inputPath, const string& outputPath) {
    ifstream input(inputPath, ios::binary);
    if (!input.is_open()) {
        cerr << "[!] Error: Could not open input file: " << inputPath << endl;
        return -1;
    }

    string fileData((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    if (fileData.empty()) {
        cerr << "[!] Error: Input file is empty.\n";
        return -2;
    }

    Huffman huffman;
    huffman.build_tree(fileData);
    huffman.generate_codes();
    string encodedData = huffman.encode(fileData);

    int paddingBits = (8 - (encodedData.size() % 8)) % 8;
    encodedData.append(paddingBits, '0');

    vector<unsigned char> byteBuffer;
    for (size_t i = 0; i < encodedData.size(); i += 8) {
        bitset<8> bits(encodedData.substr(i, 8));
        byteBuffer.push_back(static_cast<unsigned char>(bits.to_ulong()));
    }

    ofstream output(outputPath, ios::binary);
    if (!output.is_open()) {
        cerr << "[!] Error: Could not create output file: " << outputPath << endl;
        return -3;
    }

    unordered_map<unsigned char, int> frequencies;
    for (char ch : fileData)
        frequencies[ch]++;

    unsigned char symbolCount = static_cast<unsigned char>(frequencies.size());
    output.write(reinterpret_cast<const char*>(&symbolCount), sizeof(unsigned char));

    for (auto& pair : frequencies) {
        // output.write(&pair.first, sizeof(char));
        output.write(reinterpret_cast<const char*>(&pair.first), sizeof(unsigned char));  // ✅ correct

        int freq = pair.second;
        output.write(reinterpret_cast<char*>(&freq), sizeof(int));
    }

    output.write(reinterpret_cast<char*>(&paddingBits), sizeof(char));
    output.write(reinterpret_cast<char*>(byteBuffer.data()), byteBuffer.size());
    output.close();

    cout << "[+] Compression complete.\n";
    return 0;
}
