#include "HelperFunctions.hpp"
#include "Redirection.hpp"
#include "Alias.hpp"
#include "History.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

int main(int argc, char** argv) {


    History history;
    Alias alias;

    while (true) {
        showPromt();

        std::string line;
        std::getline(std::cin, line);

        // Debug
        // std::cout << "got line: " + line << std::endl;

        history.addToHistory(line);

        // exit command
        if (line == "exit") break;
        if (line == "history") {
            history.printHistory();
            continue;
        }

        auto linesSeperatedBySemiColon = splitLine(line, ';');
        for(auto& line: linesSeperatedBySemiColon) {

            parseEnviromentalVariables(line);

            std::vector<char*> lineTokens;
            makeTokens(lineTokens, line);

            if (lineTokens.size() == 1) continue; // It is just the nullptr

            //printTokens(lineTokens);

            if (alias.isAlias(lineTokens[0])) {
                auto& al = alias.getAliasMap();
                line = al[lineTokens[0]];
                lineTokens.clear();
                makeTokens(lineTokens, line);
            }

            if (alias.handle(lineTokens)) continue;

            // History + number command
            history.handleHistory(lineTokens, alias);

            // Cd command
            if (handleCd(lineTokens)) continue;

            execute(lineTokens);
        }
    }

    return 0;
}
