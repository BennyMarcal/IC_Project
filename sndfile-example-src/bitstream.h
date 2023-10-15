#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <bitset>

class BitStream {
private:
    std::fstream file;
    std::string fileMode;
    std::string fileName;
    int fileSize;
    std::vector<int> bitArray;
    int currentBitPos;
    int currentArrayPos;

public:
    BitStream(std::string name, std::string mode) {
        fileName = name;
        fileMode = mode;

        if (mode == "r") {
            file.open(fileName, std::ios::in | std::ios::binary);
            currentBitPos = 0;
            fileSize = getFileSize();
        } else if (mode == "w") {
            file.open(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
            currentBitPos = 0;
            currentArrayPos = 0;
        } else {
            std::cout << "Invalid mode" << std::endl;
        }
    }

    int getFileSize() {
        std::fstream file2;
        file2.open(fileName, std::ios::in | std::ios::binary);
        file2.seekg(0, std::ios::end);
        int size = file2.tellg();
        file2.seekg(0, std::ios::beg);
        file2.close();
        return size;
    }

    std::vector<int> byteToBitArray(char byte) {
        std::vector<int> bitArray;
        for (int i = 7; i >= 0; i--) {
            bitArray.push_back((byte >> i) & 1);
        }
        return bitArray;
    }

    char bitArrayToByte(std::vector<int> bitArray) {
        char byte = 0;
        for (int i = 0; i < 8; i++) {
            byte |= bitArray[i] << (7 - i);
        }
        return byte;
    }

    std::vector<int> readBits(int n) {
        if (fileMode != "r") {
            std::cout << "File not open for reading" << std::endl;
            return std::vector<int>();
        }

        std::vector<int> outBits;

        while (n > 0) {
            if (currentBitPos == 0) {
                char byte;
                file.read(&byte, 1);
                bitArray = byteToBitArray(byte);
            }

            outBits.push_back(bitArray[currentBitPos]);
            currentBitPos = (currentBitPos + 1) % 8;
            n--;
        }

        return outBits;
    }

    int readBit() {
        if (fileMode != "r") {
            std::cout << "File not open for reading" << std::endl;
            return -1;
        }

        if (currentBitPos == 0) {
            char byte;
            file.read(&byte, 1);
            bitArray = byteToBitArray(byte);
        }

        int bit = bitArray[currentBitPos];
        currentBitPos = (currentBitPos + 1) % 8;

        return bit;
    }

    void writeBits(std::vector<int> bits) {
        if (fileMode != "w") {
            std::cout << "File is not open for writing" << std::endl;
            return;
        }

        for (int i = 0; i < bits.size(); i++) {
            if (currentBitPos == 0) {
                bitArray = std::vector<int>(8);
            }
            bitArray[currentBitPos] = bits[i];
            currentBitPos = (currentBitPos + 1) % 8;

            if (currentBitPos == 0) {
                char byte = bitArrayToByte(bitArray);
                file.write(&byte, 1);
            }
        }
    }

    void writeBit(int bit) {
        if (fileMode != "w") {
            std::cout << "File is not open for writing" << std::endl;
            return;
        }

        if (currentBitPos == 0) {
            bitArray = std::vector<int>(8);
        }
        bitArray[currentBitPos] = bit;
        currentBitPos = (currentBitPos + 1) % 8;

        if (currentBitPos == 0) {
            char byte = bitArrayToByte(bitArray);
            file.write(&byte, 1);
        }
    }

    void close() {
        if (currentBitPos != 0) {
            char byte = bitArrayToByte(bitArray);
            file.write(&byte, 1);
        }
        file.close();
    }
};
