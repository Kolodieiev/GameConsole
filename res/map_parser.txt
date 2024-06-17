#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int transform_char(char c) {
    if (c == 'X') {
        return 2;
    } else {
        return 1;
    }
}

int main() {
    string filename = "D:/input.txt";

    ifstream input_file(filename);

    if (!input_file.is_open()) {
        cerr << "Не вдалося відкрити файл: " << filename << endl;
        return 1;
    }

    vector<string> lines;
    string line;

    while (getline(input_file, line)) {
        lines.push_back(line);
    }

    input_file.close();

    vector<vector<int>> output_matrix;

    for (const string& line : lines) {
        vector<int> transformed_line;
        for (char c : line) {
            transformed_line.push_back(transform_char(c));
        }
        output_matrix.push_back(transformed_line);
    }

    for (const vector<int>& row : output_matrix) {
        for (size_t i = 0; i < row.size(); ++i) {
            cout << row[i];
            cout << ",";
        }
        cout << endl;
    }

    return 0;
}
