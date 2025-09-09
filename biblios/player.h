#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    Player();
    Player(const std::string &name, char symbol, bool isCPU = false);

    std::string getName() const;
    char getSymbol() const;
    bool isCPUPlayer() const;

    void setName(const std::string &name);

private:
    std::string name;
    char symbol;
    bool cpu;
};

#endif 