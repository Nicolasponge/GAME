#include "player.h"

using namespace std;

Player::Player() : name("Player"), symbol('X'), cpu(false) {}

Player::Player(const string &name_, char symbol_, bool isCPU) : name(name_), symbol(symbol_), cpu(isCPU) {}

string Player::getName() const { return name; }
char Player::getSymbol() const { return symbol; }
bool Player::isCPUPlayer() const { return cpu; }
void Player::setName(const string &name_) { name = name_; }