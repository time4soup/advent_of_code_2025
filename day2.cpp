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

//adds range from start to end to back of vector v
void pushBackRange(std::vector<int>& v, int start, int end) {
    for (int i{start}; i < end + 1; i++) {
        v.push_back(i);
    }
}

std::vector<std::string> helperCounter(std::vector<std::vector<int>>& v, int i) {
    std::vector<std::string> nums{};
    if (i >= v.size()/2 - 1) {
        for (int j{0}; j < v[i].size(); j++) {
            nums.push_back(std::to_string(v[i][j]));
        }
        return nums;
    } else {
        std::vector<std::string> numEndings {helperCounter(v, i+1)};
        for (int j{0}; j < v[i].size(); j++) {
            for (int k{0}; k < numEndings.size(); k++) {
                nums.push_back(std::to_string(v[i][j]) + numEndings[k]);
            }
        }
        return nums;
    }
}

//convert half numbers into full mirrored numbers (eg. 456 -> 456654)
long long dupeNums(int halfNum) {
    std::string s {std::to_string(halfNum)};
    return stoll(s+s);
}

long long getInvalidIdCount(std::string rangeText) {
    std::vector<std::string> endpointStrings {splitText(rangeText, "-")};
    std::vector<long long> endpointInts {
        std::stoll(endpointStrings[0]), 
        std::stoll(endpointStrings[1])
    };
        
    //find possible range of even lengthed nums
    std::vector<long long> possibleRange (2);
    if (endpointStrings[0].length()%2 != 0 && endpointStrings[1].length()%2 != 0) { //odd-odd is invalid
        return 0;
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
            //check is pushing right digit
            lockedDigits[lockedDigits.size()/2 + i].push_back(endpointStrings[0][i] - '0');
        } else if (!matchEnd) {
            matchEnd = true;
            pushBackRange(lockedDigits[i], endpointStrings[0][i] - '0', std::to_string(possibleRange[1])[i] - '0');
            //check this too
            pushBackRange(lockedDigits[lockedDigits.size()/2 + i], endpointStrings[0][i] - '0', std::to_string(possibleRange[1])[i] - '0');
        } else {
            pushBackRange(lockedDigits[i], 0, 9);
            pushBackRange(lockedDigits[lockedDigits.size()/2 + i], 0, 9);
        }
    }

    //lists all possible first halves of solutions based on combos of lockedDigits
    std::vector<std::string> numberCombosStr{ helperCounter(lockedDigits, 0) };
    std::vector<int> numberCombos{};
    for (int i{0}; i < numberCombosStr.size(); i++) {
        numberCombos.push_back(std::stoi(numberCombosStr[i]));
    }

    //iterate through numberCombos to count
    long long invalidCount{0};
    long long toCheck{0};
    for (int i{0}; i < numberCombos.size(); i++) {
        toCheck = dupeNums(numberCombos[i]);
        if (toCheck >= possibleRange[0] && toCheck <= possibleRange[1]) {
            invalidCount += toCheck;
        }
    }

    return invalidCount;
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