#ifndef BOARD_H
#define BOARD_H

#include <string>

class Board {
public:
    Board();
    void reset();
    bool setCell(int row, int col, char symbol); 
    char getCell(int row, int col) const;
    bool isFull() const;
    bool checkWin(char symbol) const;
    void print() const;
    std::string serialize() const; 
    bool deserialize(const std::string &data); 
private:
    char cells[3][3];
};

#endif