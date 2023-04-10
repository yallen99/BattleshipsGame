#pragma once
#include "Grid.h"
#include "Ship.h"

#include <map>
#include <string>
#include <vector>

using namespace std;


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
    
    unsigned PlayerHealth = 0;
    
public:
    // -- Constructor -- //
    PlayerController();

    // -- Getters & Setters -- //
    unsigned GetHealth() const { return PlayerHealth; }
    vector<Ship*>& GetShipsOwned() { return ShipsOwned; }
    vector<Cell*>& GetCellsOccupied() { return CellsOccupiedByPlayer; }
    class Grid* GetGrid() const { return Grid; }
    void AddOccupiedCell(Cell* cell) { CellsOccupiedByPlayer.emplace_back(cell); }
    
    /**
     * \brief Find and return a Ship that overlaps on a given Cell
     */
    Ship* GetShipFromCell(Cell* cell) const;

    void DamagePlayer(unsigned damageValue);
};
