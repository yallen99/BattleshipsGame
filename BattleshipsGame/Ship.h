#pragma once
#include "Grid.h"
#include "StaticValues.h"

#include <string>

using namespace std;
/**
 * \brief Handles the ship properties. Does not need a .cpp file
 */
class Ship
{
private:
    string ShipName;
    unsigned Health;
    vector<Cell*> CellsOccupied;
    
    bool IsShipSunk() const;

public:
    // -- Constructor --//
    Ship(const string& name, const unsigned& hp)
    {
        ShipName = name;
        Health = hp;
        CellsOccupied.reserve(MaxShipPoints);
    }

    // -- Getters & Setters -- //
    unsigned GetHealth () const { return Health; }
    string GetShipName() const { return ShipName; }
    vector<Cell*>& GetCellsOccupied() { return CellsOccupied; }
    
    void AddCell(Cell* cell);
    void DamageShip();
};
