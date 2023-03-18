#include "graphics.h"
#include <queue>
#include <vector>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <fstream>

using namespace std;

const int dy[] = {-1, -1, -1, +0, +0, +1, +1, +1};
const int dx[] = {-1, +0, +1, -1, +1, -1, +0, +1};

struct level
{
    int size, bomb, yFirst, xFirst, sizeCell;
    level() {}
    level(int size, int bomb, int yFirst, int xFirst, int sizeCell)
    {
        this->size = size, this->bomb = bomb;
        this->xFirst = xFirst, this->yFirst = yFirst;
        this->sizeCell = sizeCell;
    }
} levels[4];

struct Image
{
    string name;
    int x, y, z, t, width, height;
    Image() {}
    Image(string name, int x, int y, int width, int height)
    {
        this->name = name;
        this->x = x, this->y = y;
        this->width = width, this->height = height;
        this->z = x + width - 1, this->t = y + height - 1;
    }
};

string name[2][5] = {{"new", "last", "stat", "about", "exit"}, {"begin", "inter", "expert", "custom", "home"}}; // name 
Image button[2][5][2]; // button
int smolPiece = 90, smallest = 30;

void setupPixel(int _smolPiece)
{
    smolPiece = _smolPiece, smallest = _smolPiece / 3;
    int xFirst = smolPiece * 3; // + smallest;
    int yFirst = smolPiece * 3 + smolPiece / 2;
    int w = 10 * smallest, h = 3 * smallest;
    for (int id = 0; id < 2; id++)
    {
        button[id][0][0] = (Image(name[id][0], xFirst, yFirst, w, h));
        button[id][0][1] = (Image(name[id][0] + "M", xFirst, yFirst, w, h));
    }
    for (int i = 1; i < 5; i++)
    {
        for (int id = 0; id < 2; id++)
        {
            button[id][i][0] = (Image(name[id][i], i == 4 ? 12 * smolPiece - smallest + 9: button[id][i - 1][0].x, i == 4 ? smolPiece * 2 / 3 : button[id][i - 1][0].y + h + smallest, w, h));
            button[id][i][1] = (Image(name[id][i] + "M", i == 4 ? 12 * smolPiece - smallest + 9: button[id][i - 1][1].x, i == 4 ? smolPiece * 2 / 3 : button[id][i - 1][1].y + h + smallest, w, h));
        }
    }

    levels[0] = level(9, 10, smolPiece * 9 / 2 - smolPiece * 2 / 3 * 9 / 2, smolPiece * 5 - smolPiece * 2 / 3 * 9 / 2, smolPiece * 2 / 3);
    levels[1] = level(16, 40, smolPiece * 9 / 2 - smolPiece / 2 * 8, smolPiece * 5 - smolPiece / 2 * 8, smolPiece / 2);
    levels[2] = level(25, 99, smolPiece * 9 / 2 - smolPiece / 3 * 13, smolPiece * 5 - smolPiece / 3 * 13, smolPiece / 3);
}

void printImage(Image img)
{
    const char* name = ("resources/" + img.name + ".jpg").c_str();
    readimagefile(name, img.x, img.y, img.z, img.t);
}

void drawThickRect(int x, int y, int width, int height, int COLOR = 15, int thickness = 10)
{
    setcolor(COLOR);
    int z = x + width - 1, t = y + height - 1;
    for (int i = thickness - 1; i >= 0; i--)
        rectangle(x - i, y - i, z + i, t + i);
}

bool isInside(int X, int Y, Image img)
{
    return img.x <= X && X <= img.z && img.y <= Y && Y <= img.t;
}

void clearMouse()
{
    clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_LBUTTONDBLCLK);
    clearmouseclick(WM_RBUTTONDOWN);
    clearmouseclick(WM_LBUTTONUP);
    clearmouseclick(WM_RBUTTONUP);
}

void printTime(int time, int yFirst, bool flag = false)
{
    for (int i = 3; i >= 1; i--)
    {
        Image img_time = Image("", (12 + i - 1) * smolPiece, (2 + flag) * smolPiece + smallest, smolPiece, smolPiece);
        img_time.name = char(time % 10 + '0');
        printImage(img_time);
        time /= 10;
    }
}

void about()
{
    Image Home = Image("home", 8 * smolPiece - 5 * smallest, 5 * smolPiece, 10 * smallest, 3 * smallest);
    Image HomeM = Image("homeM", 8 * smolPiece - 5 * smallest, 5 * smolPiece, 10 * smallest, 3 * smallest);
    printImage(Home); // home
    while (true)
    {
        if (!isInside(mousex(), mousey(), Home))
        {
            printImage(Home); // home
            if (ismouseclick(WM_LBUTTONDOWN) || ismouseclick(WM_LBUTTONDBLCLK) || ismouseclick(WM_RBUTTONDOWN))
                clearMouse();
        }
        else 
        {
            printImage(HomeM); // home mouse
            if (ismouseclick(WM_LBUTTONDBLCLK) || ismouseclick(WM_RBUTTONDOWN))
               clearMouse();
            else if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearMouse();
                delay(500);
                return;
            }
        }
    }
}

int menu(bool kind)
{
    clearMouse();
    for (int i = 0; i < 5; i++)
        printImage(button[kind][i][0]);

    while (true)
    {
        bool inside = false;
        for (int i = 0; i < 5; i++)
        {
            if (isInside(mousex(), mousey(), button[kind][i][0]))
            {
                inside = true;
                for (int j = 0; j < 5; j++) 
                    printImage(button[kind][j][j == i]);
                if (ismouseclick(WM_LBUTTONDOWN))
                {
                    clearMouse();
                    delay(500);
                    return i;
                }
                break;
            }
        }
        if (!inside)
        {
            for (int i = 0; i < 5; i++)
                printImage(button[kind][i][0]);
            clearMouse();
        }
    }
    return -1;
}

void printCus(int num, int xFirst, int yFirst, bool kind)
{
    string tmp; tmp += char(num / 10 + '0');
    printImage(Image(tmp, xFirst + smolPiece + smallest, yFirst + kind * (smolPiece + smallest), smolPiece, smolPiece));
    tmp = ""; tmp += char(num % 10 + '0');
    printImage(Image(tmp, xFirst + 2 * smolPiece + smallest, yFirst + kind * (smolPiece + smallest), smolPiece, smolPiece));
}

bool customLevel()
{
    printImage(button[1][4][0]);
    int size = 10, bomb = 10;
    int xFirst = smolPiece * 3; // + smallest;
    int yFirst = smolPiece * 3 + smolPiece / 2;
    printImage(Image("size", xFirst, yFirst, smolPiece, smolPiece));
    printImage(Image("bomb_grey", xFirst, yFirst + smolPiece + smallest, smolPiece, smolPiece));
    Image play = Image("play", xFirst, yFirst + 2 * smolPiece + 2 * smallest, 10 * smallest, 3 * smallest);
    printImage(play);
    printCus(size, xFirst, yFirst, 0);
    printCus(bomb, xFirst, yFirst, 1);

    int value[6] = {-1, -5, -9, +1, +5, + 9};
    Image change[2][6];
    for (int id = 0; id < 2; id++)
    {
        for (int i = 0; i < 6; i++)
        {
            string name = i < 3 ? "minus" : "plus"; name += (char)(abs(value[i]) + '0');
            int tmp = i < 3 ? - smallest - smolPiece * 2 / 3 * (i + 1) : + smallest * 11 + smolPiece * 2 / 3 * (i - 3);
            change[id][i] = Image(name, xFirst + tmp, yFirst + id * (smolPiece + smallest), smolPiece * 2 / 3, smolPiece * 2 / 3);
            printImage(change[id][i]);
        }
    }

    while (true)
    {
        if (isInside(mousex(), mousey(), play))
        {
            play.name += 'M';
            printImage(play);
            play.name.pop_back();
            if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearMouse();
                break;
            }
            continue;
        }
        else printImage(play);

        if (isInside(mousex(), mousey(), button[1][4][0]))
        {
            printImage(button[1][4][1]);
            if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearMouse();
                return false;
            }
            continue;
        }
        else printImage(button[1][4][0]);

        bool inside = false;
        for (int id = 0; id < 2; id++)
        {
            for (int i = 0; i < 6; i++)
            {
                if (isInside(mousex(), mousey(), change[id][i]))
                {
                    inside = true;
                    change[id][i].name += 'M';
                    printImage(change[id][i]);
                    change[id][i].name.pop_back();
                    for (int jd = 0; jd < 2; jd++)
                        for (int j = 0; j < 6; j++)
                            if (!(jd == id && j == i))
                                printImage(change[jd][j]);
                    if (ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearMouse();
                        if (id == 0 && 2 <= size + value[i] && size + value[i] <= 25)
                        {
                            size += value[i];
                            printCus(size, xFirst, yFirst, 0);
                            bomb = min(bomb, size * size - 1);
                        }
                        if (id == 1 && 1 <= bomb + value[i] && bomb + value[i] < size * size)
                            bomb += value[i];
                        printCus(bomb, xFirst, yFirst, 1);
                    }
                    id = i = 6;
                }
            }
        }
        
        if (!inside)
        {
            for (int id = 0; id < 2; id++)
                for (int i = 0; i < 6; i++)
                    printImage(change[id][i]);
            clearMouse();
        }
    }
    int sizeCell = size <= 10 ? smolPiece * 2 / 3 : smolPiece / (2 + (size > 16));
    levels[3] = level(size, bomb, smolPiece * 9 / 2 - sizeCell * size / 2, smolPiece * 5 - sizeCell * size / 2, sizeCell);
    return true;
}

void printScore(int num, int id, bool time)
{
    for (int i = 2 + time; i >= 1; i--)
    {
        Image img_num = Image("", (2 + i - 1) * smolPiece + (1 - time) * 4 * smolPiece, (1 + id + 1) * smolPiece, smolPiece, smolPiece);
        img_num.name = char(num % 10 + '0');
        printImage(img_num);
        num /= 10;
    }
}

void statistics(vector<pair<int, int>> timePlay)
{
    drawThickRect(smolPiece * 2, smolPiece, smolPiece * 6, smolPiece * 6);
    printImage(Image("rank", smolPiece * 2, smolPiece * 1, smolPiece * 6, smolPiece * 6));
    printImage(button[1][4][0]); // home
    if (timePlay.empty())
    {
        delay(1000);
        printImage(Image("norecord", 8 * smolPiece - smolPiece * 6, smolPiece * 3, smolPiece * 12, smolPiece * 2));
    }
    else
    {
        for (int i = 0; i < (int)timePlay.size(); i++)
        {
            printScore(timePlay[i].first, i, 1);
            clearMouse();
            printScore(timePlay[i].second, i, 0);
        }
    }
    while (true)
    {
        if (!isInside(mousex(), mousey(), button[1][4][0]))
        {
            printImage(button[1][4][0]); // home
            if (ismouseclick(WM_LBUTTONDOWN) || ismouseclick(WM_LBUTTONDBLCLK) || ismouseclick(WM_RBUTTONDOWN))
                clearMouse();
        }
        else 
        {
            printImage(button[1][4][1]); // home mouse
            if (ismouseclick(WM_LBUTTONDBLCLK) || ismouseclick(WM_RBUTTONDOWN))
               clearMouse();
            else if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearMouse();
                delay(500);
                return;
            }
        }
    }
}

void randomMine(char (&array)[50][50], int idLevel)
{
    int size = levels[idLevel].size, bomb = levels[idLevel].bomb;
    int cnt = 1;
    srand(time(NULL));
    while (cnt <= bomb)
    {
        int x = rand() % (size);
        int y = rand() % (size);
        if (array[x][y] == '0')
        {
            array[x][y] = 'B';
            ++cnt;
        }
    }
}

void init(char (&array)[50][50], char (&state)[50][50], int idLevel)
{
    level LEVEL = levels[idLevel];
    int size = LEVEL.size;
    
    for (int y = 0; y <= size - 1; y++)
    {
        for (int x = 0; x <= size - 1; x++)
        {
            array[y][x] = '0';
            state[y][x] = 'N';
        }
    }
    randomMine(array, idLevel);
    for (int y = 0; y <= size - 1; y++)
    {
        for (int x = 0; x <= size - 1; x++)
        {
            if (array[y][x] != 'B') 
                continue;
            // mark numbers around a bomb cell
            for (int id = 0; id < 8; id++)
            {
                int Y = y + dy[id];
                int X = x + dx[id];
                if (X < 0 || Y < 0 || X > size - 1 || Y > size - 1) 
                    continue;
                if (array[Y][X] != 'B')
                    array[Y][X] = (char)((int)array[Y][X] + 1);
            }   
        }
    }
}

void printFirst(char (&array)[50][50], int idLevel)
{
    level LEVEL = levels[idLevel];
    int xFirst = LEVEL.xFirst, yFirst = LEVEL.yFirst, size = LEVEL.size, sizeCell = LEVEL.sizeCell;
    drawThickRect(xFirst - 1, yFirst - 1, size * sizeCell + 2, size * sizeCell + 2, (int)(BLACK));
    Image img_quit = button[0][4][0];
    img_quit.name = "quit";
    printImage(img_quit);
    for (int y = 0; y <= size - 1; y++)
    {
        for (int x = 0; x <= size - 1; x++)
        {
            Image cell = Image("empty", xFirst + x * sizeCell, yFirst + y * sizeCell, sizeCell, sizeCell);
            printImage(cell);
        }
    }
    drawThickRect(11 * smolPiece, 2 * smolPiece + smallest, smolPiece * 4, smolPiece * 2, (int)(BLACK));
    printImage(Image("time", 11 * smolPiece, 2 * smolPiece + smallest, smolPiece, smolPiece));
    clearMouse();
    printImage(Image("flag", 11 * smolPiece, 3 * smolPiece + smallest, smolPiece, smolPiece));
}

void printFinal(char (&array)[50][50], char (&state)[50][50], int idLevel)
{
    level LEVEL = levels[idLevel];
    int xFirst = LEVEL.xFirst, yFirst = LEVEL.yFirst, size = LEVEL.size, sizeCell = LEVEL.sizeCell;

    for (int y = 0; y <= size - 1; y++)
    {
        for (int x = 0; x <= size - 1; x++)
        {
            Image cell = Image("", xFirst + x * sizeCell, yFirst + y * sizeCell, sizeCell, sizeCell);
            cell.name = "noBomb";
            if (state[y][x] != 'U')
            {
                if (array[y][x] == '0')
                    printImage(cell);
                else if (array[y][x] == 'B')
                {
                    cell.name = "bomb_grey";
                    printImage(cell);
                }
                else
                {
                    cell.name = array[y][x];
                    printImage(cell);
                }
            }
        }
    }
}

void printLastGame(char (&array)[50][50], char (&state)[50][50], int &flag, int &Unlocked, int &time, int idLevel)
{
    ifstream fin;
    fin.open("lastgame.data");
    fin >> idLevel >> time >> flag >> Unlocked;
    level LEVEL = levels[idLevel];
    fin >> LEVEL.size >> LEVEL.bomb;

    int xFirst = LEVEL.xFirst, yFirst = LEVEL.yFirst, size = LEVEL.size, sizeCell = LEVEL.sizeCell;

    drawThickRect(xFirst - 1, yFirst - 1, size * sizeCell + 2, size * sizeCell + 2, (int)(BLACK));
    Image img_quit = button[0][4][0];
    img_quit.name = "quit";
    printImage(img_quit);

    for (int y = 0; y <= size - 1; y++)
        for (int x = 0; x <= size - 1; x++)
            fin >> array[y][x];
    for (int y = 0; y <= size - 1; y++)
        for (int x = 0; x <= size - 1; x++)
            fin >> state[y][x];
    for (int y = 0; y <= size - 1; y++)
    {
        for (int x = 0; x <= size - 1; x++)
        {
            Image cell = Image("empty", xFirst + x * sizeCell, yFirst + y * sizeCell, sizeCell, sizeCell);
            if (state[y][x] == 'N');
            else if (state[y][x] == 'U')
            {
                if (array[y][x] == '0')
                    cell.name = "noBomb";
                else
                    cell.name = array[y][x];
            }
            else if (state[y][x] == 'F')
                cell.name = "flag";
            printImage(cell);
        }
    }

    drawThickRect(11 * smolPiece, 2 * smolPiece + smallest, smolPiece * 4, smolPiece * 2, (int)(BLACK));
    printImage(Image("time", 11 * smolPiece, 2 * smolPiece + smallest, smolPiece, smolPiece));
    clearMouse();
    printImage(Image("flag", 11 * smolPiece, 3 * smolPiece + smallest, smolPiece, smolPiece));

    fin.close();
}

void bfs(int Y, int X, char (&array)[50][50], char (&state)[50][50], int &Unlocked, int idLevel)
{
    level LEVEL = levels[idLevel];
    int xFirst = LEVEL.xFirst, yFirst = LEVEL.yFirst, size = LEVEL.size, sizeCell = LEVEL.sizeCell;

    queue<pair<int, int>> q;
    q.push(make_pair(Y, X));

    Image cell = Image("noBomb", xFirst + X * sizeCell, yFirst + Y * sizeCell, sizeCell, sizeCell);
    printImage(cell);

    ++Unlocked;
    state[Y][X] = 'U';

    while (!q.empty())
    {
        int y = q.front().first, x = q.front().second;
        q.pop();
        
        for (int id = 0; id < 8; id++) // spill out in 8 directions
        {
            int v = y + dy[id];
            int u = x + dx[id];

            if (v < 0 || u < 0 || v > size - 1 || u > size - 1) // out of domain
                continue;
            if (array[v][u] == 'B' || state[v][u] != 'N') // it's not a bomb, flag or uncovered already
                continue;

            cell = Image("", xFirst + u * sizeCell, yFirst + v * sizeCell, sizeCell, sizeCell);

            ++Unlocked;
            state[v][u] = 'U';
            if (array[v][u]  == '0') // not have bombs around -> continue to spill out
            {
                cell.name = "noBomb";
                printImage(cell);
                q.push(make_pair(v, u));
            }
            else // have bombs around -> print number of bombs around it and stop spilling
            {
                cell.name = array[v][u];
                printImage(cell);
            }
        }
    }
}

bool win(char (&array)[50][50], char (&state)[50][50], int idLevel)
{
    level LEVEL = levels[idLevel];
    int size = LEVEL.size;
    for (int y = 0; y <= size - 1; y++)
        for (int x = 0; x <= size - 1; x++)
        {
            if (state[y][x] == 'F' && array[y][x] != 'B')
                return false;
            if (array[y][x] == 'B' && state[y][x] != 'F')
                return false;
        }
    return true;
}

void saveGame(char (&array)[50][50], char (&state)[50][50], int &time, int &flag, int &Unlocked, int idLevel)
{
    int size = levels[idLevel].size;
    ofstream fout;
    fout.open("lastgame.data");
    fout << idLevel << ' ' << time << ' ' << flag << ' ' << Unlocked << '\n';
    fout << levels[idLevel].size << ' ' << levels[idLevel].bomb << '\n';
    for (int y = 0; y <= size - 1; y++)
    {
        for (int x = 0; x <= size - 1; x++)
            fout << array[y][x] << ' ';
        fout << '\n';
    }
    for (int y = 0; y <= size - 1; y++)
    {
        for (int x = 0; x <= size - 1; x++)
            fout << state[y][x] << ' ';
        fout << '\n';
    }
    fout.close();
}

void playing(char (&array)[50][50], int &flag, char (&state)[50][50], vector<pair<int, int> > &timePlay, int &Unlocked, int &time, int idLevel)
{
    ofstream fout;
    fout.open("lastgame.data");
    fout << "-1";
    fout.close();

    int lastTime = time;
    level LEVEL = levels[idLevel];
    flag = LEVEL.bomb;
    auto begin = std::chrono::high_resolution_clock::now(); // tính thời gian bắt đầu
    int xFirst = LEVEL.xFirst, yFirst = LEVEL.yFirst, size = LEVEL.size, sizeCell = LEVEL.sizeCell;
    int last_x = -1, last_y = -1, now_x = -1, now_y = -1;
    bool lastQuit = false;

    printTime(flag, yFirst, true);
    printTime(time, yFirst);

    Image img_border = Image("", xFirst, yFirst, size * sizeCell, size * sizeCell);
    Image img_quit = button[0][4][0]; img_quit.name = "quit";
    Image img_quitM = button[0][4][0]; img_quitM.name = "quitM";

    while (true)
    {
        // x, y like coordinates in pixel :D
        if (!isInside(mousex(), mousey(), img_border)) // not click on any cell
        {
            if (last_x != -1)
            {
                printImage(Image("empty", xFirst + last_x * sizeCell, yFirst + last_y * sizeCell, sizeCell, sizeCell));
                last_x = last_y = -1;
            }
            if (isInside(mousex(), mousey(), img_quit)) // quit
            {
                printImage(img_quitM);
                lastQuit = true;
                if (ismouseclick(WM_LBUTTONDOWN))
                {
                    clearMouse();
                    printImage(img_quit);
                    printImage(Image("seeyou", 8 * smolPiece - smolPiece * 6, smolPiece * 3, smolPiece * 12, smolPiece * 2));
                    saveGame(array, state, time, flag, Unlocked, idLevel);
                    Unlocked = 0;
                    delay(4000);
                    return;
                }
                clearMouse();
                continue;
            }
            if (ismouseclick(WM_LBUTTONDOWN) || ismouseclick(WM_LBUTTONDBLCLK) || ismouseclick(WM_RBUTTONDOWN))
                clearMouse();
            if (lastQuit)
            {
                printImage(img_quit);
                lastQuit = false;
            }
        }
        else // inside Map 
        {
            if (lastQuit)
            {
                printImage(img_quit);
                lastQuit = false;
            }
            for (int y = 0; y <= size - 1; y++)
            {
                for (int x = 0; x <= size - 1; x++)
                {
                    Image cell = Image("", xFirst + x * sizeCell, yFirst + y * sizeCell, sizeCell, sizeCell);
                    if (!isInside(mousex(), mousey(), cell)) // Mouse is not on this cell
                        continue;
                    
                    if (last_x != -1 && (last_x != x || last_y != y)) // last cell is no more Mouse on
                        printImage(Image("empty", xFirst + last_x * sizeCell, yFirst + last_y * sizeCell, sizeCell, sizeCell));
                    last_x = last_y = -1;

                    // mouse is on this cell
                    if (!ismouseclick(WM_LBUTTONDOWN) && !ismouseclick(WM_LBUTTONDBLCLK) && !ismouseclick(WM_RBUTTONDOWN))
                    {
                        if (state[y][x] == 'N')
                        {
                            cell.name = "emptyM";
                            printImage(cell);
                            last_x = x, last_y = y;
                        }
                    }
                    else if (ismouseclick(WM_LBUTTONDOWN)) // click 's'
                    {
                        clearMouse();
                        if (state[y][x] != 'N') // uncovered already :V
                            continue; 
                        if ((array[y][x] != '0') && (array[y][x] != 'B')) // this cell is a number (have bombs around)
                        {
                            cell.name = array[y][x];
                            printImage(cell);

                            state[y][x] = 'U'; // just uncover, not spill out
                            ++Unlocked;
                        }
                        else if (array[y][x] == 'B') // this cell is a bomb 
                        {
                            cell.name = "bomb_red";
                            printImage(cell);
                            state[y][x] = 'U';
                            printFinal(array, state, idLevel);
                            delay(3000);
                            printImage(Image("lose", 8 * smolPiece - smolPiece * 6, smolPiece * 3, smolPiece * 12, smolPiece * 2));
                            delay(4000);
                            Unlocked = 0;
                            return;
                        }
                        else if (array[y][x] == '0') // this cell is no Bomb around :D, start to spill out and print
                            bfs(y, x, array, state, Unlocked, idLevel);
                        x = y = size;
                        break;
                    }
                    else if (ismouseclick(WM_LBUTTONDBLCLK)) // double click to uncover when enough Flag
                    {
                        clearMouse();
                        if (state[y][x] != 'U' || array[y][x] == '0') // not uncovered yet or no bomb around :D
                            continue;
                        
                        int cntFlag = 0;
                        bool Died = false;
                        vector<pair<int, int>> different; // incorrect flags' position to print red :D

                        for (int i = -1; i <= 1; i++)
                            for (int j = -1; j <= 1; j++)
                            {
                                int Y = y + i, X = x + j;
                                if (Y < 0 || Y > size - 1 || X < 0 || X > size - 1)
                                    continue;
                                if (state[Y][X] == 'F')
                                    ++cntFlag;
                                else if (array[Y][X] == 'B') // incorrect flags' position
                                {
                                    Died = true;
                                    different.push_back(make_pair(Y, X));
                                }
                            }
                            
                        if (cntFlag != int(array[y][x]) - 48) // not enough flags to uncover 
                            continue;

                        if (Died) // Die =))) flags == bombs but incorrect flags' position
                        {
                            for (auto coordinates : different)
                            {
                                int Y = coordinates.first;
                                int X = coordinates.second;
                                state[Y][X] = 'U';
                                printImage(Image("bomb_red", xFirst + X * sizeCell, yFirst + Y * sizeCell, sizeCell, sizeCell));
                            }
                            printFinal(array, state, idLevel);
                            delay(3000);
                            printImage(Image("lose", 8 * smolPiece - smolPiece * 6, smolPiece * 3, smolPiece * 12, smolPiece * 2));
                            delay(4000);
                            Unlocked = 0;
                            return;
                        }

                        for (int i = -1; i <= 1; i++)
                        {
                            for (int j = -1; j <= 1; j++)
                            {
                                if (i == 0 && j == 0)
                                    continue;
                                int Y = y + i, X = x + j;
                                if (Y < 0 || Y > size - 1 || X < 0 || X > size - 1)
                                    continue;
                                if (state[Y][X] != 'N') // uncover already :D why we need to print again, right?
                                    continue;

                                if (array[Y][X] > '0') // have bombs around -> just uncover it, not spill out
                                {
                                    ++Unlocked;
                                    state[Y][X] = 'U';
                                    cell = Image("", xFirst + X * sizeCell, yFirst + Y * sizeCell, sizeCell, sizeCell);
                                    cell.name = array[Y][X];
                                    printImage(cell);
                                    
                                }           
                                else // no bomb around -> spill out and print
                                    bfs(Y, X, array, state, Unlocked, idLevel);
                            }
                        }
                    }
                    else if (ismouseclick(WM_RBUTTONDOWN)) // right click to flag 'f'
                    {
                        clearMouse();
                        if (state[y][x] == 'U')
                            continue;
                        if (state[y][x] == 'F')
                        {
                            --Unlocked;
                            ++flag;
                            printTime(flag, yFirst, true);
                            state[y][x] = 'N';
                        }
                        else if (state[y][x] == 'N' && flag > 0)
                        {
                            ++Unlocked;
                            --flag;
                            printTime(flag, yFirst, true);
                            state[y][x] = 'F';
                            cell.name = "flag";
                            printImage(cell);
                        }
                    }
                    x = y = size; // stop find cell
                    break;
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now(); // time at the present
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin); // time passed
        time = lastTime + (int)(elapsed.count() * 1e-9);
        if (time == 1000)
        {
            delay(3000);
            printImage(Image("runoutoftime", 8 * smolPiece - smolPiece * 6, smolPiece * 3, smolPiece * 12, smolPiece * 2));
            delay(4000);
            return;
        }
        printTime(time, yFirst);

        if (Unlocked == size * size)
        {
            if (win(array, state, idLevel)) // Win :>
            {
                delay(3000);
                printImage(Image("win", 8 * smolPiece - smolPiece * 6, smolPiece * 3, smolPiece * 12, smolPiece * 2));
                break;
            }
        }
    }

    timePlay.push_back(make_pair(time, size));
    sort(timePlay.begin(), timePlay.end());
    // just the 5 smallest time
    if ((int)timePlay.size() > 5)
        timePlay.pop_back();

    fout.open("statistics.data");
    fout << (int)timePlay.size() << '\n';
    for (pair<int, int> tmp : timePlay)
        fout << tmp.first << ' ' << tmp.second << '\n';
    fout.close();
    delay(4000);
}
