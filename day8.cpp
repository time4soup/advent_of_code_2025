#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

struct JunctionBox{
    int id{}; //setup id counti
    int posX{};
    int posY{};
    int posZ{};
    int FirstConnectionId{};
    int SecondConnectionId{};
};

JunctionBox parseText(const std::string& s, const std::string& delimitter) {
    JunctionBox junctionBox{};
    std::vector<std::string::size_type> foundSeperators{};
    std::string::size_type searchPos{0};
    std::string::size_type foundPos{0};

    foundPos = s.find(delimitter, searchPos);
    while (foundPos != std::string::npos) {
        foundSeperators.push_back(foundPos);
        searchPos = foundPos + delimitter.length();
        foundPos = s.find(delimitter, searchPos);
    }
    junctionBox.posX = stoi(s.substr(0, foundSeperators[0]));
    junctionBox.posY = stoi(s.substr(foundSeperators[0] + 1, foundSeperators[1] - foundSeperators[0] - 1));
    junctionBox.posZ = stoi(s.substr(foundSeperators[1] + 1, s.length() - foundSeperators[1] - 1));

    return junctionBox;
}

double distance(JunctionBox& box1, JunctionBox& box2) {
    double deltaX{ fabs(box1.posX - box2.posX) };
    double deltaY{ fabs(box1.posY - box2.posY) };
    double deltaZ{ fabs(box1.posZ - box2.posZ) };
    return sqrt(pow(deltaX, 2) + pow(deltaY, 2) + pow(deltaZ, 2));
}

//getClosest()
//cycles through all junctionBox pairs
////for i in range junctionBoxes
////compare i and i+1 through junctionBoxes.size()
//keep track of closest
////must not already be connected

int main() {
    std::ifstream inf("junctionBoxes.txt");
    if (!inf) {
        std::cerr << "unable to open junctionBoxes.txt";
        return 1;
    }

    std::string rawText{};
    std::vector<JunctionBox> junctionBoxes{};
    while (inf >> rawText) {
        junctionBoxes.push_back(parseText(rawText, ","));
    }

    for (int i{0}; i < junctionBoxes.size(); i++) {
        std::cout << junctionBoxes[i].posX << "," << junctionBoxes[i].posY << "," << junctionBoxes[i].posZ << '\n';
    }
    std::cout << "distance: " << distance(junctionBoxes[0], junctionBoxes[1]);

    return 0;
}
//set junctionBox IDs