#include <string>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<int> parseText(const std::string& s) {
    return {};
}

int main() {
    std::ifstream inf("junctionBoxes.txt");
    if (!inf) {
        std::cerr << "unable to open junctionBoxes.txt";
        return 1;
    }

    std::string rawText{};
    std::vector<std::vector<int>> boxLocs{}; //replace with class?
    while (inf >> rawText) {
        boxLocs.push_back(parseText(rawText));
    }
}