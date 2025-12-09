#include <fstream>
#include <iostream>
#include <string>

int parseRotationText(std::string lockRotationText) {
    std::string rotationNumString;
    rotationNumString = lockRotationText.substr(1,lockRotationText.length());
    int rotationAmt {std::stoi(rotationNumString)};
    if (lockRotationText[0] == 'L') {
        rotationAmt *= -1;
   } 
   return rotationAmt;
}

int getRotatedPos(int dialPos, int rotationAmt) {
   dialPos += rotationAmt;
   dialPos = dialPos % 100;
   if (dialPos < 0) {
    dialPos = 100 + dialPos;
    // dialPos = dialPos % 100;
   }

   return dialPos;
}

int countZeros(int oldDialPos, int rotationAmt) { //72, -745
    int newZeros{0};
    int partialRotations{rotationAmt % 100}; //-45
    //count full rotations
    newZeros += abs((rotationAmt - partialRotations) / 100);
    int rawNewDialPos{oldDialPos + partialRotations};
    if (rawNewDialPos > 100) {
        newZeros += 1;
    } else if (oldDialPos != 0 && rawNewDialPos < 0) {
        newZeros += 1;
    }
    return newZeros;
}

int main() {
    std::ifstream i_file{ "lockRotation.txt" };

    if (!i_file) {
        std::cerr << "Error opening lockRotation.txt!\n";
        return 1;
    }

    std::string lockRotationText{};
    int newZeroCount{0};
    int totalZeroCount{0};
    int oldDialPos{50};
    int newDialPos;
    int rotationAmt;
    while (i_file >> lockRotationText) {
        rotationAmt = parseRotationText(lockRotationText);
        newDialPos = getRotatedPos(oldDialPos, rotationAmt);
        if (newDialPos == 0) {
            newZeroCount = 1;
        } else {
            newZeroCount = 0;
        }
        newZeroCount += countZeros(oldDialPos, rotationAmt);
        totalZeroCount += newZeroCount;
        std::cout << oldDialPos << " rotates " << rotationAmt << " to " << newDialPos << " hitting " << newZeroCount << " zeros\n";
        oldDialPos = newDialPos;
    }

    std::cout << "Total zeros: " << totalZeroCount << "\n";

    return 0;
}
