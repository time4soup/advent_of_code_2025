#include <string>
#include <vector>
#include <iostream>
#include <fstream>

struct redTile {
    std::vector<long long> coords{};
    int dir1{};
    int dir2{};
};

std::vector<std::string> parseText(const std::string& s, char delimitter) {
    std::vector<std::size_t> delimPositions{};
    std::size_t searchPos{0};
    std::size_t foundPos(std::string::npos);
    while (true) {
        foundPos = s.find_first_of(delimitter, searchPos);
        if (foundPos == std::string::npos)
            break;
        delimPositions.push_back(foundPos);
        searchPos = foundPos+1;
    }

    std::vector<std::string> parsedString{};
    parsedString.push_back(s.substr(0, delimPositions[0]));
    for (std::size_t i{0}; i < delimPositions.size()-1; i++) {
        parsedString.push_back(s.substr(delimPositions[i]+1, delimPositions[i+1]-delimPositions[i]-1));
    }
    parsedString.push_back(s.substr(delimPositions.back()+1, s.size()-delimPositions.back()-1));
    return parsedString;
}

redTile parseTextToTile(const std::string& s) {
    std::vector<std::string> parsedText{parseText(s, ',')};
    std::vector<long long> coords{std::stoll(parsedText[0]), std::stoll(parsedText[1])};
    return redTile{coords, -1, -1};
}

void setDirForTile(std::vector<redTile>& redTiles, std::size_t i1, std::size_t i2) {
    redTile currentTile{redTiles[i1]};
    redTile nextTile{redTiles[i2]};
    if (currentTile.coords[0] == nextTile.coords[0]) {
        if (currentTile.coords[1] > nextTile.coords[1]) {
            redTiles[i1].dir2 = 2;
            redTiles[i2].dir1 = 0;
        } else {
            redTiles[i1].dir2 = 0;
            redTiles[i2].dir1 = 2;
        }
    } else {
        if (currentTile.coords[0] > nextTile.coords[0]) {
            redTiles[i1].dir2 = 3;
            redTiles[i2].dir1 = 1;
        } else {
            redTiles[i1].dir2 = 1;
            redTiles[i2].dir1 = 3;
        }
    }
}

void setDirForTiles(std::vector<redTile>& redTiles) {
    setDirForTile(redTiles, redTiles.size()-1, 0);
    for (std::size_t i{0}; i < redTiles.size()-1; i++) {
        setDirForTile(redTiles, i, i+1);
    }
}

int main() {
    std::ifstream inf("redTiles.txt");
    if (!inf) {
        std::cerr << "unable to open redTiles.txt";
        return 1;
    }

    std::string rawText{};
    std::vector<redTile> redTiles{};
    while (inf >> rawText) {
        redTiles.push_back(parseTextToTile(rawText));
    }
    setDirForTiles(redTiles);

    std::cout << redTiles.size();
}