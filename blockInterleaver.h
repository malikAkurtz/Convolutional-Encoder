#ifndef BLOCKINTERLEAVER_H
#define BLOCKINTERLEAVER_H

#include <vector>

std::vector<bool> interleave(std::vector<bool> input, int numRows);
std::vector<bool> deinterleave(std::vector<bool> input, int k);

#endif