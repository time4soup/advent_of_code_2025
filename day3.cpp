#include <string>
#include <fstream>
#include <iostream>

//search string of digits to find maximum digit, if tied furthest left
//start and end are inclusive
int findMax(std::string s, int start, int end) {
    int runningMax{-1};
    int maxPos{-1};
    int toCheck;
    for (int i{start}; i <= end; i++) {
        toCheck = s[i] - '0';
        if (toCheck > runningMax) {
            runningMax = toCheck;
            maxPos = i;
        }
    }
    return maxPos;
}

//searches the string to make the largest string of n digits from the string
//digits must stay in the order they were originally in the string
long long maxJoltage(std::string bank, int n) {
    int pos{-1};
    std::string final{};
    for (int i{0}; i < n; i++) {
        pos = findMax(bank, pos + 1, bank.size() + i - n);
        final += bank[pos];
    }
    std::cout << bank << '\n';
    std::cout << final << '\n';
    return stoll(final);
    
}

int main() {
    std::ifstream inf{"joltageBanks.txt"};
    if (!inf) {
        std::cerr << "Unable to open joltageBanks.txt\n";
    }
    std::string bank;
    long long totalJoltage{0};
    while (inf >> bank) {
        totalJoltage += maxJoltage(bank, 12);
    }
    std::cout << "total jolts: " << totalJoltage << '\n';
}
//works for part 1 and 2, just have to replace maxJoltage() call n with 2 or 12