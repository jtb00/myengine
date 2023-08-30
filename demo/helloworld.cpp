#include <iostream>

#include "../src/Engine.h"

//using namespace illengine;

int main( int argc, const char* argv[] ) {
    globalEngine.start();
    globalEngine.gameLoop();
    return 0;
}