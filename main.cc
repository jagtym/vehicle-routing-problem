#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unistd.h>

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

double get_distance(int x1, int y1, int x2, int y2) {
    return sqrt((double) pow(x1 - x2, 2) + (double) pow(y1 - y2, 2));
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

    float matrix[customers.size()][customers.size()];
    for (int y = 0; y < customers.size(); y++) {
        for (int x = 0; x < customers.size(); x++) {
            int first_customer_x = customers[x].x_cord; 
            int first_customer_y = customers[x].y_cord;
            int second_customer_x = customers[y].x_cord;
            int second_customer_y = customers[y].y_cord; 
            matrix[y][x] = get_distance(first_customer_x, first_customer_y, second_customer_x, second_customer_y);
        }
    }

    const int MIN_TIME = 9999999;

    int visited_customers = 0;
    double act_time = 0;
    int act_capacity = capacity;
    int act_location = 0;
    double minimum_time = MIN_TIME;
    int next_location = 0;
    int trucks = 0;
    double distance = 0;
    customers[0].is_served = true;

    while (visited_customers < customers.size() - 1) {
        bool visited_someone = false;
        for (int i = 0; i < customers.size(); i++) {    
            if (i == act_location) {
                continue;
            }
            if (customers[i].is_served == true) {
                // cout << "Already served that customer!" << i << endl;
                continue;
            }
            // demand too big for actual capacity
            if (act_capacity < customers[i].demand) {
                // cout << "Capacity too big!" << endl;
                continue;
            }
            // it's too late to take package 
            if (act_time + matrix[act_location][i] > customers[i].due_date) {
                // cout << "Time exceeded!" << endl;
                continue;
            }
            // searching for best customer
            if (customers[i].ready_time - matrix[act_location][i] + act_time < minimum_time) {
                next_location = i;
                minimum_time = customers[i].ready_time - matrix[act_location][i] + act_time;
                visited_someone = true;
            }
        }
        if (visited_someone) {
            visited_customers++;
            act_time += matrix[act_location][next_location];
            if (act_time < customers[next_location].ready_time) act_time = customers[next_location].ready_time;
            act_time += customers[next_location].service_time;
            act_capacity -= customers[next_location].demand;
            customers[next_location].is_served = true;
            cout << next_location << " ";
            act_location = next_location;
            minimum_time = MIN_TIME;
        }
        else if (act_time == 0)
        {
            trucks = -1;
            break;
        }
        else {
            cout << endl << "Next truck" << endl;
            trucks++;
            distance += act_time;
            distance += matrix[act_location][0];
            act_time = 0;
            act_capacity = capacity;
            next_location = 0;
            minimum_time = MIN_TIME;
        }
    }
    trucks++;
    distance += act_time;
    distance += matrix[act_location][0];
    cout << endl << "Trucks: " << trucks << endl;
    cout << "Distance: " << distance << endl;
}