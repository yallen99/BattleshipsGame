#pragma once
#include "Grid.h"

#include <iostream>
#include <map>
#include <regex>
#include <string>

using namespace std;

enum EChecksRegex
{
    CellCheck,
    OrientationCheckHorizontal,
    OrientationCheckVertical,
    RestartGame,
    GameDifficultyEasy,
    GameDifficultyMedium,
    GameDifficultyHard,
    QuitGame
};

/**
 * \brief Utility class used to handle all
 * the string checks and messages.
 * Use inline definitions, no cpp
 */
class StringChecker
{
private:
    string GetSanitisedInput(const string& input) const;
    
public:
    map<EChecksRegex, string> RegexRuleMap;
    
    StringChecker()
    {
         RegexRuleMap =
         {
             { CellCheck, "[A-Z][0-9]" },
             { OrientationCheckHorizontal, "HORIZONTAL|H" },
             { OrientationCheckVertical, "VERTICAL|V" },
             { GameDifficultyEasy, "E" },
             { GameDifficultyMedium, "M" },
             { GameDifficultyHard, "H" },
             { RestartGame, "R" },
             { QuitGame, "X" },
         };   
    }

    // Tools
    /**
     * \brief Check if the input matches the syntax of the queried string
     * based on a regex rule
     */
    bool IsInputValid(const string& input, const EChecksRegex& checkRule);
    /**
     * \return { row, column } 
     */
    pair<unsigned, unsigned> InputCellToCoordinates(const string& input) const;
    EOrientation InputToOrientation(const string& input);
    int InputToDifficulty(const string& input);
};

inline string StringChecker::GetSanitisedInput(const string& input) const
{
    string sanitisedString;
    for (auto chr : input)
    {
        chr = toupper(chr); 
        sanitisedString.push_back(chr);
    }
    return sanitisedString;
}

inline bool StringChecker::IsInputValid(const string& input, const EChecksRegex& checkRule)
{
    const string sanitisedInput = GetSanitisedInput(input);
    const regex rule(RegexRuleMap[checkRule]);
    return regex_match(sanitisedInput, rule);
}

inline pair<unsigned, unsigned> StringChecker::InputCellToCoordinates(const string& input) const
{
    if(input.size() != 2) return {InvalidCellIndex, InvalidCellIndex};

    // get row
    const string sanitisedInput = GetSanitisedInput(input);
    const char row = sanitisedInput[0] - 65; // ASCII capital letters start at 65. -65 resets the counter to 0

    // get column
    char column = sanitisedInput[1] - 49; // ASCII numerics start at 48. -49 resets the counter to 0

    return { row, column };
}

inline EOrientation StringChecker::InputToOrientation(const string& input)
{
    const string sanitisedInput = GetSanitisedInput(input);
    const regex ruleHorizontal(RegexRuleMap[OrientationCheckHorizontal]);
    const regex ruleVertical(RegexRuleMap[OrientationCheckVertical]);
    return regex_match(sanitisedInput, ruleHorizontal)
    ? Horizontal
    : regex_match(sanitisedInput, ruleVertical)
        ? Vertical : Invalid;
}

inline int StringChecker::InputToDifficulty(const string& input)
{
    const string sanitisedInput = GetSanitisedInput(input);
    const regex ruleEasy(RegexRuleMap[GameDifficultyEasy]);
    const regex ruleModerate(RegexRuleMap[GameDifficultyMedium]);
    const regex ruleHard(RegexRuleMap[GameDifficultyHard]);
    return regex_match(sanitisedInput, ruleEasy)
    ? 1 : regex_match(sanitisedInput, ruleModerate)
        ? 2 : regex_match(sanitisedInput, ruleHard)
            ? 3 : 0;
}