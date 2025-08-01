#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int compress_file(const string& inputPath, const string& outputPath);
int decompress_file(const string& inputPath, const string& outputPath);

void show_usage() {
    cout << "Usage:\n"
    << "  file_compressor.exe compress <input_file> <output_file>\n"
    << "  file_compressor.exe decompress <input_file> <output_file>\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "[!] Invalid number of arguments.\n";
        show_usage();
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    ifstream infile(inputFile, ios::binary);
    if (!infile.good()) {
        cerr << "[!] Error: Input file does not exist: " << inputFile << endl;
        return 2;
    }

    int status = 0;

    if (mode == "compress") {
        cout << "==> Compressing " << inputFile << " to " << outputFile << "\n";
        status = compress_file(inputFile, outputFile);
    } else if (mode == "decompress") {
        cout << "==> Decompressing " << inputFile << " to " << outputFile << "\n";
        status = decompress_file(inputFile, outputFile);
    } else {
        cout << "[!] Invalid mode: " << mode << "\n";
        show_usage();
        return 3;
    }

    if (status != 0) {
        std::cout << "[x] Operation failed with status: " << status << "\n";
        return status;
    }

    std::cout << "[\u2713] Operation completed successfully.\n";
    return 0;
}
