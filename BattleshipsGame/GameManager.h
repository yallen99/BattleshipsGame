﻿#pragma once
#include "AiBehaviour.h"
#include "Grid.h"
#include "StringChecker.h"
#include "PlayerController.h"

using namespace std;

enum EGamePhase
{
    GameModeSelect,
    Placement,
    Attack,
    End
};


class GameManager
{
private: 
    PlayerController* Player = nullptr;
    AiBehaviour* Computer = nullptr;
    Grid* PlayerGrid = nullptr;
    Grid* ComputerGrid = nullptr;
    StringChecker MessengerTool;
    EGamePhase Phase = GameModeSelect;
    EDifficulty Difficulty; 

    // Game Mode Select
    void SelectMode();
    
    // Placement phase
    void PlaceShipsPlayer();
    void PlaceShipsComputer();
    
    pair<unsigned, unsigned> TryGetCoordinatesFromInput();
    EOrientation TryGetOrientationFromInput();
    /**
     * \brief Cache the occupied cells to make sure the ships are not overlapping.
     * Check for out of bounds or occupied cells
     * \return true if the ship was placed successfully
     */
    bool TryPlaceShip(const unsigned& row,
                      const unsigned& column,
                      const EOrientation& orientation,
                      Ship& ship,
                      PlayerController& controller,
                      bool isPlayer   // flag to distinguish between manual and automatic placement
    );

    // Attack Phase
    void DoPlayerAttack();
    void DoComputerAttack();
    
    void RegisterHit(Cell* hitCell, PlayerController& opponent, bool isPlayer);
    string GetHealthStringDisplay(const unsigned& health);
public:
    // -- Constructor --//
    GameManager();

    void StartGame();
    bool IsGameOver() const;
};
