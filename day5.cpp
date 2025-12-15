#include <string>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream inf{ "foodInventory.txt " };
    if (!inf) {
        std::cerr << "unable to open foodInventory.txt";
    }

    std::string rawText;
    while (inf >> rawText) {
        while (rawText != "") {
            std::cout << rawText << '\n';
        }
        
    }
}