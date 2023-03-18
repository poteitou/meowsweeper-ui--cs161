#include "myFunctions.h"
using namespace std;

int main()
{
    int Width = getmaxwidth();
    int Height = getmaxheight() - GetSystemMetrics(SM_CYCAPTION);
    int smolPiece = min(90, min(Width / 16, Height / 9));
    Width = smolPiece * 16, Height = smolPiece * 9;
    initwindow(Width, Height, "Meow Sweeper OwO - Trinh Nguyen Thao Vi 22125120");

    setupPixel(smolPiece);
    vector<pair<int, int> > timePlay;
    ifstream fin;
    fin.open("statistics.data");
    int len;
    fin >> len;
    for (int i = 0; i < len; i++)
    {
        int time, size;
        fin >> time >> size;
        timePlay.push_back(make_pair(time, size));
    }
    fin.close();

    while (true)
    {
        char array[50][50], state[50][50];
        int flag = 0, Unlocked = 0, time = 0, idLevel;
        clearMouse();
        printImage(Image("background", 0, 0, Width, Height));
        int option = menu(0);

        switch(option)
        {
            case 0: // New game
                idLevel = menu(1);
                if (idLevel == 4) // Home
                    break;
                if (idLevel == 3) // Custom
                {
                    printImage(Image("background", 0, 0, Width, Height));
                    if (customLevel() == false) // Home
                        break;
                }
                init(array, state, idLevel);
                printImage(Image("backgroundblur", 0, 0, Width, Height));
                printFirst(array, idLevel);
                playing(array, flag, state, timePlay, Unlocked, time, idLevel);
                delay(500);
                break;
            case 1: // Last game
                fin.open("lastgame.data");
                fin >> idLevel;
                fin.close();
                printImage(Image("backgroundblur", 0, 0, Width, Height));
                if (idLevel == -1)
                {
                    delay(1000);
                    printImage(Image("norecord", 8 * smolPiece - smolPiece * 6, smolPiece * 3, smolPiece * 12, smolPiece * 2));
                    delay(4000);
                    break;
                }
                printLastGame(array, state, flag, Unlocked, time, idLevel);
                playing(array, flag, state, timePlay, Unlocked, time, idLevel);
                break;
            case 2: // Statistics
                printImage(Image("backgroundblur", 0, 0, Width, Height));
                statistics(timePlay);
                break;
            case 3: // About
                printImage(Image("information", 0, 0, Width, Height));
                about();
                break;
            default: // Exit
                printImage(Image("seeyou", 8 * smolPiece - smolPiece * 6, smolPiece * 3, smolPiece * 12, smolPiece * 2));
                delay(4000);
                closegraph();
                return 0;
        }
    }
    return 0;
}