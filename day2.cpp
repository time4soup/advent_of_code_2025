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

void pushBackRange(std::vector<int>& v, int start, int end) {
    for (int i{start}; i < end + 1; i++) {
        v.push_back(i);
    }
}

std::vector<int> helperCounter(std::vector<std::vector<int>>& v, int i) {
    std::vector<int> nums;
    if (i >= v.size() / 2) {
        nums = v[i-1];
        return nums;
    } else {
        std::vector<int> numEndings {helperCounter(v, i+1)};
        for (int j{0}; j < v[i-1].size(); j++) {
            for (int k{0}; k < numEndings.size(); k++) {
                nums.push_back(
                    stoi(
                        std::to_string(v[i-1][j]) + 
                        std::to_string(numEndings[k])
                    )
                );
            }
        }
        return nums;
    }
    //stop when i = 0 so don't index v[-1]
}
//cycle thru first half of vectors
//check invalidity
//rolling count
// swap to all possibles instead of locked #s
// recursive func to cycle through all possibles

void getInvalidIdCount(std::string rangeText) {
    std::vector<std::string> endpointStrings {splitText(rangeText, "-")};
    std::vector<long long> endpointInts {
        std::stoll(endpointStrings[0]), 
        std::stoll(endpointStrings[1])
    };
    
    //find possible range of even lengthed nums
    std::vector<long long> possibleRange (2);
    if (endpointStrings[0].length()%2 != 0 && endpointStrings[1].length()%2 != 0) { //odd-odd is invalid
        return;
    } else if (endpointStrings[0].length()%2 != 0 && endpointStrings[1].length()%2 == 0) { //odd-even
        possibleRange[0] = std::pow(10, endpointStrings[0].length());
        possibleRange[1] = endpointInts[1];
        endpointStrings[0] = "0" + endpointStrings[0];
    } else if (endpointStrings[0].length()%2 == 0 && endpointStrings[1].length()%2 != 0) { //even-odd
        possibleRange[0] = endpointInts[0];
        possibleRange[1] = std::pow(10, endpointStrings[0].length()) - 1;
    } else if (endpointStrings[0].length()%2 == 0 && endpointStrings[1].length()%2 == 0) { //even-even
        possibleRange[0] = endpointInts[0];
        possibleRange[1] = endpointInts[1];
    }

    //find numbers that are pre-solved
    bool matchEnd {false};
    std::vector<std::vector<int>> lockedDigits (endpointStrings[0].length());
    for (int i{0}; i < lockedDigits.size()/2; i++) {
        if (endpointStrings[0][i] == std::to_string(possibleRange[1])[i] && !matchEnd) {
            lockedDigits[i].push_back(endpointStrings[0][i] - '0');
            lockedDigits[lockedDigits.size() - i - 1].push_back(endpointStrings[0][i] - '0');
        } else if (!matchEnd) {
            matchEnd = true;
            pushBackRange(lockedDigits[i], endpointStrings[0][i] - '0', std::to_string(possibleRange[1])[i] - '0');
            pushBackRange(lockedDigits[lockedDigits.size() - i - 1], endpointStrings[0][i] - '0', std::to_string(possibleRange[1])[i] - '0');
        } else {
            pushBackRange(lockedDigits[i], 1, 9);
            pushBackRange(lockedDigits[lockedDigits.size() - i - 1], 1, 9);
        }
    }

    //lists all possible first halves of solutions based on combos of lockedDigits
    std::vector<int> numberCombos{ helperCounter(lockedDigits, 1) };
    for (int i{0}; i < numberCombos.size(); i++) {
        std::cout << numberCombos[i] << ", ";
    }
    std::cout << "\n";

    //iterate through numberCombos to count

    // std::cout << endpointInts[0] << ", " << endpointInts[1] << ": ";
    // for (int i{0}; i < lockedDigits.size(); i++) {
    //     for (int j{0}; j < lockedDigits[i].size(); j++) {
    //         std::cout << lockedDigits[i][j] << " ";
    //     }
    //     std::cout << ", ";
    // }
    // std::cout << '\n';

    return;
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

    int invalidIdCount{0};
    for (int i{0}; i < idRanges.size(); i++) {
        getInvalidIdCount(idRanges[i]);
    }
    std::cout << invalidIdCount << "\n";

    return 0;
}
//dont count dupes
//use regex?