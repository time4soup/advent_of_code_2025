#include <string>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream inf{"joltageBanks.txt"};
    if (!inf) {
        std::cerr << "Unable to open joltageBanks.txt\n";
    }
    std::string rawText;
    while (inf >> rawText) {
        std::cout << rawText << '\n';
    }
}