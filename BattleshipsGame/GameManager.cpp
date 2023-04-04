#include "GameManager.h"
#include <iostream>

GameManager::GameManager()
{
    DemoGrid = new Grid(7, 7);
}

void GameManager::Start() const
{
    DemoGrid->GenerateCells();
    DemoGrid->DrawGrid();   
}

