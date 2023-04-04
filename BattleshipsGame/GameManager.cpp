#include "GameManager.h"

#include <iostream>

GameManager::GameManager()
{
    Player = new PlayerController();
    Computer = new AiBehaviour();
    PlayerGrid = Player->GetGrid();
    ComputerGrid = Computer->GetGrid();
}

void GameManager::StartGame()
{
    // todo place ships Player - manually
    // todo place ships Computer - automatically
    // todo Fighting phase
    // todo Results
}

