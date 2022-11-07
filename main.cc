#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

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

    string problem_name;
    int vehicle_number;
    int capacity;

    string line;
    vector<string> file_content;
    while(input_file) {
        getline(input_file, line);
        file_content.push_back(line);
    }

    problem_name = file_content[0]; 

    istringstream(file_content[4]) >> vehicle_number >> capacity;
    cout << vehicle_number << " " << capacity << endl;

    const int first_customer_line = 7;
    for_each(file_content.begin() + first_customer_line, file_content.end(), [](string line){
        cout << line << endl;
    });
}