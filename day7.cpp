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

std::vector<long long> shootBeam(std::vector<std::vector<size_t>>& v, size_t start) {
    std::vector<long long> beam(141); //width of beam splitters
    std::size_t pos{};
    beam[start] = 1;

    for (int i{0}; i < v.size(); i++) {
        for (size_t j{0}; j < v[i].size(); j++) {
            pos = v[i][j];
            if (beam[pos] >= 1) {
                beam[pos+1] += beam[pos];
                beam[pos-1] += beam[pos];
                beam[pos] = 0;
            }
        }
    }
    return beam;
}

int quantumBeamHelper(std::vector<std::vector<size_t>>& v, int row, size_t beamPos) {
    if (row >= v.size())
        return 0;
    int count{0};
    std::cout << "beam: " << beamPos << ", row: " << row << '\n';
    for (int i{0}; i < v[row].size(); i++) {
        if (beamPos == v[row][i]) {
            std::cout << "beam hit at " << beamPos << " in row " << row << '\n';
            count += 1;
            count += quantumBeamHelper(v, row+1, beamPos+1);
            count += quantumBeamHelper(v, row+1, beamPos-1);
        }
    }
    return count;
}

int quantumBeam(std::vector<std::vector<size_t>>& v, size_t start) {
    int count{1};
    size_t row{0};
    std::cout << "beam: " << start << ", row: " << row << '\n';
    for (int i{0}; i < v[row].size(); i++) {
        if (start == v[row][i]) {
            count += 1;
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

    std::vector<long long> beams{shootBeam(beamMap, start)};

    long long count{0};
    std::cout << "final possible beam positions: \n";
    for (int i{0}; i < beams.size(); i++) {
        count += beams[i];
        std::cout << beams[i] << ",";
    }
    std::cout << '\n';
    std::cout << "total count: " << count << '\n';

    return 0;

}