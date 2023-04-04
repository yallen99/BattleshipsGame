#pragma once
#include "Grid.h"
#include "Ship.h"

#include <map>
#include <string>
#include <vector>

using namespace std;

/**
 * \brief Playable base for all entities (player or AI)
 */
class PlayerController
{
private:
    map<string, unsigned> ShipsHealthMap =
        {
            {"Carrier", 5},
            {"Battleship", 4},
            {"Cruiser", 3},
            {"Submarine", 2},
            {"Destroyer", 2}
        };
    
    vector<Cell*> CellsOccupiedByPlayer;
    vector<Ship*> ShipsOwned;
    Grid* Grid = nullptr;
    
    // todo might not need this
    unsigned PlayerHealth = 0;
    
public:
    // -- Constructor -- //
    PlayerController();

    // -- Getters & Setters -- //
    unsigned GetPlayerHealth() const { return PlayerHealth; }
    vector<Ship*>& GetShipsOwned() { return ShipsOwned; }
    vector<Cell*>& GetCellsOccupied() { return CellsOccupiedByPlayer; }
    class Grid* GetGrid() const { return Grid; }

    /**
     * \brief Find and return a Ship that overlaps on a given Cell
     */
    Ship* GetShipFromCell(Cell* cell) const;

    // todo might not need this
    void DamagePlayer(unsigned damageValue);
};
