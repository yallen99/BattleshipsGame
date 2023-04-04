#pragma once
#include "AiBehaviour.h"
#include "Grid.h"
#include "PlayerController.h"

using namespace std;

class GameManager
{
private:
    PlayerController* Player = nullptr;
    AiBehaviour* Computer = nullptr;
    Grid* PlayerGrid = nullptr;
    Grid* ComputerGrid = nullptr;
    
public:
    // -- Constructor --//
    GameManager();

    void StartGame();
};
