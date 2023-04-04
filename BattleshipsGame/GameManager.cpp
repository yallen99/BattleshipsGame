#include "GameManager.h"

#include <iostream>

void GameManager::Start() const
{
    PrintMessage("Hello");
    
}

void GameManager::PrintMessage(const std::string& message) const
{
    cout << message << " ";
}
