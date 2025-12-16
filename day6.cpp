#include <string>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<std::string> splitText(const std::string& text, const std::string& delimitter) {
    std::vector<std::string> slices;
    std::string::size_type foundN;
    std::string::size_type searchN{0};

    foundN = text.find(delimitter, searchN);
    while (foundN != std::string::npos) {
        if (text.substr(searchN, foundN - searchN) != "")
            slices.push_back(text.substr(searchN, foundN - searchN));
        searchN = foundN + delimitter.length();
        foundN = text.find(delimitter, searchN);
    }
    if (text.substr(searchN, text.length() - searchN) != "")
        slices.push_back(text.substr(searchN, text.length() - searchN));

    return slices;
}

std::vector<char> splitChars(const std::string& s) {
    std::vector<char> v{};
    for (int i{0}; i < s.length(); i++) {
        v.push_back(s[i]);
    }
    return v;
}

int charsToNum(std::vector<std::vector<char>>& v, size_t j) {
    std::string s{""};
    for (int i{0}; i < v.size()-1; i++) {
        if (v[i][j] != ' ') {
            s += v[i][j];
        }
    }
    if (s == "")
        return 0;
    try {
        return stoi(s);
        throw s;
    }
    catch (std::string) { //fix
        std::cerr << "unable to print (" << s << ")\n";
    }
    return 0;
}

long long totalNums(std::vector<int>& v, std::string& symbol) {
    long long total{v[0]};
    std::cout << total;
    for (int i{1}; i < v.size(); i++) {
        if (symbol == "+") {
            std::cout << "+" << v[i];
            total += v[i];
        }
        if (symbol == "*") {
            std::cout << "*" << v[i];
            total *= v[i];
        }
    }
    std::cout << "=" << total << '\n';
    return total;
}

long long sumEquations(std::vector<std::vector<char>>& v, std::vector<std::string>& symbols) {
    std::string symbol{v.back()[0]};
    std::vector<int> nums{};
    long long total{0};
    for (int i{0}; i < v[0].size(); i++) {
        if (v[0][i] == ' ' && v[1][i] == ' ' && v[2][i] == ' ' && v[3][i] == ' ') {
            total += totalNums(nums, symbol);
            symbol = v.back()[i+1];
            nums.clear();
        } else {
            nums.push_back(charsToNum(v, i));
        }
    }
    total += totalNums(nums, symbol);
    return total;
}

int main() {
    std::ifstream inf("cephalopodMath.txt");
    if (!inf) {
        std::cerr << "unable to open cephalopodMath.txt\n";
    }

    std::string rawText{};
    std::vector<std::vector<char>> mathArray{};
    std::vector<char> charRow{};
    std::vector<std::string> symbols{};
    int i{0};
    while (std::getline(inf, rawText)) {
        if (i < 5) {
        charRow = splitChars(rawText);
        mathArray.push_back(charRow);
        } else {
            symbols = splitText(rawText, " ");
        }
        i++;
    }

    std::cout << "total: " << sumEquations(mathArray, symbols) << '\n';
}