#include "markov.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

std::string joinWords(const std::string words[], int startIndex, int count) {
    std::string result = "";

    for (int i = 0; i < count; i++) {
        result += words[startIndex + i];
        if (i < count - 1) {
            result += " ";
        }
    }

    return result;
}

int readWordsFromFile(std::string filename, std::string words[], int maxWords) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return -1;
    }

    int counter = 0;

    while (counter < maxWords && inputFile >> words[counter]) {
        counter++;
    }

    inputFile.close();
    return counter;
}

int buildMarkovChain(const std::string words[], int numWords, int order,
                     std::string prefixes[], std::string suffixes[],
                     int maxChainSize) {

    int count = 0;

    for (int i = 0; i <= numWords - order - 1; i++) {

        if (count >= maxChainSize)
            break;

        std::string prefix = joinWords(words, i, order);
        std::string suffix = words[i + order];

        prefixes[count] = prefix;
        suffixes[count] = suffix;

        count++;
    }

    return count;
}

std::string getRandomSuffix(const std::string prefixes[], const std::string suffixes[],
                            int chainSize, std::string currentPrefix) {

    int matchCount = 0;
    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            matchCount++;
        }
    }

    if (matchCount == 0)
        return "";

    int pick = rand() % matchCount;

    int currentMatch = 0;

    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            if (currentMatch == pick) {
                return suffixes[i];
            }
            currentMatch++;
        }
    }

    return "";
}

std::string getRandomPrefix(const std::string prefixes[], int chainSize) {
    if (chainSize == 0)
        return "";

    int index = rand() % chainSize;
    return prefixes[index];
}

std::string generateText(const std::string prefixes[], const std::string suffixes[],
                         int chainSize, int order, int numWords) {

    if (chainSize == 0)
        return "";

    std::string currentPrefix = getRandomPrefix(prefixes, chainSize);
    std::string result = currentPrefix;
    std::string lastWords[3];
    int wordCount = 0;
    std::string temp = currentPrefix;
    std::string word;

    for (int i = 0; i < order; i++) {
        size_t pos = temp.find(" ");
        if (pos == std::string::npos) {
            lastWords[i] = temp;
            break;
        } else {
            lastWords[i] = temp.substr(0, pos);
            temp = temp.substr(pos + 1);
        }
    }

    for (int i = 0; i < numWords; i++) {

        std::string newWord = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);

        if (newWord == "")
            break;

        result += " " + newWord;

        if (order == 1) {
            currentPrefix = newWord;
        } else {
            for (int j = 0; j < order - 1; j++) {
                lastWords[j] = lastWords[j + 1];
            }
            lastWords[order - 1] = newWord;

            currentPrefix = joinWords(lastWords, 0, order);
        }
    }

    return result;
}
