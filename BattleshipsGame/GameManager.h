#pragma once

#include <string>
using namespace std;


class GameManager
{
private:
    // -- Private members --//

    // -- Private functions --//
    
public:
    // -- Constructor --//
    GameManager() = default;

    // -- Public functions --//
    void Start() const;
    void PrintMessage(const string& message) const;
  
};
