#pragma once
#include "Grid.h"

using namespace std;

class GameManager
{
private:
    // -- Private members --//
    Grid* DemoGrid = nullptr;
    
    // -- Private functions --//
    
public:
    // -- Constructor --//
    GameManager();

    // -- Public functions --//
    void Start() const;
};
