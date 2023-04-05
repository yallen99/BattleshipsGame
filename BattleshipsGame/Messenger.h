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
};

/**
 * \brief Utility class used to handle all
 * the string checks and messages.
 * Use inline definitions, no cpp
 */
class Messenger
{
private:
    string GetSanitisedInput(const string& input);
    
public:
    map<EChecksRegex, string> RegexRuleMap;
    
    Messenger()
    {
         RegexRuleMap =
         {
             { CellCheck, "[A-Z][0-9]" },
             { OrientationCheckHorizontal, "HORIZONTAL|H" },
             { OrientationCheckHorizontal, "VERTICAL|V" },
         };   
    }

    // Print 
    void Print(const string& message);

    // Tools
    /**
     * \brief Check if the input matches the syntax of the queried string
     * based on a regex rule
     */
    bool IsInputValid(const string& input, const EChecksRegex& checkRule);
    /**
     * \return { row, column } 
     */
    pair<unsigned, unsigned> InputCellToCoordinates(const string& input);
    EOrientation InputToOrientation(const string& input);
};

inline string Messenger::GetSanitisedInput(const string& input)
{
    string sanitisedString;
    for (auto chr : input)
    {
        chr = toupper(chr); 
        sanitisedString.push_back(chr);
    }
    return sanitisedString;
}

inline void Messenger::Print(const string& message)
{
   cout << message << endl;
}

inline bool Messenger::IsInputValid(const string& input, const EChecksRegex& checkRule)
{
    const string sanitisedInput = GetSanitisedInput(input);
    const regex rule(RegexRuleMap[checkRule]);
    return regex_match(sanitisedInput, rule);
}

inline pair<unsigned, unsigned> Messenger::InputCellToCoordinates(const string& input)
{
    if(input.size() != 2) return {InvalidCellIndex, InvalidCellIndex};

    // get row
    const string sanitisedInput = GetSanitisedInput(input);
    const char row = sanitisedInput[0] - 65; // ASCII capital letters start at 65. -65 resets the counter to 0

    // get column
    char column = sanitisedInput[1] - 49; // ASCII numerics start at 48. -49 resets the counter to 0

    return { row, column };
}

inline EOrientation Messenger::InputToOrientation(const string& input)
{
    const string sanitisedInput = GetSanitisedInput(input);
    const regex ruleHorizontal(RegexRuleMap[OrientationCheckHorizontal]);
    return regex_match(sanitisedInput, ruleHorizontal) ? Horizontal : Vertical;
}


