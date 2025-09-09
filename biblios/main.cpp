#include "game.h"
#include <iostream>

using namespace std;

int main() {
    Game g;
    try {
        g.start();
    } catch (int) {
        
        g.start();
    }
    return 0;
}