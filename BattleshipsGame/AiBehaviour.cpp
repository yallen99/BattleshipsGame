#include "AiBehaviour.h"

pair<unsigned, unsigned> AiBehaviour::GetNextCell(EDifficulty difficulty)
{
    pair<unsigned, unsigned> coordinates;
    switch (difficulty)
    {
    case Easy:
         coordinates = GetRandomCellCoordinates();
         break;
    case Moderate:
         coordinates = GetCellFromHitOnMedium();
         break;
    case Hard:
         coordinates = GetCellFromHitOnHard();
         break;
    }
    if(!IsPositionValidOnGrid(coordinates))
    {
        return {InvalidCellIndex, InvalidCellIndex};
    }
        return coordinates;
}

void AiBehaviour::SetTargetCellOnHit(const Cell& cell)
{
    HitCell = new Cell(cell);
}

void AiBehaviour::SetLastTriedCell(const Cell& cell)
{
    LastTriedCell = new Cell(cell);
}


void AiBehaviour::ResetDataOnShipSunk()
{
    HitCell = nullptr;
    OriginCell = nullptr;
    IteratorIndexHard = 0;
}

pair<unsigned, unsigned> AiBehaviour::GetRandomCellCoordinates() const
{
    const unsigned randRow = rand() % GetGrid()->GetRows();
    const unsigned randColumn = rand() % GetGrid()->GetColumns();

    return {randRow, randColumn};
}

pair<unsigned, unsigned> AiBehaviour::GetFilteredRandomCellCoordinates(const pair<unsigned, unsigned> filterCoordinates) const
{
    Movement movement = Movement();
    int invalidNeighboursCount = 0;
    Cell& originCell = *OpponentGrid->GetCellAt(filterCoordinates.first, filterCoordinates.second);
    for(auto& rule : MovementRuleSheet)
    {
        // Apply the rule from the sheet to the movement and get the cell
        rule(movement);
        pair<unsigned, unsigned> neighbourCoordinates = GetCellCoordinatesWithMovement(movement, originCell);

        // Check if the cell is out of bounds
        if(!IsPositionValidOnGrid(neighbourCoordinates))
        {
            invalidNeighboursCount ++;
            continue;
        }

        // Check if the cell was previously hit
        const Cell& neighbourCell = *OpponentGrid->GetCellAt(neighbourCoordinates.first, neighbourCoordinates.second);
        if(neighbourCell.GetState() == Miss || neighbourCell.GetState() == Sunk || neighbourCell.GetState() == Hit)
        {
            invalidNeighboursCount++;
        }
    }
    
    // If all the possible directions are invalid or take, return invalid coords 
    return invalidNeighboursCount >= PossibleMovementDirections
        ? pair<unsigned, unsigned>{InvalidCellIndex, InvalidCellIndex}
        : filterCoordinates;
}

pair<unsigned, unsigned> AiBehaviour::GetCellCoordinatesWithMovement(const Movement& movement, Cell& originCell) const
{
    pair<unsigned, unsigned> coordinates = originCell.GetCoordinates();
    switch (movement.Orientation)
    {
    case Horizontal:
        return pair<unsigned, unsigned>{coordinates.first, coordinates.second + movement.Direction};
    case Vertical:
        return pair<unsigned, unsigned>{coordinates.first + movement.Direction, coordinates.second};
    case Invalid: break;
    }
    return pair<unsigned, unsigned>{InvalidCellIndex, InvalidCellIndex};
}

pair<unsigned, unsigned> AiBehaviour::GetCellFromHitOnMedium()
{
    // If there wasn't a successful previous hit, try a random cell
    if(HitCell == nullptr) return GetRandomCellCoordinates();
    return TryGetNextValidPosition();
}

/**
 * \brief Tries to simulate the human thinking.
 * If you hit a cell, you will try it's surroundings.
 * If you go out of the ship bounds, you'd change direction. 
 */
pair<unsigned, unsigned> AiBehaviour::GetCellFromHitOnHard()
{
    // If there wasn't a successful previous hit and the origin cell is also empty, try another random cell
    if(HitCell == nullptr && OriginCell == nullptr) return GetRandomCellCoordinates();
    
    // If there was a hit on the previous round, set the origin cell
    if(LastTriedCell->GetState() == Hit && OriginCell == nullptr)
    {
        OriginCell = new Cell(*HitCell);
    }

    // If the origin cell is set - aka, if we found a ship - compute the next move
    if(OriginCell != nullptr)
    {
        pair<unsigned, unsigned> coordinates;
        
        // Compute which cell to hit next to destroy the ship, considering if the cell is valid or not
        do
        {
            // If the last hit cell was a hit, carry on hitting with the movement rule
            // If it was a miss, change the movement rule
            coordinates = LastTriedCell->GetState() == Hit
                ? GetCellCoordinatesWithMovement(HardModeMovement, *LastTriedCell) 
                : TryGetNextValuePositionWithRule();

            // If the cell you want to hit next is not valid, change the movement rule
            if(!IsPositionValidOnGrid(coordinates))
            {
                TryGetNextValuePositionWithRule();
            }
        }
        while(!IsPositionValidOnGrid(coordinates));
        return coordinates;
    }

    // If the origin is not set, find the next available position
    const pair<unsigned, unsigned> coordinates = TryGetNextValidPosition();
    return coordinates;
}

bool AiBehaviour::IsPositionValidOnGrid(pair<unsigned, unsigned> coordinates) const
{
    // Check if the position is out of bounds
    if(!OpponentGrid->IsCellWithinBounds(coordinates.first, coordinates.second,
        PlayableGridSize, PlayableGridSize))
    {
        return false;
    }

    // Check if the cell is already been hit
    const Cell& triedCell = *OpponentGrid->GetCellAt(coordinates.first, coordinates.second);
    if(triedCell.GetState() == Hit ||triedCell.GetState() == Miss || triedCell.GetState() == Sunk)
    {
        return false;
    }

    return true;
}

pair<unsigned, unsigned> AiBehaviour::TryGetNextValidPosition()
{
    Movement movement = Movement();
    for (auto& rule : MovementRuleSheet)
    {
        rule(movement);
        const pair<unsigned, unsigned> coordinates = GetCellCoordinatesWithMovement(movement, *HitCell);
        if(IsPositionValidOnGrid(coordinates))
        {
            HitCell = nullptr;
            HardModeMovement = movement;
            return coordinates;
        }
    }

    HitCell = nullptr;
    return GetRandomCellCoordinates();
}

pair<unsigned, unsigned> AiBehaviour::TryGetNextValuePositionWithRule()
{
    IteratorIndexHard++;
    if(IteratorIndexHard < PossibleMovementDirections)
    {
        MovementRuleSheet[IteratorIndexHard](HardModeMovement);
        return GetCellCoordinatesWithMovement(HardModeMovement, *OriginCell);
    }

    // Tried all the surrounding cells - return a random cell
    IteratorIndexHard = 0;
    return GetRandomCellCoordinates();
}



