#pragma once
#include <functional>

#include "PlayerController.h"

enum EDifficulty
{
    Easy,
    Moderate,
    Hard
};

using namespace std;
/**
 * \brief Computer class for Player 2
 */
class AiBehaviour : public PlayerController
{
private:
    /**
     * \brief Struct handling the next picked slot by AI.
     * Direction gives the horizontal / vertical axis.
     * Movement can be either 1 or -1, where 1 corresponds to up or right,
     * and -1 to down or left, based on the direction.
     */
    struct Movement
    {
        int Direction = 0;
        EOrientation Orientation = Invalid;
        Movement()
        {
            const int randomIndex = rand() % 2;
            // Random starting orientation
            Orientation = static_cast<EOrientation>(randomIndex);

            // Random starting direction
            Direction = randomIndex == 0 ? -1 : randomIndex;
        }

        void ChangeOrientation()
        {
            Orientation = (Orientation == Horizontal || Orientation == Invalid)
            ? Vertical : Horizontal;
        }

        void ChangeDirection()
        {
            Direction = Direction == 1 ? -1 : 1;
        }
        
    };

    vector<function<void(Movement& movement)>> MovementRuleSheet =
        {
            [](Movement& movement){},
            [](Movement& movement){movement.ChangeDirection();},
            [](Movement& movement){movement.ChangeOrientation();},
            [](Movement& movement){movement.ChangeDirection();},
        };

    Cell* HitCell = nullptr;

    // Params relevant for the Hard Difficulty only
    Cell* OriginCell = nullptr;
    Cell* LastTriedCell = nullptr;
    Movement HardModeMovement = Movement();
    int IteratorIndexHard = 0;
    class Grid* OpponentGrid;
    
public:
    // -- Constructor -- //
    explicit AiBehaviour(class Grid* opponentGrid)
    {
         OpponentGrid = opponentGrid;
    }

    pair<unsigned, unsigned> GetNextCell(EDifficulty difficulty);
    void SetTargetCellOnHit(const Cell& cell);
    void SetLastTriedCell(const Cell& cell);
    void ResetDataOnShipSunk();
    
private:
    pair<unsigned, unsigned> GetFilteredRandomCellCoordinates(pair<unsigned, unsigned> filterCoordinates) const;
    pair<unsigned, unsigned> GetCellCoordinatesWithMovement(const Movement& movement, Cell& originCell) const;

    pair<unsigned, unsigned> GetRandomCellCoordinates() const;
    pair<unsigned, unsigned> GetCellFromHitOnMedium();
    pair<unsigned, unsigned> GetCellFromHitOnHard();

    bool IsPositionValidOnGrid(pair<unsigned, unsigned> coordinates) const;
    pair<unsigned, unsigned> TryGetNextValidPosition();
    pair<unsigned, unsigned> TryGetNextValuePositionWithRule();
};
