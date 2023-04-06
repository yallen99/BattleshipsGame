#include "AiBehaviour.h"

pair<unsigned, unsigned> AiBehaviour::GetRandomCell() const
{
    const unsigned randRow = rand() % GetGrid()->GetRows();
    const unsigned randColumn = rand() % GetGrid()->GetColumns();

    return {randRow, randColumn};
}
