#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void printCoordinates(const std::vector<std::pair<int, int>>& coordinates) {
    for (const auto& coord : coordinates) {
        std::cout << "{" << coord.first << " * 32, " << coord.second << " * 32},\n";
    }
}

int main() {
    std::string filename = "D:/input.txt";


    std::vector<std::pair<int, int>> starCoordinates;
    std::vector<std::pair<int, int>> dotCoordinates;
    std::vector<std::pair<int, int>> atCoordinates;

    int row = 0;
    int col = 0;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не вдалося відкрити файл " << filename << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        col = 0;
        for (char c : line) {
            if (c == '*') {
                starCoordinates.push_back(std::make_pair(col, row));
            } else if (c == '.') {
                dotCoordinates.push_back(std::make_pair(col, row));
            }
            else if (c == '@') {
                atCoordinates.push_back(std::make_pair(col, row));
            }
            col++;
        }
        row++;
    }

    file.close();

    if(starCoordinates.size() != dotCoordinates.size() )
    {
        std::cout << "\nPARSING ERROR\n";
        return 1;
    }

    std::cout << "\nNum of box:\n" << starCoordinates.size() <<std::endl;

    std::cout << "\nSokoban\n";
    printCoordinates(atCoordinates);

    std::cout << "\n\nconst uint16_t POINT_POS_[][2] = { \n";
    printCoordinates(dotCoordinates);
    std::cout << "};";

    std::cout << "\n\nconst uint16_t BOX_POS_[][2] = { \n";
    printCoordinates(starCoordinates);
    std::cout << "};\n\n";

    return 0;
}
