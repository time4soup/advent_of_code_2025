#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

struct Line{
    std::vector<long long> p1;
    std::vector<long long> p2;

    char direction() {
        if (p1[0] == p2[0])
            return 'v';
        return 'h';
    };

    long long length() {
        if (direction() == 'v')
            return abs(p1[1]-p2[1]);
        return abs(p1[0]-p2[0]);
    };
};

std::vector<long long> parseCoordinates(const std::string& s) {
    std::string::size_type seperatorLoc;
    seperatorLoc = s.find(",");
    return {
        stoll(s.substr(0, seperatorLoc)),
        stoll(s.substr(seperatorLoc+1, s.size()-1))
    };
}

long long getArea(std::vector<long long>& pos1, std::vector<long long>& pos2) {
    long long deltaX{abs(pos1[0] - pos2[0])+1};
    long long deltaY{abs(pos1[1] - pos2[1])+1};
    return deltaX * deltaY;
}

std::vector<long long> getRectPosition(std::vector<long long>& rectP1, std::vector<long long>& rectP2) {
    long long xCoord;
    long long yCoord;

    if (rectP1[0] < rectP2[0]) {
        xCoord = rectP1[0];
    } else {
        xCoord = rectP2[0];
    }

    if (rectP1[1] < rectP2[1]) {
        yCoord = rectP1[1];
    } else {
        yCoord = rectP2[1];
    }

    return {xCoord, yCoord};
}

bool checkIntersection(Line& l1, Line& l2) {
    std::vector<long long> IntersectCoords(2);
    Line verticalLine;
    Line horizontalLine;

    if (l1.p1[0] == l1.p2[0]) {
        verticalLine = l1;
        horizontalLine = l2;
    } else {
        verticalLine = l2;
        horizontalLine = l1;
    }
    IntersectCoords = {verticalLine.p1[0], horizontalLine.p1[1]};

    if (!(IntersectCoords[0] < horizontalLine.p1[0] && IntersectCoords[0] > horizontalLine.p2[0]) && !(IntersectCoords[0] > horizontalLine.p1[0] && IntersectCoords[0] < horizontalLine.p2[0])) {
        return false;
    }
    if (!(IntersectCoords[1] < verticalLine.p1[1] && IntersectCoords[1] > verticalLine.p2[1]) && !(IntersectCoords[1] > verticalLine.p1[1] && IntersectCoords[1] < verticalLine.p2[1])) {
        return false;
    }
    return true;
}

bool validateRectangle(std::vector<Line>& boundingLines, std::vector<long long>& rectP1, std::vector<long long>& rectP2) {
    long long width{ abs(rectP1[0]-rectP2[0]) };
    long long height{ abs(rectP1[1]-rectP2[1]) };
    std::vector<long long> rectPosition { getRectPosition(rectP1, rectP2) };
    Line boundingLine{};
    Line rectLine1{};
    Line rectLine2{};

    for (int i{0}; i < boundingLines.size(); i++) {
        boundingLine = boundingLines[i];
        if (boundingLine.direction() == 'v') {
            rectLine1.p1 = {rectPosition};
            rectLine1.p2 = {rectPosition[0]+width, rectPosition[1]};
            rectLine2.p1 = {rectPosition[0], rectPosition[1]-height};
            rectLine2.p2 = {rectPosition[0]+width, rectPosition[1]-height};
        } else {
            rectLine1.p1 = {rectPosition};
            rectLine1.p2 = {rectPosition[0], rectPosition[1]-height};
            rectLine2.p1 = {rectPosition[0]+width, rectPosition[1]};
            rectLine2.p2 = {rectPosition[0]+width, rectPosition[1]-height};
        }

        if (checkIntersection(boundingLine, rectLine1) || checkIntersection(boundingLine, rectLine2))
            return false;
    }
    return true;
}

//returns list of TileLines for each pair of exterior lines in tile shape
std::vector<Line> calculateTileLines(std::vector<std::vector<long long>>& tiles) {
    std::vector<Line> v{};

    for (int i{0}; i < tiles.size(); i++) {
        if (i == tiles.size()-1) {
            v.push_back(Line{tiles[i], tiles[0]});
        } else {
            v.push_back(Line{tiles[i], tiles[i+1]});
        }
    }
    return v;
}

int main() {
    std::ifstream inf( "redTileLocations.txt" );
    if (!inf) {
        std::cerr << "unable to open redTileLocations.txt";
        return 1;
    }

    std::string rawText{};
    std::vector<std::vector<long long>> tileLocations{};
    while (inf >> rawText) {
        tileLocations.push_back(parseCoordinates(rawText));
    }

    std::vector<Line> tileLines{ calculateTileLines(tileLocations) };

    long long maxArea{0};
    long long currentArea{};
    for (int i{0}; i < tileLocations.size(); i++) {
        for (int j{i+1}; j < tileLocations.size(); j++) {
            currentArea = getArea(tileLocations[i], tileLocations[j]);
            if (currentArea > maxArea && validateRectangle(tileLines, tileLocations[i], tileLocations[j])) {
                maxArea = currentArea;
                std::cout << "(" << tileLocations[i][0] << "," << tileLocations[i][1] << ") (" << tileLocations[j][0] << "," << tileLocations[j][1] << "): " << currentArea << " is new max\n";
            }
        }
    }

    std::cout << "max area: " << maxArea << '\n';
    std::cout << "size: " << tileLocations.size() << '\n';
}

//3750797226 too high