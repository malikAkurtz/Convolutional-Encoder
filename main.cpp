#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "convViterbi.h"
#include "utility.h"
#include "genPolynomials.h"
#include "blockInterleaver.h"

int main() {
    
    float p;
    int lowerKlimit;
    int upperKlimit;
    std::string exportFile = "results.csv";

    // degree of any gen polynomial should always be less than or equal to k-1
    std::vector<std::vector<bool>> possibleStates;

    // used to get the random number between 0 and 1 when determining when to flip bits
    // unsigned int seed = 12345;  // Replace 12345 with any specific seed you want
    // srand(seed);
    srand( (unsigned)time( NULL ) );

    // the code that we want to encode
    std::string message;
    std::getline(std::cin, message);
    std::vector<bool> code = stringToVecBool(message);

    // string code = "1010";

    // string code = "1010";
    
    // the probability of ax single bit flipping after encoding the original code
    p = 0.1; // LOL
    p = 0.01; // Poor channel conditions, severe interference, or far-from-optimal signal quality.
    // p = 0.001; // Moderate noise, common in low-quality wireless connections or basic wired links with interference.
    p = 0.05;
    int numIterations = 1000;


    // if (code.length() < 50) {
    //     lowerKlimit = 4;
    //     upperKlimit = 5;
    // }
    // else if (code.length() < 100) {
    //     lowerKlimit = 5;
    //     upperKlimit = 7;
    // }
    // else {
    //     lowerKlimit = 7;
    //     upperKlimit = 8; // going above 8 will destroy your computer :)
    //}


    lowerKlimit = 4;
    upperKlimit = 10;
    
    std::map<int, std::vector<float>> k_averages;  // Adjusted to store averages for all possible_k values

    // Loop over possible values of k
    for (int possible_k = lowerKlimit; possible_k <= upperKlimit; possible_k++) {

        float average_ber = 0.0;
        float average_success_rate = 0.0;

        float ber;

        possibleStates = generateStates(possible_k);

        std::cout << "===============================================================" << std::endl;
        std::cout << "Processing for k = " << possible_k << " (" << numIterations << " iterations)" << std::endl;
        std::cout << "===============================================================" << std::endl;

        // Perform multiple iterations for each k value
        for (int i = 0; i < numIterations; i++) {
            // Reset the environment for each iteration
            int timeSteps = 0;
            std::vector<std::vector<vNode>> trellis;
 
            // Encoding, adding noise, and decoding
            std::vector<bool> encoded = encode(code, possible_k, generatorPolynomialsMap[possible_k], timeSteps);
            std::vector<bool> encoded_interleaved = interleave(encoded, possible_k);
            std::vector<bool> noisy_encoded_interleaved = addNoise(encoded_interleaved, p);

            std::vector<bool> noisy_encoded = deinterleave(noisy_encoded_interleaved, possible_k);
            std::vector<bool> originalCode = viterbiDecode(noisy_encoded, possible_k, possibleStates, generatorPolynomialsMap[possible_k], timeSteps, trellis);
            std::string originalMessage = vecBoolToString(originalCode);
            // string originalMessage = originalCode;

            // Displaying results for each iteration
            std::cout << "---------------------------------------------------------------" << std::endl;
            std::cout << "Iteration #" << i + 1 << ":" << std::endl;
            std::cout << "K = " << possible_k << std::endl;

            // print a newline after all polynomials have been printed
            std::cout << std::endl;
            std::cout << "Original Message : " << message << std::endl;
            std::cout << "Original Code  : " << vecBoolToStringBinary(code) << std::endl;
            std::cout << "Encoded Code   : " << vecBoolToStringBinary(encoded) << std::endl;
            std::cout << "Interleaved    : " << vecBoolToStringBinary(encoded_interleaved) << std::endl;
            std::cout << "Noisy Code     : " << vecBoolToStringBinary(noisy_encoded_interleaved) << std::endl;
            std::cout << "Noise Added?   : " << (noisy_encoded == encoded ? "No" : "Yes") << std::endl;
            std::cout << "# bits flipped: " << calculateHammingDistance(encoded_interleaved,noisy_encoded_interleaved) << std::endl;
            ber = ((float) calculateHammingDistance(code, originalCode) / (float) code.size());
            std::cout << "Bit Error Rate : " << ber * 100 << "%" << std::endl;
            average_ber += ber;
            std::cout << "Decoded Code   : " << vecBoolToStringBinary(originalCode) << std::endl;
            std::cout << "Decoded Message: " << originalMessage << std::endl;

            bool success = (code == originalCode);
            if (success) {
                std::cout << "Result         : SUCCESS" << std::endl;
                average_success_rate += 1;
            } else {
                std::cout << "Result         : FAIL" << std::endl;
            }
            std::cout << "---------------------------------------------------------------" << std::endl;
        }

        // Calculate and display the success rate for this k value
        average_ber /= numIterations;
        average_success_rate /= numIterations;
        k_averages[possible_k].push_back(average_ber);
        k_averages[possible_k].push_back(average_success_rate);

        std::cout << "===============================================================" << std::endl;
        std::cout << "Summary for k = " << possible_k << ":" << std::endl;
        std::cout << "Average Success Rate: " << average_success_rate * 100 << "% Success Rate" << std::endl;
        std::cout << "Average BER: " << average_ber * 100 << "% Bit Error Rate" << std::endl;
        std::cout << "===============================================================" << std::endl << std::endl;
    }

    // Final summary of all k values
    std::cout << "===================== Overall Results =====================" << std::endl;
    std::cout << "Noise was: " << p << std::endl;
    std::cout << "Original message was: " << message << std::endl;
    std::cout << "Message length was: " << code.size() << std::endl;
    // cout << "Output Bits per input: " << outputBits << endl;
    for (int i = lowerKlimit; i <= upperKlimit; i++) {
        std::cout << "For k = " << i << 
        " -> Average BER = " << std::fixed << std::setprecision(2) << k_averages[i][0] * 100 << "%" << 
        " -> Average Success Rate = " << k_averages[i][1] * 100 << "%" << std::endl;
    }
    std::cout << "===========================================================" << std::endl;

    exportData(k_averages, exportFile);
    return 0;
}
