#include <iostream>
#include "game.hpp"
int main(void){
    srand(time(NULL));
    //auto g=game::read();
    auto g=game();
    std::cout<<g.pool.size();
    g.start();
}