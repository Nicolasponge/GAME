#include "board.h"
#include <iostream>
#include <sstream>

using namespace std;

Board::Board() {
    reset();
}

void Board::reset() {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            cells[r][c] = ' ';
}

bool Board::setCell(int row, int col, char symbol) {
    if (row < 0 || row >= 3 || col < 0 || col >= 3) return false;
    if (cells[row][col] != ' ') return false;
    cells[row][col] = symbol;
    return true;
}

char Board::getCell(int row, int col) const {
    if (row < 0 || row >= 3 || col < 0 || col >= 3) return '?';
    return cells[row][col];
}

bool Board::isFull() const {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (cells[r][c] == ' ') return false;
    return true;
}

bool Board::checkWin(char s) const {

    for (int r = 0; r < 3; ++r)
        if (cells[r][0] == s && cells[r][1] == s && cells[r][2] == s) return true;
    
    for (int c = 0; c < 3; ++c)
        if (cells[0][c] == s && cells[1][c] == s && cells[2][c] == s) return true;
    //
    if (cells[0][0] == s && cells[1][1] == s && cells[2][2] == s) return true;
    if (cells[0][2] == s && cells[1][1] == s && cells[2][0] == s) return true;
    return false;
}

void Board::print() const {
    cout << "\n   0   1   2\n";
    for (int r = 0; r < 3; ++r) {
        cout << r << "  ";
        for (int c = 0; c < 3; ++c) {
            cout << cells[r][c];
            if (c < 2) cout << " | ";
        }
        cout << "\n";
        if (r < 2) cout << "  ---+---+---\n";
    }
    cout << "\n";
}

string Board::serialize() const {
    string s;
    s.reserve(9);
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            s.push_back(cells[r][c]);
    return s;
}

bool Board::deserialize(const string &data) {
    if (data.size() != 9) return false;
    for (int i = 0; i < 9; ++i) {
        char ch = data[i];
        if (ch != 'X' && ch != 'O' && ch != ' ') return false;
        int r = i / 3;
        int c = i % 3;
        cells[r][c] = ch;
    }
    return true;
}