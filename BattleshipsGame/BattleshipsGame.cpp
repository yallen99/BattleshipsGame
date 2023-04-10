
#include "GameManager.h"
using namespace std;

int main()
{
    bool shouldRestart;
    string restartKey;
    do
    {
        GameManager game = GameManager();
        StringChecker stringChecker = StringChecker();
        game.StartGame();

        cout << "Restart [R]        Quit [X]" << endl;
        cin >> restartKey;
        
        shouldRestart = stringChecker.IsInputValid(restartKey, QuitGame)
        ? false
        : stringChecker.IsInputValid(restartKey, RestartGame)
            ? true
            : false;
        
    } while(shouldRestart);
    return 0;
}


// Stretch goal:
// SALVO game mode (5 vs 5 turns)
// Difficulties
// 2 players game mode (?)
