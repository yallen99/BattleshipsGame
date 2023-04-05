#include "GameManager.h"

#include <iostream>

// todo place ships Computer - automatically
// todo Fighting phase
// todo Results

GameManager::GameManager()
{
    Player = new PlayerController();
    Computer = new AiBehaviour();
    PlayerGrid = Player->GetGrid();
    ComputerGrid = Computer->GetGrid();
    MessengerTool = Messenger();
}

// ------------ GAME LOOP ------------ // 
void GameManager::StartGame()
{
    MessengerTool.Print("Welcome to Battleships!");
    PlaceShipsPlayer();
    PlaceShipsComputer();
}

// ------------ PLACEMENT ------------ // 
void GameManager::PlaceShipsPlayer()
{
    PlayerGrid->DrawGrid();
    const unsigned shipsCount = Player->GetShipsOwned().size();  // NOLINT(clang-diagnostic-shorten-64-to-32)
    unsigned unplacedShips = shipsCount;
    
    // loop while there are still unplaced ships
    do
    {
        Ship& currentShip = *Player->GetShipsOwned()[shipsCount - unplacedShips];

        MessengerTool.Print("Now placing ship - " + currentShip.GetShipName());
        
        //input the Cell where to place the ship from (ships are placed left to right or top to bottom)
        MessengerTool.Print("Enter slot (ie. A3):");
        const pair<unsigned, unsigned> cell = TryGetCoordinatesFromInput();
        const unsigned row = cell.first;
        const unsigned column = cell.second;
        if(row == InvalidCellIndex || column == InvalidCellIndex) continue;
        
        //input the orientation (vertical / horizontal)
        MessengerTool.Print("Enter orientation (H / Horizontal or V / Vertical):");
        EOrientation orientation = TryGetOrientationFromInput();
        if(orientation == Invalid) continue;

        // Try to place the requested ship
        if(!TryPlaceShip(row, column,
            orientation, currentShip,
            *Player, true)) continue;
        
        unplacedShips--;
    } while(unplacedShips > 0);
}
void GameManager::PlaceShipsComputer()
{
    cout<<" More Coming Soon!";
}

pair<unsigned, unsigned> GameManager::TryGetCoordinatesFromInput()
{
    string inputCell;
    cin >> inputCell;

    if(!MessengerTool.IsInputValid(inputCell, CellCheck))
    {
        MessengerTool.Print("Cell position is invalid!");
        return {InvalidCellIndex, InvalidCellIndex};
    }
    
    return MessengerTool.InputCellToCoordinates(inputCell);
}
EOrientation GameManager::TryGetOrientationFromInput()
{
    string inputOrientation;
    cin >> inputOrientation;
    if(!MessengerTool.IsInputValid(inputOrientation, OrientationCheckHorizontal) &&
        MessengerTool.IsInputValid(inputOrientation, OrientationCheckVertical))
    {
        MessengerTool.Print("Cell orientation is invalid!");
        return Invalid;
    }
    return MessengerTool.InputToOrientation(inputOrientation);
}

bool GameManager::TryPlaceShip(
    const unsigned& row,
    const unsigned& column,
    EOrientation& orientation,
    Ship& ship,
    PlayerController& controller,
    bool isPlayer)
{
    vector<Cell*> occupiedCellsByCurrentShip = {};
    Grid& gridController = *controller.GetGrid();

    // Checks loop - if all conditions are met, cache occupied cells
    for(unsigned c = 0; c < ship.GetHealthOrSize(); c++)
    {
        Cell* selectedCellSpace = orientation == Horizontal
        ? gridController.GetCellAt(row, column + c)
        : gridController.GetCellAt(row + c, column);

        // Check if the cell is invalid
        if(selectedCellSpace == nullptr)
        {
            if(isPlayer) MessengerTool.Print("Invalid cell position.");
            return false;
        }

        // Check if the ship is overlapping other ships
        if(count(controller.GetCellsOccupied().begin(), controller.GetCellsOccupied().end(), selectedCellSpace))
        {
            if(isPlayer) MessengerTool.Print("Cannot overlap ships.");
            return false;
        }

        // Check if the ship is going over the bounds
        if(!gridController.IsCellWithinBounds(
            selectedCellSpace->GetCoordinates().first,
            selectedCellSpace->GetCoordinates().second,
            PlayableGridSize, PlayableGridSize))
        {
            if(isPlayer) MessengerTool.Print("Cannot place ship out of bounds of the grid.");
            return false;
        }

        // Place the ship if all of the above are valid
        occupiedCellsByCurrentShip.emplace_back(selectedCellSpace);
    }

    // Register the occupied cells from cache
    for (Cell* cachedCell : occupiedCellsByCurrentShip)
    {
        ship.AddCell(cachedCell);
        cachedCell->SetState(isPlayer ? Full : Hidden);
        controller.AddOccupiedCell(cachedCell);
    }

    // Send successful feedback to player only
    if(isPlayer)
    {
        MessengerTool.Print("Ship placed successfully!");
        gridController.DrawGrid();
    }
    return true;
}





