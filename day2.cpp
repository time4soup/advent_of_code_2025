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

void getInvalidIdCount(std::string rangeText) {
    std::vector<std::string> endpoints {splitText(rangeText, "-")};
    std::cout << endpoints[0] << " to " << endpoints[1] << "\n";

    return;
}
//countInvalids()
//parse text to ints
//check for even length of int strings
////invalid if both odd
//string version of ints and int version
//assert length of possible invalids
//find fixed digits / valid ranges
//// mirror fixed digits/ranges

int main() {
    std::ifstream inf{ "invalidRanges.txt" };

    if (!inf) {
        std::cerr << "Error opening invalidRanges.txt!\n";
        return 1;
    }

    std::string rawInvalidIdText;
    inf >> rawInvalidIdText;
    std::vector<std::string> idRanges {splitText(rawInvalidIdText, ",")};

    int invalidIdCount{0};
    for (int i; i < idRanges.size(); i++) {
        getInvalidIdCount(idRanges[i]);
    }
    std::cout << invalidIdCount << "\n";

    return 0;
}
//dont count dupes
//invalid must be even length string
//use regex?