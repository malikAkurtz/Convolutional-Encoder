#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <map>
#include "vNode.h"
#include "genPolynomials.h"



int calculateHammingDistance(std::vector<bool>& code1, std::vector<bool>& code2);                         
std::vector<bool> generateOutput(std::vector<bool>& shiftregister, std::vector<unsigned int>& genPolynomials); 
std::vector<bool> addNoise(std::vector<bool>& code, float prob_of_error);                                 
std::vector<bool> getOriginalCode(const std::vector<std::vector<vNode>> &trellis);                             
std::vector<std::vector<bool>> generateStates(int k);                                                     
std::vector<bool> calculatePotentialInput(std::vector<bool>& curState, bool next_input);                  
std::vector<bool> stringToVecBool(std::string& message);                                                  
std::string vecBoolToString(std::vector<bool>& binary);                                                  
void exportData(std::map<int, std::vector<float>>& k_data_points, std::string& filename);
void printTrellisStates(const std::vector<std::vector<vNode>> &trellis);
std::string vecBoolToStringBinary(std::vector<bool>& binary);
int vecBoolToInt(std::vector<bool>& bits);
std::vector<std::vector<bool>> takeTranspose(std::vector<std::vector<bool>> inputMatrix);
void printMatrix(std::vector<std::vector<bool>> matrix);
void printVecBool(std::vector<bool> vector);

#endif