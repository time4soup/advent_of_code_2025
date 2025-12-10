#include <regex>
#include <string>
#include <fstream>
#include <iostream>

std::vector<std::string> splitText(const std::string& text, const std::string& delimitter) {
    std::vector<std::string> slices;
    std::string::size_type foundN;
    std::string::size_type searchN{0};

    foundN = text.find(delimitter, searchN);
    while (foundN != std::string::npos) {
        slices.push_back(text.substr(searchN, foundN - searchN));
        searchN = foundN + delimitter.length();
        foundN = text.find(delimitter, searchN);
    }
    slices.push_back(text.substr(searchN, text.length() - searchN));

    return slices;
}

int main() {
    std::ifstream inf{ "invalidRanges.txt" };

    if (!inf) {
        std::cerr << "Error opening invalidRanges.txt!\n";
        return 1;
    }

    std::string rawInvalidIdText;
    inf >> rawInvalidIdText;
    std::vector<std::string> idRanges {splitText(rawInvalidIdText, ",")};

    long long invalidIdCount{0};
    for (int i{0}; i < idRanges.size(); i++) {
        invalidIdCount += getInvalidIdCount(idRanges[i]);
    }
    std::cout << "invalids: " << invalidIdCount << "\n";

    return 0;
}
//dont include dupes