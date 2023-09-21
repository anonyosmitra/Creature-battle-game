#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "randoms.hpp"
enum cretype{water=0,earth=1,air=2,fire=3,ice=4,steel=5};
std::vector<std::string> typeNames={"Water", "Earth", "Air", "Fire" ,"Ice","Steel"};
struct creature{
    creature(const std::string n, const std::string sN, int h, int d, cretype t) {
        name=n;
        specialName=sN;
        healthc=h;
        type=t;
        health=1;
        damage=d;
    }
    std::string name,specialName;
    int exp=0,level=0;
    float defence=0,damage,healthc,health=1;
    cretype type;
    bool defend(float force) {//true=alive,false=defeated
        if(health<=0)
            return 0;
        if(defence>0)
            std::cout<<name<<" shields the attack!\n";
        force-=defence;//softening the blow with defence(shield)
        if(force<0)
            force=0;
        health= health-force/((healthc*(100+(10*level))/100));//calculate change in health
        if(health <= 0){
            std::cout<<name<<" defeated!\n";
            health=0;}
        defence=0;
        return health > 0;
    }
    void reset(){
        health=1;
    }
    void print(){
        std::cout<<name<<", "<<health<<", "<<damage<<", "<<type<<"\n";
    }
    void levelUp(){//increase creature level
        level++;
        exp-=10;
    }
    void shield(){
        defence=healthc*(100+(10*level))/100/2;//add single use defence (50% of total health)
        exp-=5;
    }
    bool attack(creature* enemy){//true=enemy's creature defeated, false=not yet defeated
        return attack(enemy, false);
    }
    bool attack(creature* enemy, bool special){//true=enemy's creature defeated, false=not yet defeated
        exp++;//exp+1 for normal attacks
        float force=getForce(enemy->type);
        force=force*(100+(5*level))/100;//each level increases force by 5%
        if(special){
            std::cout<<name<<" uses "<<specialName<<"!\n";
            force=force*2;
            exp-=8;
        }
        if(force==0)
            return false;
        std::cout<<this->name<<" attacks "<<enemy->name<<" with force "<<force<<"\n";
        if(!enemy->defend(force)){//enemy's creature defeated
            exp+=4;//exp
            return true;
        }
        return false;
    }
    virtual float getForce(cretype &tp){std::cout<<"Last safe space\n";return 5;}

    bool turn(creature* e) {//true=defeated enemy's creature,false=not defeated
        //shield?,evolve?,(attack or special attack)
        if(exp>=10)//can afford to evolve
            if(randoms::dice(3))//1/3 chances to evolve
            {
                levelUp();
                std::cout<<name<<" leveled up to "<<level<<"\n";
            }
        if(exp>=5)//can afford to enable shield
            if(randoms::dice(5))//1/5 chances to enable shield
                shield();
        if(exp>=8)//can afford special attack
            if(randoms::dice(3))//1/3 chances for special attack
                return attack(e, true);
        return attack(e);//normal attack
    }
};
struct waterCreature:creature{
    waterCreature(const std::string n, const std::string sN, int h, int d): creature(n,sN,h,d,water) {}
    float getForce(cretype &tp) override{
        if(health<=0){
            return 0;
        }
        float force=damage;
        switch (tp) {
            case water:
                force=force*3/4;//reduce force by 25%
                break;
            case earth:
            case fire:
                force=force*5/4;//increase force by 25%
                break;
        }
        return force;
    }
};
struct earthCreature:creature{
    earthCreature(const std::string n, const std::string sN, int h, int d): creature(n,sN,h,d,earth) {}
    float getForce(cretype &tp) override{
        if(health<=0){
            return 0;
        }
        float force=damage;
        switch (tp) {
            case air:
                force=force*3/4;
                break;
            case fire:
            case steel:
            case ice:
                force=force*5/4;
                break;
        }
        return force;
    }
};
struct airCreature:creature{
    airCreature(const std::string n, const std::string sN, int h, int d): creature(n,sN,h,d,air) {}
    float getForce(cretype &tp) override{
        if(health<=0){
            return 0;
        }
        float force=damage;
        switch (tp) {
            case earth:
            case steel:
                force=force*3/4;
                break;
            case ice:
                force=force*5/4;
                break;
        }
        return force;
    }
};
struct fireCreature:creature{
    fireCreature(const std::string n, const std::string sN, int h, int d): creature(n,sN,h,d,fire) {}
    float getForce(cretype &tp) override{
        if(health<=0){
            return 0;
        }
        float force=damage;
        switch (tp) {
            case water:
            case earth:
                force=force*3/4;
                break;
            case ice:
            case steel:
                force=force*5/4;
                break;
        }
        return force;
    }
    virtual std::string getType(){return "fire";}
};
struct iceCreature:creature{
    iceCreature(const std::string n, const std::string sN, int h, int d): creature(n,sN,h,d, ice) {}
    float getForce(cretype &tp) override{
        if(health<=0){
            return 0;
        }
        float force=damage;
        switch (tp) {
            case ice:
            case water:
            case fire:
                force=force*3/4;
                break;
            case earth:
                force=force*5/4;
                break;
        }
        return force;
    }
};
struct steelCreature:creature{
    steelCreature(const std::string n, const std::string sN, int h, int d): creature(n,sN,h,d, steel) {}
    float getForce(cretype &tp) override
    {
        if(health<=0){
            return 0;
        }
        float force=damage;
        switch (tp) {
            case fire:
            case steel:
                force=force*3/4;
                break;
            case water:
            case air:
                force=force*5/4;
                break;
        }
        return force;
    }
};
namespace gameFuns {
    std::vector<creature *> getLive(std::vector<creature *> *pool) {//get all alive creatures in the vector
        std::vector<creature *> vec;
        for (creature *c: *pool)
            if (c->health > 0)
                vec.push_back(c);
        return vec;
    }

    bool hasAliveCreatures(std::vector<creature *> *pool) {//are there any alive creatures in the vector
        for (creature *c: *pool)
            if (c->health > 0)
                return true;
        return false;
    }
}