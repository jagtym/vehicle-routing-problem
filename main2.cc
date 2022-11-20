#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unistd.h>
#include <iomanip>

using namespace std;

#include "customer.h"

void load_file_contents(ifstream &file, vector<string> &file_content) {
    string line;
    while(file >> ws && getline(file, line)) {
        file_content.push_back(line);
    }
}

void populate_customer_array(vector<string> &file_content, vector<Customer> &customers) {
    const int first_customer_line = 6;
    
    for (int i = first_customer_line; i < file_content.size(); i++) { 
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

float get_distance(int x1, int y1, int x2, int y2) {
    return sqrt((double) pow(x1 - x2, 2) + (double) pow(y1 - y2, 2));
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "usage: " << argv[0] << " [dataset_file]" << endl;
    }

    const string input_filename = argv[1];
    const string output_filename = argv[2];
    ifstream input_file;
    ofstream output_file;
    input_file.open(input_filename);
    output_file.open(output_filename);

    if (!input_file.is_open() || !output_file.is_open()) {
        cerr << "File not found!" << endl;
        return 1;
    }

    string problem_name;
    int vehicle_number;
    int capacity;

    vector<string> file_content;
    load_file_contents(input_file, file_content);

    problem_name = file_content[0];
    istringstream(file_content[3]) >> vehicle_number >> capacity;
    cout << "Vehicle number: " << vehicle_number << " Capacity: " << capacity << endl;

    vector<Customer> customers;
    populate_customer_array(file_content, customers);

    for_each(customers.begin(), customers.end(), [](Customer customer) {
        customer.print();
    });

    vector<vector<float>> matrix(customers.size(), vector<float> (customers.size(), 0));
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
    const int NUMBER_OF_MINS = 3;

    int visited_customers = 0;
    double act_time = 0;
    int act_capacity = capacity;
    int act_location = 0;
    float minimum_time = MIN_TIME;
    int next_location = 0;
    int trucks = 0;
    double distance = 0;
    int vertices = 0;
    int number_of_minimums = 0;
    customers[0].is_served = true;
    vector<vector<int>> answer_matrix(customers.size(), vector<int> (customers.size(), 0));

    cout << "First truck" << endl;
    while (visited_customers < customers.size() - 1) {
        bool visited_someone = false;
        vector<float> min_times_vector;
        vector<float> min_locations_vector;
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
            // won't come back to depot on time
            if (act_time + matrix[act_location][i] + customers[i].service_time + matrix[i][0] > customers[0].due_date
                || customers[i].ready_time + customers[i].service_time + matrix[i][0] > customers[0].due_date) {
                // cout << "Won't come back!" << endl;
                continue;
            }
            // searching for best customer
            // if (customers[i].ready_time - matrix[act_location][i] + act_time < minimum_time) {
            //     next_location = i;
            //     minimum_time = customers[i].ready_time - matrix[act_location][i] + act_time;
            //     visited_someone = true;
            // }
            bool inserted = false;
            int j = 0;
            minimum_time = customers[i].ready_time - matrix[act_location][i] + act_time;
            while(!inserted) {
                if (min_times_vector.size() == j) {
                    min_times_vector.push_back(minimum_time);
                    min_locations_vector.push_back(i);
                    inserted = true;
                }
                else {
                    if (minimum_time < min_times_vector[j]) {
                        min_times_vector.insert(min_times_vector.begin() + j, minimum_time);
                        min_locations_vector.insert(min_locations_vector.begin() + j, i);
                        inserted = true;
                    }
                    else j++;
                }
            }
        }
        if (min_times_vector.size() != 0) {
            srand(time(NULL));
            int random = 0;
            if (min_times_vector.size() < NUMBER_OF_MINS) random = rand() % min_times_vector.size();
            else random = rand() % NUMBER_OF_MINS;
            next_location = min_locations_vector[random];
            visited_someone = true;
        }
        if (visited_someone) {
            visited_customers++;
            act_time += matrix[act_location][next_location];
            if (act_time < customers[next_location].ready_time) act_time = customers[next_location].ready_time;
            act_time += customers[next_location].service_time;
            act_capacity -= customers[next_location].demand;
            customers[next_location].is_served = true;
            cout << next_location << " ";
            answer_matrix[trucks][vertices] = next_location;
            vertices++;
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
            vertices = 0;
            distance += act_time;
            distance += matrix[act_location][0];
            act_time = 0;
            act_capacity = capacity;
            act_location = 0;
            next_location = 0;
            minimum_time = MIN_TIME;
        }
    }
    trucks++;
    distance += act_time;
    distance += matrix[act_location][0];
    if (trucks == 0) {
        trucks = -1; //task requirements
        distance = 0;
    }
    cout << endl << "Trucks: " << trucks << endl;
    cout << "Distance: " << fixed << setprecision(5) << distance << endl;
    output_file << trucks << " " << fixed << setprecision(5) << distance << endl;
    for (int i = 0; i < trucks; i++) {
        int j = 0;
        while (answer_matrix[i][j] != 0) {
            output_file << answer_matrix[i][j] << " ";
            j++;
        }
        output_file << endl;
    }
    input_file.close();
    output_file.close();
}