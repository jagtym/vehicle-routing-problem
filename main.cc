#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "usage: " << argv[0] << " [dataset_file]" << endl;
    }

    const string filename = argv[1];
    ifstream input_file;
    input_file.open(filename);

    if (!input_file.is_open()) {
        cerr << "File not found!" << endl;
        return 1;
    }
}