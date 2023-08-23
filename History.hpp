#pragma once

#include "HelperFunctions.hpp"
#include "Alias.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <array>

class History {
public:
    History() = default;
    ~History() = default;

    void printHistory() {
        for (int i = 0; i < currentIndex; i++) {
            std::cout << i << ": " << history[i] << std::endl;
        }
    }

    void addToHistory(const std::string& line) {
        if (currentIndex == 19) currentIndex = 0;
        history[currentIndex++] = line;
    }

    void handleHistory(std::vector<char*>& lineTokens, Alias& alias) {
        if (lineTokens[1] == nullptr) return;
        if (strcmp(lineTokens[0], "history") != 0) return;
        int number = 0;

        try
        {
            number = std::stoi(lineTokens[1]);
            if (number < 0 || number > 19) return;

            auto command = history[number];
            lineTokens.clear();

            // Check if the history command is alias
            if (alias.isAlias(command)) {
                auto& al = alias.getAliasMap();
                auto line = al[command];
                parseEnviromentalVariables(line);
                makeTokens(lineTokens, line);
            }
            else {
                parseEnviromentalVariables(history[number]);
                makeTokens(lineTokens, history[number]);
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Number required for history usage\n";
            return;
        }
    }

private:
    std::array< std::string, 20> history;
    int maxSize = 20;
    int currentIndex = 0;
};
