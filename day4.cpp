#include <string>
#include <fstream>
#include <iostream>

void addPaperRow(std::vector<std::vector<char>>& v, std::string& s) {
    std::vector<char> rolls{};
    for (int i{0}; i<s.length(); i++) {
        rolls.push_back(s[i]);
    }
    v.push_back(rolls);
    return;
}

//prints the data in a 2D table of chars
void printTable(std::vector<std::vector<char>>& v) {
    for (int i{0}; i<v.size(); i++) {
        std::cout << i;
        if (i < 10)
            std::cout << " ";
        if (i < 100)
            std::cout << " ";
        for (int j{0}; j<v[i].size(); j++) {
            std::cout << v[i][j];
        }
        std::cout << '\n';
    }
}



bool checkRoll(std::vector<std::vector<char>>& v, int i, int j) {
    size_t maxI{v.size() - 1};
    size_t maxJ{v[0].size() - 1};
    int filledCount{0};
    if (i != maxI && j != maxJ && v[i+1][j+1]=='@')
        filledCount += 1;
    if (i != 0 && j != 0 && v[i-1][j-1]=='@')
        filledCount += 1;
    if (i != maxI && j != 0 && v[i+1][j-1]=='@')
        filledCount += 1;
    if (i != 0 && j != maxJ && v[i-1][j+1]=='@')
        filledCount += 1;
    if (j != maxJ && v[i][j+1]=='@')
        filledCount += 1;
    if (j != 0 && v[i][j-1]=='@')
        filledCount += 1;
    if (i != maxI && v[i+1][j]=='@')
        filledCount += 1;
    if (i != 0 && v[i-1][j]=='@')
        filledCount += 1;

    if (filledCount >= 4) {
        return false;
    }
    // std::cout << i << "," << j << "   \n";
    return true;
}

int countAvailableRolls(std::vector<std::vector<char>>& v) {
    int availableRollCount{0};
    for (int i{0}; i < v.size(); i++) {
        for (int j{0}; j < v[i].size(); j++) {
            if (v[i][j] == '@' && checkRoll(v, i, j)) {
                v[i][j] = 'x';
                availableRollCount += 1;
            }
        }
    }
    return availableRollCount;
}

int main() {
    std::ifstream inf{ "printingRolls.txt" };
    if (!inf) {
        std::cerr << "unable to open printingRolls.txt";
    }

    //creates 2D table of paper rolls
    std::string rawTextLine{};
    std::vector<std::vector<char>> table{};
    while (inf >> rawTextLine) {
        addPaperRow(table, rawTextLine);
    }

    int count{countAvailableRolls(table)};
    int total{0};
    while (count > 0) {
        total += count;
        count = countAvailableRolls(table);
    }

    printTable(table);
    std::cout << "total count: " << total << '\n';
}