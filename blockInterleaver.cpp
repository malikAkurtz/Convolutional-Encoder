#include <vector>
#include <iostream>
#include <algorithm>
#include "utility.h"


std::vector<bool> interleave(std::vector<bool> input, int k) { // numRows = numCharacters in input string
    std::vector<bool> interleaved = {};
    while ((input.size() % 8) != 0) {
        input.push_back(0);
    }

    // a char is 8 bits
    std::size_t numRows = std::max(static_cast<std::size_t>(3 * k), input.size()/8);
    std::vector<std::vector<bool>> inputAsMatrix(numRows, std::vector<bool>(8));



    // put them in the block_interleaver
    for (int i = 0; i < 3*k; i++) {
        for (int j = 0; j < 8; j++) {
            inputAsMatrix[i][j] = (input[(i*(8)) + j]);
        }
    }

    // Take the transpose of the input matrix to create the actual block interleaver

    // std::cout << "Input Matrix: " << std::endl;
    // printMatrix(inputAsMatrix);
    
    std::vector<std::vector<bool>> block_interleaver = takeTranspose(inputAsMatrix);
    // std::cout << "Transposed: " << std::endl;
    // printMatrix(block_interleaver);

    // read the values back out row by row to get the interleaved output

    for (int i = 0; i < block_interleaver.size(); i++) {
        for (int j = 0; j < block_interleaver[0].size(); j++) {
            interleaved.push_back(block_interleaver[i][j]);
        }
    }

    return interleaved;
}   

std::vector<bool> deinterleave(std::vector<bool> input, int k) { // numRows = numCharacters in input string
    std::vector<bool> interleaved = {};

    // a char is 8 bits
    std::size_t numCols = std::max(static_cast<std::size_t>(3 * k), input.size()/8);
    std::vector<std::vector<bool>> inputAsMatrix(8, std::vector<bool>(numCols));


    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3*k; j++) {
            inputAsMatrix[i][j] = (input[(i*(3*k)) + j]);
        }
    }

    // Take the transpose of the input matrix to create the actual block interleaver

    // std::cout << "Input Matrix: " << std::endl;
    // printMatrix(inputAsMatrix);
    
    std::vector<std::vector<bool>> block_interleaver = takeTranspose(inputAsMatrix);
    // std::cout << "Transposed: " << std::endl;
    // printMatrix(block_interleaver);

    // read the values back out row by row to get the interleaved output

    for (int i = 0; i < block_interleaver.size(); i++) {
        for (int j = 0; j < block_interleaver[0].size(); j++) {
            interleaved.push_back(block_interleaver[i][j]);
        }
    }

    return interleaved;
}   


// int main() {

//     std::string message = "This is a much longer message";
//     std::vector<bool> messageBinary = stringToVecBool(message);
//     std::cout << "To Interleave: " << vecBoolToStringBinary(messageBinary) << std::endl;
    

//     std::vector<bool> interleaved = interleave(messageBinary, 2);
//     std::cout << "Interleaved: " << vecBoolToStringBinary(interleaved) << std::endl;
//     std::vector<bool> deinterleaved = deinterleave(interleaved, 2);
//     std::cout << "Deinterleaved: " << vecBoolToStringBinary(deinterleaved) << std::endl;
//     std::cout << "Original: " << vecBoolToString(deinterleaved) << std::endl;
    

//     return 0;
// }