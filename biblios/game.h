#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"

class Game {
public:
    Game();
    void start();
    void saveToFile(const char *filename) const;
    bool loadFromFile(const char *filename);
private:
    Board board;
    Player p1, p2;
    bool p1Turn;

    int hist_r[9];
    int hist_c[9];
    char hist_sym[9];
    int histSize;

    void mainMenu();
    void playLoop();
    void playerTurn(const Player &p);
    void cpuTurn(const Player &p);
    int minimax(Board &b, char playerSymbol, char maximizingSymbol);
    int evaluateBoard(const Board &b, char aiSym, char humanSym);
    void pushHistory(int r, int c, char s);
    bool undoLast();
    void printHeader() const;
};

#endif 