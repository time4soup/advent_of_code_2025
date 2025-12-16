#include <string>
#include <iostream>
#include <fstream>
#include <vector>

//creates 2D array of position of each splitter in each row
void storeRow(std::vector<std::vector<size_t>>& v, const std::string& s) {
    std::vector<size_t> splitters{};
    std::string::size_type loc{s.find('^')};
    if (loc == std::string::npos)
        return;
    while (loc != std::string::npos) {
        splitters.push_back(loc);
        loc = s.find('^', loc + 1);
    }
    v.push_back(splitters);
}

int shootBeam(std::vector<std::vector<size_t>>& v, size_t start) {
    std::vector<int> beam(v.front().size());
    std::size_t pos{};
    int count{0};
    beam[start] = 1;

    for (int i{0}; i < v.size(); i++) {
        for (size_t j{0}; j < v[i].size(); j++) {
            pos = v[i][j];
            if (beam[pos] == 1) {
                beam[pos] = 0;
                beam[pos+1] = 1;
                beam[pos-1] = 1;
                count += 1;
            }
        }
        std::cout << count << '\n';
    }
    return count;
}

int quantumBeamHelper(std::vector<std::vector<size_t>>& v, int row, size_t beamPos) {
    if (row == v.size())
        return 0;
    int count{0};
    for (int i{0}; i < v[row].size(); i++) {
        if (beamPos == v[row][i]) {
            count += 2;
            count += quantumBeamHelper(v, row+1, beamPos+1);
            count += quantumBeamHelper(v, row+1, beamPos-1);
        }
    }
    return count;
}

int quantumBeam(std::vector<std::vector<size_t>>& v, size_t start) {
    int count{0};
    size_t row{0};
    for (int i{0}; i < v[row].size(); i++) {
        if (start == v[row][i]) {
            count += 2;
            count += quantumBeamHelper(v, row+1, start+1);
            count += quantumBeamHelper(v, row+1, start-1);
        }
    }
    return count;
}

int main() {
    std::ifstream inf( "tachyonBeams.txt" );
    if (!inf) {
        std::cerr << "unable to open tachyonBeams.txt";
    }

    std::string rawText{};
    inf >> rawText;
    size_t start{rawText.find('S')};
    std::vector<std::vector<size_t>> beamMap{};
    while (inf >> rawText) {
        storeRow(beamMap, rawText);
    }

    int splits{quantumBeam(beamMap, start)};
    std::cout << "splits: " << splits << '\n';

    // for (int i{0}; i < beamMap.size(); i++) {
    //     for (int j{0}; j < beamMap[i].size(); j++) {
    //         std::cout << beamMap[i][j] << ' ';
    //     }
    //     std::cout << '\n';
    // }

    return 0;

}