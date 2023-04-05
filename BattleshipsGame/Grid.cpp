#include "Grid.h"
#include "StaticValues.h"

#include <iostream>

using namespace std;

void Grid::DrawGridCorner()
{
    cout << "   ";
}
void Grid::DrawColumnNumber(const unsigned& c)
{
    cout << " " << c << " ";
}
void Grid::DrawRowIndex(const unsigned& r)
{
    // Capital ASCII letters start at 65
    const char rowLetter = r + 65;
    cout << " " << rowLetter << " ";
}
void Grid::DrawCell(const Cell& cell)
{
    cout << CellStateMap[cell.GetState()];
}
void Grid::DrawOneLine(const unsigned& r, const unsigned& columnCount, const Grid& grid)
{
    DrawRowIndex(r);
    for(unsigned c = 0; c < columnCount; c++)
    {
        const unsigned cellIndex = columnCount * r + c;
        DrawCell(*grid.Cells[cellIndex]);
    }
}

Grid::Grid(const unsigned& rows, const unsigned& columns)
{
    Columns = columns;
    Rows = rows;

    CellStateMap = {
        {None, "[ ]"},
        {Hit, "[X]"},
        {Miss, "[O]"},
        {Full, "[=]"},
        {Hidden, "[ ]"},
    };
}

Cell* Grid::GetCellAt(const unsigned& row, const unsigned& column) 
{
    if(row > Rows - 1 || column > Columns - 1) return nullptr;
    return *find_if(Cells.begin(), Cells.end(), [row, column] (Cell* cell)
    {
        // GetCoordinates -> { row, column }
        const pair<unsigned, unsigned> coordinates = cell->GetCoordinates();
        return coordinates.first == row && coordinates.second == column;
    });
}

void Grid::GenerateCells()
{
    for(unsigned r = 0; r < Rows; r++)
    {
        for (unsigned c = 0; c < Columns; c++)
        {
            Cells.emplace_back(new Cell(r, c));
        }
    }
}

void Grid::DrawGrid() 
{
   DrawGridCorner();
    for(unsigned c = 0; c < Columns; c++)
    {
        DrawColumnNumber(c + 1);
    }
    
    cout << endl;
    
    for(unsigned i = 0; i < Cells.size(); i++)
    {
        if(i % Rows == 0)
        {
            // If we reached the end of a line, jump on a new line
            if(i / Rows != 0) cout << endl;
            DrawRowIndex(i / Rows);
        }
        DrawCell(*Cells[i]);
    }
    cout << endl;
}

void Grid::DrawBoard(const Grid& PlayerOneGrid, const Grid& PlayerTwoGrid)
{
    // todo - draw the board for 2 players
}

bool Grid::IsCellWithinBounds(const unsigned& row,
    const unsigned& column,
    const unsigned& rowBound,
    const unsigned& columnBound)
{
    const bool cellIndexIsInBounds = column < columnBound && row < rowBound;
    const bool cellIndexIsValid = column != InvalidCellIndex && row != InvalidCellIndex;
    return cellIndexIsValid && cellIndexIsInBounds;
}
