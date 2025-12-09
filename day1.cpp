#include <fstream>
#include <iostream>
#include <string>

int rotateLock(int dialPos, std::string lockRotationText) {
    /*
    parse lockRotationText
    if L: subtract, if R: add
    find new lock pos
    modulo 100 if rotated
    */
   std::string rotationNumString;
    rotationNumString = lockRotationText.substr(1,lockRotationText.length());
   int rotationAmt {std::stoi(rotationNumString)};
   if (lockRotationText[0] == 'L') {
        dialPos -= rotationAmt;
   } else {
        dialPos += rotationAmt;
   }

   dialPos = dialPos % 100;
   if (dialPos < 0) {
    dialPos = 100 + dialPos;
    dialPos = dialPos % 100;
   }

   return dialPos;
}

int main() {
    std::ifstream i_file{ "lockRotation.txt" };

    if (!i_file) {
        std::cerr << "Error opening lockRotation.txt!\n";
        return 1;
    }

    std::string lockRotationText{};
    int dialPos{50};
    int zeroCount{0};
    while (i_file >> lockRotationText) {
        std::cout << "Lock at " << dialPos << " rotates " << lockRotationText << "\n";
        dialPos = rotateLock(dialPos, lockRotationText);
        if (dialPos == 0) {
            zeroCount += 1;
        }
    }

    std::cout << "Total zeros: " << zeroCount << "\n";

    return 0;
}
