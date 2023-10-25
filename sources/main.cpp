
#include "Game.hpp"

box::game_impl app;

int main(int c, const char* v[]) 
{
    app.setup(v, c);
    app.run();
    return 0;
}
