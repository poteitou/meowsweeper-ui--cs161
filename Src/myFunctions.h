#include "myFunctions.cpp"

void setupPixel(int _smolPiece);
void printImage(Image img);
void drawThickRect(int x, int y, int weight, int height, int COLOR, int thickness);
bool isInside(int X, int Y, Image img);
void clearMouse();
void printTime(int time, int yFirst, bool flag);

void about();
int menu(bool kind);
void printCus(int num, int xFirst, int yFirst, bool kind);
bool customLevel();
void printScore(int num, int id, bool time);
void statistics(vector<pair<int, int>> timePlay);

void randomMine(char (&array)[50][50], int idLevel); //random mine 
void init(char (&array)[50][50], char (&state)[50][50], int idLevel); // init and random mine
void printFirst(char (&array)[50][50], int idLevel);
void printFinal(char (&array)[50][50], char (&state)[50][50], int idLevel);
void printLastGame(char (&array)[50][50], char (&state)[50][50], int &flag, int &Unlocked, int &time, int idLevel); // init last game

void bfs(int i, int j, char (&array)[50][50], char (&state)[50][50], int &Unlocked, int idLevel);
bool win(char (&array)[50][50], char (&state)[50][50], int idLevel);
void saveGame(char (&array)[50][50], char (&state)[50][50], int &time, int &flag, int &Unlocked, int idLevel);
void playing(char (&array)[50][50], int &flag, char (&state)[50][50], vector<pair<int, int> > &timePlay, int &Unlocked, int &time, int idLevel);
