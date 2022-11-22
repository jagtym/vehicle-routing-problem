struct Customer
{
    int index;
    int x_cord;
    int y_cord;
    int demand;
    int ready_time;
    int due_date;
    int service_time;

    bool is_served;

    Customer(int i, int x, int y, int d, int rt, int dd, int st) : index(i),
                                                                   x_cord(x),
                                                                   y_cord(y),
                                                                   demand(d),
                                                                   ready_time(rt),
                                                                   due_date(dd),
                                                                   service_time(st),
                                                                   is_served(false)
    {
    }

    void print()
    {
        cout << index << " " << x_cord << " " << y_cord << " " << demand << " " << ready_time << " " << due_date << " " << service_time << endl;
    }
};

struct Results {
    vector<vector<int>> answer_matrix;
    int trucks;
    double distance;
    vector<vector<int>> answer_m;

    Results(int trucks, double distance, vector<vector<int>> &answer_matrix) : trucks(trucks),
                                                                               distance(distance)
    {

        for (int j = 0; j < answer_matrix.size(); j++) {
            vector<int> empty(answer_matrix.size(), 0); 
            answer_m.push_back(empty);
        }

        for (int j = 0; j < answer_matrix.size(); j++) {
            for (int i = 0; i < answer_matrix.size(); i++) {
                // cout << i << " " << j << ": " << answer_matrix[i][j] << endl;
                answer_m[i][j] = answer_matrix[i][j];
            }
        }
    }
};