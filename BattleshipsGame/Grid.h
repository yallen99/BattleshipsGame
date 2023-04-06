#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>
using namespace std;

enum ECellState : uint8_t
{
    None,
    Hit,
    Miss,
    Full,
    Hidden,
    Sunk
};

enum EOrientation : uint8_t
{
    Horizontal = 0,
    Vertical = 1,
    Invalid,
};

/**
 * \brief The Grid is made of these Cells
 */
struct Cell
{
private:
    // -- Members -- //
    ECellState State = None;
    unsigned RowIndex = 0;
    unsigned ColumnIndex = 0;
    
public:
    // -- Constructor -- //
    Cell(const unsigned& r, const unsigned& c)
    {
        RowIndex = r;
        ColumnIndex = c;
    }

    // -- Getters & Setters -- //
    
    /**
     * \brief Get the pair of the coordinates of this cell
     * \return row, column
     */
    pair<unsigned, unsigned> GetCoordinates()
    {
        return {RowIndex, ColumnIndex};
    }

    ECellState GetState() const { return State; }
    void SetState(const ECellState& state) {State = state; }
};

/**
 * \brief Class handling the drawing and updating of the Grid.
 * The Grid is drew one LINE at a time ( horizontally )
 */
class Grid {
private:
    // -- Private members -- //
    unsigned Rows = 0;
    unsigned Columns = 0;
    
     vector<Cell*> Cells{};

    
    // -- Private functions -- //
    // Functions to draw the grid in the console
    static void DrawGridCorner();
    static void DrawColumnNumber(const unsigned& c);
    static void DrawRowIndex(const unsigned& r);
    static void DrawCell(const Cell& cell);
    static void DrawOneLine(const unsigned& r, const unsigned& columnCount, const Grid& grid);
    
public:
    Grid() = default;
    Grid(const unsigned& rows, const unsigned& columns);
    
    // -- Getters & Setters -- //
    unsigned GetRows() const { return Rows; }
    unsigned GetColumns() const { return Columns; }
    Cell* GetCellAt(const unsigned& row, const unsigned& column);

    // -- Public functions --//
    /**
     * \brief Populate the Cells vector
     */
    void GenerateCells();
    
    /**
     * \brief Print the grid in the console
     */
    void DrawGrid();
    
    // -- Static functions --//

    /**
     * \brief Prints the double Grids for 2 players in the console, side by side
     * \param PlayerOneGrid the grid on the left
     * \param PlayerTwoGrid the grid on the right
     */
    static void DrawBoard(const Grid& PlayerOneGrid, const Grid& PlayerTwoGrid);

    /**
     * \brief Checks if a cell is within the bounds of a grid
     * \param row the current row of the checked cell
     * \param column the current column of the checked cell
     *  \param rowBound max column index in the grid
     * \param columnBound max column index in the grid
     * \return true if the cell is within the grid bounds
     */
    bool IsCellWithinBounds(
        const unsigned& row,
        const unsigned& column,
        const unsigned& rowBound,
        const unsigned& columnBound);
};

static map<const ECellState, string> CellStateMap = {
    {None, "[ ]"},
    {Hit, "[X]"},
    {Miss, "[O]"},
    {Full, "[#]"},
    {Hidden, "[ ]"},
    {Sunk, "[~]"}
};