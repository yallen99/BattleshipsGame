#pragma once
#include "AiBehaviour.h"
#include "Grid.h"
#include "StringChecker.h"
#include "PlayerController.h"

using namespace std;

enum EGamePhase
{
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
    EGamePhase Phase = Placement;
    EDifficulty Difficulty = Hard; // todo add a difficulty selector
    
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
public:
    // -- Constructor --//
    GameManager();

    void StartGame();
    bool IsGameOver() const;
};
