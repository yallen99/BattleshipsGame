#include "GameManager.h"

#include <iostream>

// todo Fighting phase
// todo Results

GameManager::GameManager()
{
    Player = new PlayerController();
    Computer = new AiBehaviour();
    PlayerGrid = Player->GetGrid();
    ComputerGrid = Computer->GetGrid();
    MessengerTool = StringChecker();
}

// ------------ GAME LOOP ------------ // 
void GameManager::StartGame()
{
    cout << "Welcome to Battleships!" << endl;
    // Generate a seed for the random generators
    srand(static_cast<int>(time(nullptr)));  // NOLINT(cert-msc51-cpp, clang-diagnostic-shorten-64-to-32)
    
    // Placement Phase
    do
    {
        PlaceShipsPlayer();
        PlaceShipsComputer();

        cout << endl << endl << "                        FIGHT!" << endl;
        Grid::DrawBoard(*PlayerGrid, *ComputerGrid);

        Phase = Attack;
        
    } while(Phase == Placement);
   
    // Attack Phase
    do
    {
        DoPlayerAttack();
        Grid::DrawBoard(*PlayerGrid, *ComputerGrid);
        DoComputerAttack();
        Grid::DrawBoard(*PlayerGrid, *ComputerGrid);

        if(IsGameOver()) Phase = End;
    }
    while(Phase == Attack);
}



// ------------ PLACEMENT ------------ //

void GameManager::PlaceShipsPlayer()
{
    PlayerGrid->DrawGrid();
    const unsigned shipsCount = static_cast<unsigned>(Player->GetShipsOwned().size());
    unsigned unplacedShips = shipsCount;
    
    // Loop while there are still unplaced ships
    do
    {
        Ship& currentShip = *Player->GetShipsOwned()[shipsCount - unplacedShips];

        cout << "Now placing ship - " + currentShip.GetShipName() << endl;
        
        // Input the Cell where to place the ship from (ships are placed left to right or top to bottom)
        cout << "Enter slot (ie. A3):" << endl;
        const pair<unsigned, unsigned> cell = TryGetCoordinatesFromInput();
        const unsigned row = cell.first;
        const unsigned column = cell.second;
        if(row == InvalidCellIndex || column == InvalidCellIndex) continue;
        
        // Input the orientation (vertical / horizontal)
        cout << "Enter orientation (H / Horizontal or V / Vertical):" << endl;
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
    const unsigned shipsCount = static_cast<unsigned>(Computer->GetShipsOwned().size()); 
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
        cout << ("Cell position is invalid!");
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
        cout << ("Cell orientation is invalid!");
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
            if(isPlayer) cout << ("Invalid cell position.");
            return false;
        }

        // Check if the ship is overlapping other ships
        if(count(controller.GetCellsOccupied().begin(), controller.GetCellsOccupied().end(), selectedCellSpace))
        {
            if(isPlayer) cout << ("Cannot overlap ships.");
            return false;
        }

        // Check if the ship is going over the bounds
        if(!gridController.IsCellWithinBounds(
            selectedCellSpace->GetCoordinates().first,
            selectedCellSpace->GetCoordinates().second,
            PlayableGridSize, PlayableGridSize))
        {
            if(isPlayer) cout << ("Cannot place ship out of bounds of the grid.");
            return false;
        }

        // Place the ship if all of the above are valid
        occupiedCellsByCurrentShip.emplace_back(selectedCellSpace);
    }

    // Register the occupied cells from cache
    for (Cell* cachedCell : occupiedCellsByCurrentShip)
    {
        ship.AddCell(cachedCell);
        cachedCell->SetState(isPlayer ? Full : Debug); // todo set Debug to Hidden. Debug purposes only
        controller.AddOccupiedCell(cachedCell);
    }

    // Send successful feedback to player only
    if(isPlayer)
    {
        cout << "Ship placed successfully!" << endl;
        gridController.DrawGrid();
    }
    return true;
}

// ------------ ATTACK ------------ //

void GameManager::DoPlayerAttack()
{   // Number of turns the player can take at once
    // For now, is 1
    int turnsAllowedPlayer = 1;
    do
    {
        cout << endl;
        cout << "Enter slot to attack (ie. A3):";

        const pair<unsigned, unsigned> cell = TryGetCoordinatesFromInput();
        const unsigned row = cell.first;
        const unsigned column = cell.second;

        // Check if the cell is invalid
        if(row == InvalidCellIndex && column == InvalidCellIndex)
        {
            cout << "Invalid cell position";
            continue;
        }

        // Check if the cell is already been hit
        Cell* triedCell = ComputerGrid->GetCellAt(row, column);
        const ECellState triedCellState = triedCell->GetState();
        if(triedCellState == Hit || triedCellState == Miss)// || triedCellState == Sunk)
        {
            cout << "You cannot hit the same cell twice!";
            continue;
        }
        
        RegisterHit(triedCell, *Computer, true);
        
        turnsAllowedPlayer--;
    } while (turnsAllowedPlayer > 0);
}

void GameManager::DoComputerAttack()
{
    // Number of turns the player can take at once
    // For now, is 1
    int turnsAllowedComputer = 1;

    do
    {
        const pair<unsigned, unsigned> coordinates = Computer->GetRandomCell();
        // Check if the cell is invalid
        if(coordinates.first == InvalidCellIndex && coordinates.second == InvalidCellIndex)
        {
            continue;
        }

        Cell* cell = PlayerGrid->GetCellAt(coordinates.first, coordinates.second);
        RegisterHit(cell, *Player, false);
        
        turnsAllowedComputer--;
    } while(turnsAllowedComputer > 0);
}

void GameManager::RegisterHit(Cell* hitCell, PlayerController& opponent, bool isPlayer)
{
    //todo [ stretch goal ] add difficulty here?
    
    // Update the hit cell state
    hitCell->SetState(hitCell->GetState() == Full // is it a player grid cell?
        ? Hit
        : hitCell->GetState() == Hidden || hitCell->GetState() == Debug // todo remove debug
            ? Hit   // is it a computer grid cell?
            : Miss); // is it an empty cell, anyway?

    hitCell->GetState() == Hit ? cout << "HIT !" << endl : cout << "MISS !" << endl;

    // If we hit something, decrease the ship's life
    if(hitCell->GetState() == Hit)
    {
        Ship& hitShip = *opponent.GetShipFromCell(hitCell);
        hitShip.DamageShip();
        opponent.DamagePlayer(1);
    }
}

bool GameManager::IsGameOver() const
{
    if(Player->GetHealth() != 0 && Computer->GetHealth() != 0 ) return false;
    Player->GetHealth() == 0
    ? cout << "*** COMPUTER WINS! ***" << endl
    : cout << "*** PLAYER WINS! ***" << endl;
    return true;
}





