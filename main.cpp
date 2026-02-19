#include <iostream>
#include <cstdlib>
#include <ctime>
#include "markov.h"

int main() {

    srand(time(0));

    std::string filename;
    int order;
    int numWords;

    std::cout << "Enter input filename: ";
    std::cin >> filename;

    std::cout << "Enter order (1, 2, or 3): ";
    std::cin >> order;

    std::cout << "Enter number of words to generate: ";
    std::cin >> numWords;

    const int MAX_WORDS = 100000;
    const int MAX_CHAIN = 100000;

    std::string words[MAX_WORDS];
    std::string prefixes[MAX_CHAIN];
    std::string suffixes[MAX_CHAIN];

    int wordCount = readWordsFromFile(filename, words, MAX_WORDS);

    if (wordCount == -1) {
        std::cout << "Error: Could not open file.\n";
        return 1;
    }

    int chainSize = buildMarkovChain(words, wordCount, order,
    prefixes, suffixes, MAX_CHAIN);

    if (chainSize == 0) {
        std::cout << "Error: Chain is empty.\n";
        return 1;
    }

    std::string output = generateText(prefixes, suffixes, chainSize,
    order, numWords);

    std::cout << "\nGenerated text:\n";
    std::cout << output << std::endl;

    return 0;
}
