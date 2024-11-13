#include <vector>
#include <cmath>
#include <bitset>
#include <map>
#include <iostream>
#include <fstream>
#include "vNode.h"
#include "genPolynomials.h"


int calculateHammingDistance(std::vector<bool>& code1, std::vector<bool>& code2) {
    if (code1.size() != code2.size()) {
        return -1;
    }

    int hammingDistance = 0;
    for (int i = 0; i < code1.size(); i++) {
        if (code1[i] != code2[i]) {
            hammingDistance++;
        }
    }

    return hammingDistance;
}

std::vector<bool> generateOutput(std::vector<bool>& shiftregister, std::vector<unsigned int>& genPolynomials) {
    std::vector<bool> toReturn = {};
    int k = shiftregister.size();

    bool registerParity=0;

    for (unsigned int genPoly : genPolynomials) {
        genPoly = genPoly << 1 | 1; // just need to add another 1 at the end due to implicit +1 notation
        registerParity = 0;
        // cout << "Current genPoly: " << bitset<8>(genPoly) << endl; // Print binary of genPoly for clarity
        // cout << "Shift Register: " << shiftregister << endl;
        // gen poly = 1011
        // k = 4
        //j = 0
        for (int j = 0; j < k; j++) {
            if (((genPoly >> j) & 1) == 1) {
                // cout << "  - XOR with shiftregister[" << k - 1 - j << "] (" << shiftregister[k - 1 - j] << ")" << endl;
                registerParity ^= shiftregister[k - 1 - j];
            } 
            else {}
        }
        // cout << "Intermediate registerParity: " << registerParity << endl;
        toReturn.push_back(registerParity);
    }
// cout << "Final Output is: " << parityBits << endl;
    return toReturn; 
}

std::vector<bool> addNoise(std::vector<bool>& code, float prob_of_error) {

    std::vector<bool> noisyEncoded = {};
    
    // For every  bit in the  code
    for (int i = 0; i < code.size(); i++) {
        // turn the char back to an int
        // calculate a random number between 0 and 1 and if its less than the p value passed in, flip the bit
        if ((float) rand()/RAND_MAX < prob_of_error) {
            switch (code[i]) {
                case 0:
                    noisyEncoded.push_back(1);
                    break;
                case 1:
                    //cout << "Flipping 1 to 0" << endl;
                    noisyEncoded.push_back(1);
                    //cout << std::bitset<64>(raw_code_as_vector[i]).to_string() << endl;
                    break;
            }
        }
        // if the random number generated isn't less than p, dont flip the bit
        else {
            noisyEncoded.push_back(code[i]);
        }
        
    }
    return noisyEncoded;
}

void printTrellisStates(const std::vector<std::vector<vNode>> &trellis) {
    for (int i = 0; i < trellis.size(); i++) {
        std::cout << "For t = " << i << std::endl;
        for (int j = 0; j < trellis[i].size(); j++) {
            std::cout << "Node State: " << trellis[i][j].state 
                << " Cumulative Hamming Distance: " << trellis[i][j].cumHammingDistance << std::endl;
        }
    }
}


std::vector<bool> recursiveBackTrack(const std::vector<std::vector<vNode>>& trellis, int t, int state) {
    if (t == 0) {
        return {};
    }

    int bestPrevState = 0;
    int minDistance = INT_MAX;
    bool inputArrivalBit = 0;

    for (int prevState = 0; prevState < trellis[t-1].size(); prevState++) {
        if (trellis[t-1][prevState].cumHammingDistance < minDistance) {
            minDistance = trellis[t-1][prevState].cumHammingDistance;
            bestPrevState = prevState;
            inputArrivalBit = trellis[t][state].inputArrivalBit;
        }
    }

    std::vector<bool> result = recursiveBackTrack(trellis, t - 1, bestPrevState);

    result.push_back(inputArrivalBit);

    return result;
}


std::vector<bool> getOriginalCode(const std::vector<std::vector<vNode>> &trellis) {
    int finalState = 0;
    int minDistance = INT_MAX;


    for (int state = 0; state < trellis.back().size(); state++) {
        if (trellis.back()[state].cumHammingDistance < minDistance) {
            minDistance = trellis.back()[state].cumHammingDistance;
            finalState = state;
        }
    }

    return recursiveBackTrack(trellis, trellis.size()-1, finalState);
}



std::vector<std::vector<bool>> generateStates(int k) {
    std::vector<std::vector<bool>> statesVector = {};

    std::vector<bool> curState;
    //Ex. k = 4
    // this loop will run 8 times to generate all 8 possible states
    // i goes from 0-7
    // for i = 0
    for (int i = 0; i < pow(2, k-1); i++) {
        curState = {};
        // bit = 4 - 2 = 2
        // so bit goes from 2-0
        for (int bit = k - 2; bit >= 0; bit--) {
            // i = 0
            // bit = 2
            // 0 >> 2 = 0, & 1 = 0, 0 gets appended
            // bit = 1
            // 0 >> 1 = 0, & 1 = 0, 0 gets appended
            // bit = 0
            // 0 >> 0 = 0, & 1 = 0, 0 gets appended
            curState.push_back(((i >> bit) & 1));
        }
        statesVector.push_back(curState);
    }

    return statesVector;
}

std::vector<bool> calculatePotentialInput(std::vector<bool>& curState, bool next_input) {
    std::vector<bool> input = curState;
    input.push_back(next_input);
    return input;
}


std::vector<bool> stringToVecBool(std::string& message) {

    std::vector<bool> toReturn = {};

    std::bitset<8> curCharacter;

    // for every character in the string
    for (char c : message) {
        curCharacter = c;
        // for every bit in the character
        for (int i = 7; i >= 0; i--) {
            toReturn.push_back(curCharacter[i]);
        }
    }

    return toReturn;
}


std::string vecBoolToString(std::vector<bool>& binary) {
    std::string toReturn = "";

    char c = 0;

    for (int i = 0; i < binary.size() ; i+=8) {
        for (int j = 0; j < 8 ; j++) {
            c = c << 1 | binary[i+j];
        }
        toReturn += c;
    }

    return toReturn;
}

std::string vecBoolToStringBinary(std::vector<bool>& binary) {
    std::string stringBinary = "";

    for (int i = 0; i < binary.size(); i++) {
        stringBinary += to_string(binary[i]);
    }

    return stringBinary;
}

void exportData(std::map<int, std::vector<float>>& k_data_points, std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "K, Success Rate, BER\n";
        for (auto& entry : k_data_points) {
            file << entry.first << "," << entry.second[1] * 100 << "," << entry.second[0] * 100 << "\n";
        }
        file.close();
    }

}

int vecBoolToInt(std::vector<bool>& bits) {
    int value = 0;
    for (bool bit : bits) {
        value = (value << 1) | bit;
    }
    return value;
}

std::vector<std::vector<bool>> takeTranspose(std::vector<std::vector<bool>> inputMatrix) {
    int numRows = inputMatrix.size();
    int numCols = inputMatrix[0].size();  // Get the number of columns from the first row

    // Initialize the transposed matrix with dimensions numCols x numRows
    std::vector<std::vector<bool>> transposed(numCols, std::vector<bool>(numRows));

    // Fill the transposed matrix
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            transposed[j][i] = inputMatrix[i][j];
        }
    }

    return transposed;
}


void printMatrix(std::vector<std::vector<bool>> matrix) {
    std::cout << "[";
    for (int i = 0; i < matrix.size(); i++) {
        std::cout << "[";
        for (bool b : matrix[i]) {
            std::cout << b;
        } std::cout << "]";
    } std::cout << "]" << std::endl;;
}

void printVecBool(std::vector<bool> vector) {
    std::cout << "[";
    for (bool b : vector) {
        std::cout << b;
    } std::cout << "]" << std::endl;
}