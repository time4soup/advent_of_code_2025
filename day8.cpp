#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

int JUNCTION_BOX_COUNT {0};

//info for a juunction box of id, position, and ids of connected boxes
struct JunctionBox{
    int id{JUNCTION_BOX_COUNT};
    int posX{};
    int posY{};
    int posZ{};
    std::vector<int> ConnectionIds{};
};

std::ostream& operator<<(std::ostream& out, const JunctionBox& box) {
    out << box.id << " @ " << box.posX << "," << box.posY << "," << box.posZ;
    return out;
}

//info on ids of 2 JunctionBoxes and the distance between them
struct DistancePair{
    int id1{};
    int id2{};
    double dist{};
};

std::ostream& operator<<(std::ostream& out, const DistancePair& dp) {
    out << "(" << dp.id1 << "," << dp.id2 << ") @ " << dp.dist << "\n";
    return out;
}

//parses text of 3D point in format "11,22,33" for posX, posY, and posZ
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

//returns distance between two JunctionBoxes
double getDistance(const JunctionBox& box1, const JunctionBox& box2) {
    double deltaX{ fabs(box1.posX - box2.posX) };
    double deltaY{ fabs(box1.posY - box2.posY) };
    double deltaZ{ fabs(box1.posZ - box2.posZ) };
    return sqrt(pow(deltaX, 2) + pow(deltaY, 2) + pow(deltaZ, 2));
}

bool helperCheckConnected(std::vector<JunctionBox>& boxes, int targetId, int searchingId, int pastId){
    JunctionBox& searchingBox{ boxes[searchingId] };
    int searchId;
    for (int i{0}; i < searchingBox.ConnectionIds.size(); i++) {
        searchId = searchingBox.ConnectionIds[i];
        if (searchId == targetId)
            return true;
        if (searchId == pastId)
            continue;
        if (helperCheckConnected(boxes, targetId, searchId, searchingId))
            return true;
    }
    return false;
}

//checks connections for a JunctionBox and all connected connections for id1 and id2
bool checkConnected(std::vector<JunctionBox>& boxes, int id1, int id2) {
    JunctionBox& box1{ boxes[id1] };
    JunctionBox& box2{ boxes[id2] };
    int searchId;
    for (int i{0}; i < box1.ConnectionIds.size(); i++) {
        searchId = box1.ConnectionIds[i];
        if (searchId == id2) {
            return true;
        }
        if (helperCheckConnected(boxes, id2, searchId, id1)) {
            return true;
        }
    }
    return false;
}

//marks JunctionBoxes 1 and 2 as connected by adding each to the others ConnectionId lists
void connectBoxes(JunctionBox& box1, JunctionBox& box2) {
    //add check if already connected?
    box1.ConnectionIds.push_back(box2.id);
    box2.ConnectionIds.push_back(box1.id);
}

//lists all possible combinations of JunctionBoxes in list boxes and their distances as DistancePairs
std::vector<DistancePair> listDistances(std::vector<JunctionBox>& boxes) {
    std::vector<DistancePair> distPairs{};
    double dist;
    JunctionBox box1;
    JunctionBox box2;

    for (int i{0}; i < boxes.size(); i++) {
        box1 = boxes[i];
        for (int j{i+1}; j < boxes.size(); j++) {
            box2 = boxes[j];
            dist = getDistance(box1, box2);
            distPairs.push_back(DistancePair{box1.id, box2.id, dist});
        }
    }
    return distPairs;
}

//sorts a list of DistancePairs by distance ascending
void sortDistances(std::vector<DistancePair>& distPairs) {
    struct
    {
        bool operator()(DistancePair dp1, DistancePair dp2) const { return dp1.dist < dp2.dist; }
    }
    distancePairLess;

    std::sort(distPairs.begin(), distPairs.end(), distancePairLess);
}

//prints info about a 2D vector
template <typename T>
void print2dVector(const std::vector<std::vector<T>>& v) {
    std::vector<std::size_t> sizes{};
    std::size_t totalNodeCount{0};
    for (int i{0}; i < v.size(); i++) {
        std::cout << "(" << v[i][0];
        for (int j{1}; j < v[i].size(); j++) {
            std::cout << "," << v[i][j];
        }
        std::cout << ")\n";
    }

    for (int i{0}; i < v.size(); i++) {
        sizes.push_back(v[i].size());
    }
    std::sort(sizes.begin(), sizes.end());

    std::cout << "sizes: " << sizes[0];
    totalNodeCount += sizes[0];
    for (int i{1}; i < sizes.size(); i++) {
        std::cout << "," << sizes[i];
        totalNodeCount += sizes[i];
    }
    std::cout << '\n';

    std::cout << "nodes: " << totalNodeCount << '\n';
}

//recrusive function for PrintJunctionBoxes
void helperPrintJunctionBoxes(std::vector<JunctionBox>& boxes, std::vector<int>& idsFound, std::vector<int>& currentCircuit, int searchingId, int pastId) {
    JunctionBox& searchingBox{ boxes[searchingId] };
    int toSearchId;
    for (int i{0}; i < searchingBox.ConnectionIds.size(); i++) {
        toSearchId = searchingBox.ConnectionIds[i];
        if (toSearchId == pastId) {
            continue;
        } else {
            idsFound.push_back(toSearchId);
            currentCircuit.push_back(toSearchId);
            helperPrintJunctionBoxes(boxes, idsFound, currentCircuit, toSearchId, searchingId);
        }
    }
}

//calculates circuits from JunctionBoxes and prints info
void printJunctionBoxes(std::vector<JunctionBox>& boxes) {
    std::vector<std::vector<int>> circuits{};
    std::vector<int> idsFound{};
    std::vector<int> currentCircuit{};
    int boxId;
    int searchingId;
    bool counted;
    for (int i{0}; i <boxes.size(); i++) {
        currentCircuit.clear();
        counted = false;
        boxId = boxes[i].id;
        for (int j{0}; j < idsFound.size(); j++) {
            if (idsFound[j] == boxId)
                counted = true;
        }
        if (counted)
            continue;
        
        idsFound.push_back(boxId);
        currentCircuit.push_back(boxId);
        for (int j{0}; j < boxes[i].ConnectionIds.size(); j++) {
            searchingId = boxes[i].ConnectionIds[j];
            idsFound.push_back(searchingId);
            currentCircuit.push_back(searchingId);
            helperPrintJunctionBoxes(boxes, idsFound, currentCircuit, searchingId, boxId);
        }
        circuits.push_back(currentCircuit);
    }

    print2dVector(circuits);
}

void printDistancePair(std::vector<JunctionBox>& boxes, DistancePair& dp) {
    JunctionBox box1{boxes[dp.id1]};
    JunctionBox box2{boxes[dp.id2]};
    std::cout << box1 << " and " << box2 << '\n';
}

int main() {
    //reads input file
    std::ifstream inf("junctionBoxes.txt");
    if (!inf) {
        std::cerr << "unable to open junctionBoxes.txt";
        return 1;
    }

    //parses raw input text into vector of JunctionBox objects
    std::string rawText{};
    std::vector<JunctionBox> junctionBoxes{};
    while (inf >> rawText) {
        junctionBoxes.push_back(parseText(rawText, ","));
    }

    //calculates distances between all pairs of boxes in list as DistancePairs
    std::vector<DistancePair> distancePairs{ listDistances(junctionBoxes) };
    sortDistances(distancePairs);
    
    //cycles through distance pairs to connect if unconnected
    DistancePair closestPair;
    DistancePair lastConnected;
    for (int i{0}; i < distancePairs.size(); i++) {
        closestPair = distancePairs[i];
        if (!checkConnected(junctionBoxes, closestPair.id1, closestPair.id2)) {
            connectBoxes(junctionBoxes[closestPair.id1], junctionBoxes[closestPair.id2]);
            lastConnected = closestPair;
            std::cout << i << ": boxes " << closestPair.id1 << " and " << closestPair.id2 << " now connected\n";
        } else {
            std::cout << i << ": boxes " << closestPair.id1 << " and " << closestPair.id2 << " already connected\n";
        }
    }

    printJunctionBoxes(junctionBoxes);
    printDistancePair(junctionBoxes, lastConnected);

    return 0;
}