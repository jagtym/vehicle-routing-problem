#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#include "customer.h"

void load_file_contents(ifstream &file, vector<string> &file_content) {
    string line;
    while(file) {
        getline(file, line);
        file_content.push_back(line);
    }
}

void populate_customer_array(vector<string> &file_content, vector<Customer> &customers) {
    const int first_customer_line = 9;
    
    for (int i = first_customer_line; i < file_content.size() - 2; i++) {
        int index;
        int x_cord;
        int y_cord;
        int demand;
        int ready_time;
        int due_date;
        int service_time;

        istringstream(file_content[i]) >> index >> x_cord >> y_cord >> demand >> ready_time >> due_date >> service_time;
        Customer customer = Customer(index, x_cord, y_cord, demand, ready_time, due_date, service_time); 
        customers.push_back(customer);
    }
}

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

    vector<string> file_content;
    load_file_contents(input_file, file_content);

    problem_name = file_content[0]; 
    istringstream(file_content[4]) >> vehicle_number >> capacity;
    cout << vehicle_number << " " << capacity << endl;

    vector<Customer> customers;
    populate_customer_array(file_content, customers);

    for_each(customers.begin(), customers.end(), [](Customer customer) {
        customer.print();
    });


}