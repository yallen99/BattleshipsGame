# include "Ship.h"

bool Ship::IsShipSunk() const
{
    return Health == 0;
}

void Ship::AddCell(Cell* cell)
{
    CellsOccupied.emplace_back(cell);
}

void Ship::DamageShip()
{
    if(Health == 0) return;
    Health--;
    if(IsShipSunk())
    {
        for (Cell* cell : CellsOccupied)
        {
            cell->SetState(Sunk);
        }
    }
}
