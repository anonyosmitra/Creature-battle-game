#pragma once
#include "creature.hpp"
struct enemy{
    std::string name;
    std::vector<creature*> pool;
    creature* currentCreature= nullptr;
    enemy(const std::string name){
        this->name=name;
    }

    bool chooseCreature() {//false=has no alive creatures, true=random creature selected
        std::vector<creature*> alive=gameFuns::getLive(&pool);
        if(alive.empty())
            return false;
        currentCreature = alive.at(randoms::randomInRange(alive.size()));
        return true;
    }

    void setLevel(int l) {//set level to match player, give random exp points;
        for(creature* c:pool){
            if(l-2>=0)
                c->level= randoms::randomInRange(l-2,l+3);
            else
                c->level= randoms::randomInRange(0,l+3);
            c->exp= randoms::randomInRange(0,9);
        }
    }
};