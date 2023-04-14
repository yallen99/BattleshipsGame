
#include "GameManager.h"
using namespace std;

int main()
{
    bool shouldRestart = false;
    string restartKey;
    do
    {
        GameManager game = GameManager();
        StringChecker stringChecker = StringChecker();
        game.StartGame();

        cout << "Restart [R]        Quit [X]" << endl;
        cin >> restartKey;
        
        if(stringChecker.IsInputValid(restartKey, QuitGame))
        {
             shouldRestart = false;
        }
        else if(stringChecker.IsInputValid(restartKey, RestartGame))
        {
            shouldRestart = true;
        }

    } while(shouldRestart);
    return 0;
}

// Stretch goal:
// SALVO game mode (5 vs 5 turns)
// 2 players game mode (?)