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
            std::cout << id << " is fresh in range: " << v[i][0] << "-" << v[i][1] << '\n';
            return 1; //fresh
        }
    }
    return 0; //spoiled
}

void sortRange(std::vector<std::vector<long long>>& v) {
    std::sort(v.begin(), v.end(), [](long long a, long long b)
        {
            return a[0] < b[0];
        }
    );
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

    sortRange(freshRanges);

    //checks each available item for freshness
    int freshCount{0};
    while (std::getline(inf, rawText)) {
        freshCount += checkFreshness(freshRanges, rawText); 
    }

    std::cout << "fresh item count: " << freshCount << '\n';

    return 0;
}

//sort ranges by low value
//lower[i][1] < lower[i+1][0]
    //keep seperate
//lower[i][1] >= lower[i+1][0]
    //join ranges
    //lower[i][0] to lower[i+1][1]
//lower[i][0] == lower[i+1][0]
    //sort by lower[i][1] vs lower[i+1][1]