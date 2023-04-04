#pragma once
#include <cstdint>

/**
 * \brief Holds static numeric data for different aspects of the game
 * \n See "StaticValues.h" file
 */
enum EStaticGameValues : uint8_t
{
    MaxShipPoints = 5,
    PlayableGridSize = 7, // 7x7 grid
    InvalidCellIndex = 99
};
