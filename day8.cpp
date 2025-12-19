#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

int JUNCTION_BOX_COUNT {0};

struct JunctionBox{
    int id{JUNCTION_BOX_COUNT};
    int posX{};
    int posY{};
    int posZ{};
    std::vector<int> ConnectionIds{};
};

std::ostream& operator<<(std::ostream& out, const JunctionBox& box) {
    out << box.id << " @ " << box.posX << "," << box.posY << "," << box.posZ << '\n';
    return out;
}

JunctionBox parseText(const std::string& s, const std::string& delimitter) {
    JunctionBox junctionBox{};
    JUNCTION_BOX_COUNT += 1;
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

double distance(const JunctionBox& box1, const JunctionBox& box2) {
    double deltaX{ fabs(box1.posX - box2.posX) };
    double deltaY{ fabs(box1.posY - box2.posY) };
    double deltaZ{ fabs(box1.posZ - box2.posZ) };
    return sqrt(pow(deltaX, 2) + pow(deltaY, 2) + pow(deltaZ, 2));
}

//returns 1 if sharing circuit, or 0 if not
bool checkConnected(std::vector<JunctionBox>& boxes, int id1, int id2) {
    if (id1 == -1 || id2 == -1) {
        std::cerr << "checking invalid boxes with id '-1'\n";
        return -1;
    } 
    JunctionBox box1{ boxes[id1] };
    JunctionBox box2{ boxes[id2] };

    for (int i{0}; i < box1.ConnectionIds.size(); i++) {
        if (box1.ConnectionIds[i] == id2) {
            return true;
        }
    }
    return false;
}

void connectBoxes(std::vector<JunctionBox>& boxes, int id1, int id2) {
    if (id1 == -1 || id2 == -1) {
        std::cerr << "linking invalid boxes with id '-1'\n";
        return;
    }
    JunctionBox box1{ boxes[id1] };
    JunctionBox box2{ boxes[id2] };

    box1.ConnectionIds.push_back(id2);
    box2.ConnectionIds.push_back(id1);
}

void getClosest(std::vector<JunctionBox> boxes) {
    double closestDist{INFINITY};
    int closestId1{-1};
    int closestId2{-1};
    double dist{0.0};

    for (int i{0}; i < boxes.size(); i++) {
        JunctionBox box1{ boxes[i] };
        for (int j{i+1}; j < boxes.size(); j++) {
            JunctionBox box2{ boxes[j] };
            dist = distance(box1, box2);
            if (dist < closestDist && !checkConnected(boxes, i, j)) {
                closestDist = dist;
                closestId1 = i;
                closestId2 = j;
            }
        }
    }
    connectBoxes(boxes, closestId1, closestId2);
    std::cout << "connected boxes " << closestId1 << " and " << closestId2 << '\n';
}

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
        std::cout << junctionBoxes[i];
    }

    for (int i{0}; i < 1000; i++) { //goal to connect closest 1000 boxes
        getClosest(junctionBoxes);
    }
    std::cout << "distance: " << distance(junctionBoxes[424], junctionBoxes[712]) << '\n';
    std::cout << "size: " << junctionBoxes.size() << '\n';

    return 0;
}


