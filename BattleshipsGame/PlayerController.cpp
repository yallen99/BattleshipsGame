#include "PlayerController.h"

#include <algorithm>

PlayerController::PlayerController()
{
    Grid = new class Grid(PlayableGridSize, PlayableGridSize);
    Grid->GenerateCells();

    ShipsOwned.reserve(ShipsHealthMap.size());
    for (auto const& ship : ShipsHealthMap)
    {
        ShipsOwned.emplace_back(new Ship(ship.first, ship.second));
        PlayerHealth += ship.second;
    }
}

Ship* PlayerController::GetShipFromCell(Cell* cell) const
{
    for (Ship* ship : ShipsOwned)
    {
        if(any_of(ship->GetCellsOccupied().begin(),
            ship->GetCellsOccupied().end(),
            [cell] (Cell* foundCell)
            {
                return cell->GetCoordinates().first == foundCell->GetCoordinates().first &&
                    cell->GetCoordinates().second == foundCell->GetCoordinates().second;
            }))
        {
            return ship;
        }
    }
    return nullptr;
}

void PlayerController::DamagePlayer(unsigned damageValue)
{
    if(PlayerHealth == 0) return;
    PlayerHealth --;
}


