
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cmath>

using namespace std;

// Function to read CSV dynamically
vector<vector<string>> read_csv(const string &filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return {};
    }

    string line;
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string value;

        while (getline(ss, value, ',')) {
            row.push_back(value);
        }

        data.push_back(row);
    }

    return data;
}

// Function to analyze all columns dynamically
void analyze_all_columns(const vector<vector<string>> &data, bool has_header = true) {
    if (data.empty()) {
        cout << "No data to analyze!" << endl;
        return;
    }

    int rows = data.size();
    int cols = data[0].size();
    int start_row = has_header ? 1 : 0; // Skip header if present

    cout << "Analyzing all columns:" << endl;

    for (int col = 0; col < cols; ++col) {
        double sum = 0.0, mean = 0.0, std_dev = 0.0;
        int numeric_count = 0;
        unordered_map<string, int> freq_map;
        vector<double> numeric_values;

        for (int i = start_row; i < rows; ++i) { // Use `start_row` to handle headers
            if (col >= data[i].size()) continue;
            string value = data[i][col];

            try {
                double num_value = stod(value);
                numeric_values.push_back(num_value);
                sum += num_value;
                numeric_count++;
            } catch (const invalid_argument &) {
                freq_map[value]++;
            }
        }

        if (!numeric_values.empty()) {
            mean = sum / numeric_count;
            for (double num : numeric_values) {
                std_dev += pow(num - mean, 2);
            }
            std_dev = sqrt(std_dev / numeric_count);

            cout << "Column " << col + 1 << " (Numeric): Sum = " << sum
                 << ", Average = " << mean
                 << ", Standard Deviation = " << std_dev << endl;
        } else {
            int max_count = 0;
            string mode_val;

            for (const auto &entry : freq_map) {
                if (entry.second > max_count) {
                    max_count = entry.second;
                    mode_val = entry.first;
                }
            }

            cout << "Column " << col + 1 << " (Non-numeric): Mode = " << mode_val
                 << " (Frequency = " << max_count << ")" << endl;
        }
    }
}

int main() {
    string filename = "IPL_Matches.csv"; 
    vector<vector<string>> data = read_csv(filename);

    cout << "Total rows read: " << data.size() << endl;
    if (!data.empty()) {
        cout << "Columns in first row: " << data[0].size() << endl;
    }

    analyze_all_columns(data, true); // Set to `false` if there's no header
    return 0;
}
