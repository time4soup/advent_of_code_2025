#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

bool checkEmpty(std::string& s) {
    if (s == "")
        return true;
    return false;
}

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

void addRange(std::vector<std::vector<long long>>& v, std::string& s) {
    std::vector<std::string> stringRange{ splitText(s, "-") };
    std::vector<long long> longRange{ 
        stoll(stringRange[0]), 
        stoll(stringRange[1])
    };
    v.push_back(longRange);
    return;
}

int checkFreshness(std::vector<std::vector<long long>>& v, std::string& s) {
    long long id{stoll(s)};
    for (std::size_t i{0}; i < v.size(); i++) {
        if (v[i][0] <= id && id <= v[i][1]) {
            return 1; //fresh
        }
    }
    return 0; //spoiled
}

void sortRange(std::vector<std::vector<long long>>& v) {
    struct
    {
        bool operator()(std::vector<long long> a, std::vector<long long> b) const { 
            if (a[0] == b[0]) {
                return a[1] < b[1];
            }
            return a[0] < b[0]; 
        }
    }
    sortByFirst;

    std::sort(v.begin(), v.end(), sortByFirst);
    return;
}

std::vector<std::vector<long long>> combineOverlapRanges(std::vector<std::vector<long long>>& v) {
    long long a0{v[0][0]};
    long long a1{v[0][1]};
    long long b0{v[1][0]};
    long long b1{v[1][1]};
    std::vector<std::vector<long long>> newRanges{};
    std::vector<long long> range{a0, a1};

    for (int i{0}; i < v.size()-1; i++) { //last range case
        a0 = v[i][0];
        a1 = v[i][1];
        b0 = v[i+1][0];
        b1 = v[i+1][1];

        if (range[1] >= b0-1) { //overlap
            if (range[1] >= b1) {
                std::cout << "merge @ " << i << ": " << range[0] << "," << range[1] << " + " << b0 << "," << b1 << " = " << range[0] << "," << range[1] << '\n';
            } else {
                std::cout << "merge @ " << i << ": " << range[0] << "," << range[1] << " + " << b0 << "," << b1 << " = " << range[0] << "," << b1 << '\n';
                range[1] = b1;
            }
        } else {
            newRanges.push_back(range);
            range[0] = b0;
            range[1] = b1;
        }
    }
    newRanges.push_back(range);
    return newRanges;
    //merge @ 6: 1,30 + 21,24 = 1,24   merge @ 13: 1,99 + 44,50 = 1,50   merge @ 15: 1,50 + 46,47 = 1,47
}

int main() {
    std::ifstream inf( "freshFood.txt" );
    if (!inf) {
        std::cerr << "unable to open freshFood.txt";
    }

    std::string rawText;
    std::vector<std::vector<long long>> freshRanges{};
    //fills array with fresh ranges
    while (std::getline(inf, rawText)) {
        if (checkEmpty(rawText))
            break;
        addRange(freshRanges, rawText);
    }

    //sort ranges low to high by low value
    sortRange(freshRanges);
    std::cout << "raw size: " << freshRanges.size() << '\n';
    
    //trims overlapping ranges
    for (int i{0}; i < freshRanges.size(); i++) {
        for (int j{0}; j < freshRanges[i].size(); j++) {
            std::cout << freshRanges[i][j] << ",";
        }
        std::cout << "   ";
    }
    std::cout << '\n';

    std::cout << "sorted size: " << freshRanges.size() << '\n';
    freshRanges = combineOverlapRanges(freshRanges);
    std::cout << "trimmed size: " << freshRanges.size() << '\n';

    for (int i{0}; i < freshRanges.size(); i++) {
        for (int j{0}; j < freshRanges[i].size(); j++) {
            std::cout << freshRanges[i][j] << ",";
        }
        std::cout << "   ";
    }
    std::cout << '\n';

    //checks each available item for freshness
    int freshCount{0};
    while (std::getline(inf, rawText)) {
        freshCount += checkFreshness(freshRanges, rawText); 
    }

    long long validCount{0};
    for (int i{0}; i < freshRanges.size(); i++) {
        std::cout << validCount;
        validCount += freshRanges[i][1] - freshRanges[i][0] + 1;
        std::cout << " \t+ " << freshRanges[i][1] - freshRanges[i][0] + 1 << "\t(" << freshRanges[i][1] << "-" << freshRanges[i][0] << "+1)\n";
    }

    // std::cout << "fresh item count: " << freshCount << '\n';
    std::cout << "valid range count: " << validCount << '\n';

    return 0;
}

//880403489 low
//338693411431457 high

//sort ranges by low value
//lower[i][1] < lower[i+1][0]
    //keep seperate
//lower[i][1] >= lower[i+1][0]
    //join ranges
    //lower[i][0] to lower[i+1][1]
//lower[i][0] == lower[i+1][0]
    //sort by lower[i][1] vs lower[i+1][1]