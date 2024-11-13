#ifndef CONVITERBI_H
#define CONVITERBI_H

#include <vector>
#include <map>
#include "vNode.h"
#include "genPolynomials.h"


std::vector<bool> encode(std::vector<bool> code, int k, std::vector<unsigned int> genPolynomials, int &timeSteps);
std::vector<bool> viterbiDecode(std::vector<bool> noisy_encoded_code, int k, std::vector<std::vector<bool>> states, 
    std::vector<unsigned int> genPolynomials, int &timeSteps, std::vector<std::vector<vNode>> &trellis);


#endif