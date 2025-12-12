#include <regex>
#include <string>
#include <fstream>
#include <iostream>

//splits given text on delimitter, returning strings before and after
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

//parses input text to pairs of range data
std::vector<std::vector<std::string>> getRanges (std::string& s) {
    std::vector<std::string> split {splitText(s, ",")};
    std::vector<std::vector<std::string>> fullyParsed;
    for (int i{0}; i < split.size(); i++) {
        fullyParsed.push_back(splitText(split[i], "-"));
    }
    return fullyParsed;
}

//checks if input ID is valid
//invalid is repeating string of #s: 4949, 1111, 123123, 323232...
bool checkInvalid(long long n) {
    std::string s{ std::to_string(n) };
    std::regex re(R"(^(\d+)\1+$)"); //fix regex
    std::cmatch match; //adjust length max
    return std::regex_match(s, re);
}

//takes ranges of ID and sums the invalid IDs in those ranges
long long getInvalidIdSum(std::vector<std::string> numStrings) {
    long long lowBound{ stoll(numStrings[0]) };
    long long highBound{ stoll(numStrings[1]) };
    long long sum{0};

    std::cout << lowBound << " to " << highBound << '\n';

    for (long long i{lowBound}; i < highBound; i++) {
        if (checkInvalid(i)) {//and check for dupes
            std::cout << i << " ";
            sum += i;
        }
    }
    std::cout << '\n';
    return sum;
}

//reads from file, parses num ranges, and sums invalid IDs in those ranges
int main() {
    std::ifstream inf{ "invalidRanges.txt" };

    if (!inf) {
        std::cerr << "Error opening invalidRanges.txt!\n";
        return 1;
    }

    std::string rawInvalidIdText;
    inf >> rawInvalidIdText;
    std::vector<std::vector<std::string>> idRanges {getRanges(rawInvalidIdText)};

    long long invalidIdCount{0};
    for (int i{0}; i < idRanges.size(); i++) {
        invalidIdCount += getInvalidIdSum(idRanges[i]);
    }
    std::cout << "invalids: " << invalidIdCount << "\n";

    return 0;
}