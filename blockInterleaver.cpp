#include <vector>
#include <iostream>
#include "utility.h"


std::vector<bool> interleave(std::vector<bool> input, int k) { // numRows = numCharacters in input string
    std::vector<bool> interleaved = {};

    // a char is 8 bits
    std::vector<std::vector<bool>> inputAsMatrix(3*k, std::vector<bool>(3*k));

    // pad with zeroes
    while (input.size() % (3*k) != 0) {
        input.push_back(0);
    }


    // put them in the block_interleaver
    for (int i = 0; i < 3*k; i++) {
        for (int j = 0; j < 3*k; j++) {
            inputAsMatrix[i][j] = (input[(i*(3*k)) + j]);
        }
    }

    // Take the transpose of the input matrix to create the actual block interleaver

    std::cout << "Input Matrix: " << std::endl;
    printMatrix(inputAsMatrix);
    
    std::vector<std::vector<bool>> block_interleaver = takeTranspose(inputAsMatrix);
    std::cout << "Transposed: " << std::endl;
    printMatrix(block_interleaver);

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
    std::vector<std::vector<bool>> inputAsMatrix(3*k, std::vector<bool>(3*k));


    // put them in the block_interleaver
    for (int i = 0; i < 3*k; i++) {
        for (int j = 0; j < 3*k; j++) {
            inputAsMatrix[i][j] = (input[(i*(3*k)) + j]);
        }
    }

    // Take the transpose of the input matrix to create the actual block interleaver

    std::cout << "Input Matrix: " << std::endl;
    printMatrix(inputAsMatrix);
    
    std::vector<std::vector<bool>> block_interleaver = takeTranspose(inputAsMatrix);
    std::cout << "Transposed: " << std::endl;
    printMatrix(block_interleaver);

    // read the values back out row by row to get the interleaved output

    for (int i = 0; i < block_interleaver.size(); i++) {
        for (int j = 0; j < block_interleaver[0].size(); j++) {
            interleaved.push_back(block_interleaver[i][j]);
        }
    }

    return interleaved;
}   
