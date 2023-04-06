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

    // Generate a seed for the random generators
    srand(time(nullptr));  // NOLINT(cert-msc51-cpp, clang-diagnostic-shorten-64-to-32)
    PlaceShipsPlayer();
    PlaceShipsComputer();

    cout << endl;
    cout << endl;
    MessengerTool.Print("                        FIGHT!");
    cout << endl;
    Grid::DrawBoard(*PlayerGrid, *ComputerGrid);
}

// ------------ PLACEMENT ------------ // 
void GameManager::PlaceShipsPlayer()
{
    PlayerGrid->DrawGrid();
    const unsigned shipsCount = Player->GetShipsOwned().size();  // NOLINT(clang-diagnostic-shorten-64-to-32)
    unsigned unplacedShips = shipsCount;
    
    // Loop while there are still unplaced ships
    do
    {
        Ship& currentShip = *Player->GetShipsOwned()[shipsCount - unplacedShips];

        MessengerTool.Print("Now placing ship - " + currentShip.GetShipName());
        
        // Input the Cell where to place the ship from (ships are placed left to right or top to bottom)
        MessengerTool.Print("Enter slot (ie. A3):");
        const pair<unsigned, unsigned> cell = TryGetCoordinatesFromInput();
        const unsigned row = cell.first;
        const unsigned column = cell.second;
        if(row == InvalidCellIndex || column == InvalidCellIndex) continue;
        
        // Input the orientation (vertical / horizontal)
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
    const unsigned shipsCount = Computer->GetShipsOwned().size(); // NOLINT(clang-diagnostic-shorten-64-to-32)
    unsigned unplacedShips = shipsCount;
    // Loop while there are still unplaced ships
    do
    {
        Ship& currentShip = *Computer->GetShipsOwned()[shipsCount - unplacedShips];
        
        // Get a random number for the orientation (0-1)
        const unsigned orientationIndex = rand() % 2;  // NOLINT(concurrency-mt-unsafe)
        EOrientation orientation = static_cast<EOrientation>(orientationIndex);

        // Get a random cell to place the ship
        // Consider the ship size
        // +1 to include the last cells
        const unsigned randRow = orientation == Vertical
        ? rand() % (ComputerGrid->GetRows() - currentShip.GetHealthOrSize() + 1) // NOLINT(concurrency-mt-unsafe
        : rand() % (ComputerGrid->GetRows());  // NOLINT(concurrency-mt-unsafe
        
        const unsigned randColumn = orientation == Horizontal
        ? rand() % (ComputerGrid->GetColumns() - currentShip.GetHealthOrSize() + 1) // NOLINT(concurrency-mt-unsafe
        : rand() % ComputerGrid->GetColumns(); // NOLINT(concurrency-mt-unsafe

        // Try again if the positioning was unsuccessful
        if(!TryPlaceShip(randRow, randColumn, orientation, currentShip, *Computer, false))
            { continue; }
        
        unplacedShips--;
        
    } while (unplacedShips > 0);
    
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
        !MessengerTool.IsInputValid(inputOrientation, OrientationCheckVertical))
    {
        MessengerTool.Print("Cell orientation is invalid!");
        return Invalid;
    }
    return MessengerTool.InputToOrientation(inputOrientation);
}

bool GameManager::TryPlaceShip(
    const unsigned& row,
    const unsigned& column,
    const EOrientation& orientation,
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
        cachedCell->SetState(isPlayer ? Full : Sunk); // todo set Sunk to Hidden. Debug purposes only
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





