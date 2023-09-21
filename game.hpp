#pragma once
#include "enemy.hpp"
#include <set>
#include <fstream>

std::string path="game.bin";
void makeCreatures(std::vector<waterCreature>* w, std::vector<earthCreature>* e,
                   std::vector<airCreature> *a, std::vector<iceCreature> *i,
                   std::vector<fireCreature> *f, std::vector<steelCreature> *s,
                   std::vector<creature *> *p) {
    p->clear();
    w->push_back(waterCreature("Water 1","Special Attack",20,5));
    w->push_back(waterCreature("Water 2","Special Attack",20,5));
    w->push_back(waterCreature("Water 3","Special Attack",20,5));
    w->push_back(waterCreature("Water 4","Special Attack",20,5));
    w->push_back(waterCreature("Water 5","Special Attack",20,5));
    w->push_back(waterCreature("Water 6","Special Attack",20,5));
    a->push_back(airCreature("Air 1","Special Attack",20,5));
    a->push_back(airCreature("Air 2","Special Attack",20,5));
    a->push_back(airCreature("Air 3","Special Attack",20,5));
    a->push_back(airCreature("Air 4","Special Attack",20,5));
    a->push_back(airCreature("Air 5","Special Attack",20,5));
    a->push_back(airCreature("Air 6","Special Attack",20,5));
    e->push_back(earthCreature("Earth 1","Special Attack",20,5));
    e->push_back(earthCreature("Earth 2","Special Attack",20,5));
    e->push_back(earthCreature("Earth 3","Special Attack",20,5));
    e->push_back(earthCreature("Earth 4","Special Attack",20,5));
    e->push_back(earthCreature("Earth 5","Special Attack",20,5));
    e->push_back(earthCreature("Earth 6","Special Attack",20,5));
    f->push_back(fireCreature("Fire 1","Special Attack",20,5));
    f->push_back(fireCreature("Fire 2","Special Attack",20,5));
    f->push_back(fireCreature("Fire 3","Special Attack",20,5));
    f->push_back(fireCreature("Fire 4","Special Attack",20,5));
    f->push_back(fireCreature("Fire 5","Special Attack",20,5));
    f->push_back(fireCreature("Fire 6","Special Attack",20,5));
    i->push_back(iceCreature("Ice 1","Special Attack",20,5));
    i->push_back(iceCreature("Ice 2","Special Attack",20,5));
    i->push_back(iceCreature("Ice 3","Special Attack",20,5));
    i->push_back(iceCreature("Ice 4","Special Attack",20,5));
    i->push_back(iceCreature("Ice 5","Special Attack",20,5));
    i->push_back(iceCreature("Ice 6","Special Attack",20,5));
    s->push_back(steelCreature("Steel 1","Special Attack",20,5));
    s->push_back(steelCreature("Steel 2","Special Attack",20,5));
    s->push_back(steelCreature("Steel 3","Special Attack",20,5));
    s->push_back(steelCreature("Steel 4","Special Attack",20,5));
    s->push_back(steelCreature("Steel 5","Special Attack",20,5));
    s->push_back(steelCreature("Steel 6","Special Attack",20,5));
    for(int j=0;j<w->size();j++)
        p->push_back(&w->at(j));
    for(int j=0;j<a->size();j++)
        p->push_back(&a->at(j));
    for(int j=0;j<e->size();j++)
        p->push_back(&e->at(j));
    for(int j=0;j<f->size();j++)
        p->push_back(&f->at(j));
    for(int j=0;j<i->size();j++)
        p->push_back(&i->at(j));
    for(int j=0;j<s->size();j++)
        p->push_back(&s->at(j));
}
struct game{
    static game read(){//read from file
        std::vector<game> gm;
        auto file=std::fstream(path,std::ios::in|std::ios::binary);
        file.read(reinterpret_cast<char *>(gm.data()), sizeof(game));
        file.close();
        return gm.at(0);
    }
    static void write(game* g){//write to file
            auto file=std::fstream(path,std::ios::out|std::ios::trunc|std::ios::binary);
            file.write(reinterpret_cast<char*>(&g), sizeof(game));
            file.close();
    }
    std::vector<waterCreature> wp;
    std::vector<earthCreature> ep;
    std::vector<airCreature> ap;
    std::vector<iceCreature> ip;
    std::vector<fireCreature> fp;
    std::vector<steelCreature> sp;
    std::vector<creature*> creaturePool;
    std::vector<creature*> pool;
    std::vector<enemy> enemyPool;
    int round=0,victories=0;//index of enemy being fought,number of battles won
    creature* currentCreature= nullptr;
    void exit(){
        write(this);
        abort();
    }
    game(std::string){}
    game(){
        std::set<int> choices={};
        makeCreatures(&wp, &ep, &ap, &ip, &fp, &sp, &creaturePool);
        for(int i=1;i<7;i++)
            enemyPool.push_back(enemy("Enemy "+std::to_string(i)));
        std::cout<<"Pool size: "<<creaturePool.size()<<"\n";
        for(int i=0;i<creaturePool.size();i++)
            std::cout<<"["<<i+1<<"] "<<creaturePool.at(i)->name<<"\t Type: "+typeNames.at(creaturePool.at(i)->type)<<"\n";
        while(choices.size()<6){//choose 6 creatures from the creature pool
            int c;
            std::cout<<"Choose Creature "<<choices.size()+1<<" : ";
            std::cin>>c;
            if((c>0)&&(c<creaturePool.size()+1))
                if(choices.contains(c))
                    std::cout<<"Already Selected\n";
                else
                    choices.insert(c-1);
            else
                std::cout<<"Invalid Choice\n";
        }
        //choices={0,6,12,18,24,30};
        int e=0;
        for(int i=0;i<creaturePool.size();i++)
            if(choices.contains(i)){//move chosen creatures to your pool
                pool.push_back(creaturePool.at(i));
            }
            else {//distribute remaining creatures among enemies
                enemyPool.at(e).pool.push_back(creaturePool.at(i));
                if(e==5)
                    e=0;
                else
                    e++;
            }
    }
    void swapCreature(){//select from alive creatures in the pool
        std::vector<creature*> cret=gameFuns::getLive(&pool);
        int c= 200000;
        for(int i=0;i<cret.size();i++){
            std::cout<<"["<<i+1<<"] "<<cret.at(i)->name<<"\tLvl: "<<cret.at(i)->level<<"\tHP: "<<cret.at(i)->health*100<<"%\tExp: "<<cret.at(i)->exp<<"\n";}
        while((c<1)||(c>cret.size())) {
            if(c!=200000)
                std::cout << "Invalid choice\n";
            std::cout << "Choose creature> ";
            while (!(std::cin >> c)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice\n";
                std::cout << "Choose creature> ";
            }
        }
        currentCreature=cret.at(c-1);
    }
    int getPlayerAvgLevel(){
        int sum=0;
        for(creature* c:pool)
            sum+=c->level;
        return sum/pool.size();
    }
    void resetPoolHealth(){
        for(creature* c:pool)
            c->reset();
    }
    bool playerTurn(creature* enemy){//true=enemy defeated,false=not yet defeated
        std::string msg="";
        while(true){
            int numOfOptions=2;
            if(currentCreature->health>0) {
                numOfOptions++;
                if (currentCreature->exp > 4)
                    numOfOptions++;
                if (currentCreature->exp > 7)
                    numOfOptions++;
                if (currentCreature->exp > 9)
                    numOfOptions++;
            }
            if(msg=="")
                msg="Select option between 1-"+ std::to_string(numOfOptions);
            int inp=0;
            while((inp<1)||(inp>numOfOptions)) {
                std::cout << "\n---------------------------------------------------------------------------------------------------------------------------------\n";
                std::cout << "Enemy: " << enemy->name << "\tLevel: " << enemy->level + 1 << "\tType: "<< typeNames.at(enemy->type) << "\tHealth: " << int(enemy->health *100) << "%\n";
                std::cout << "You: " << currentCreature->name << "\tLevel: " << currentCreature->level + 1 << "\tType: " << typeNames.at(currentCreature->type)<< "\tHealth: " << int(currentCreature->health* 100) << "%\n";
                std::cout << "\t Shield: " << (currentCreature->defence > 0) << "\tExp: " << currentCreature->exp << "\n";
                std::vector<std::string> options={"[1] Exit game\n","[2] Swap Creature\n","[3] Normal Attack\n","[4] Enable Shield (5 Exp pts)\n","[5] Special Attack (8 Exp pts)\n","[6] Level up (10 Exp pts)\n"};
                for(int i=0;i<numOfOptions;i++)
                    std::cout<<options.at(i);
                std::cout << msg;
                std::cout<<"\n>";
                if (!(std::cin >> inp)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                msg="";
            }
            std::cout<<inp<<"\n";
            switch (inp) {
                case 1:
                    exit();
                case 2:
                    swapCreature();
                    msg="Creature swapped to "+currentCreature->name+"!";
                    break;
                case 3:
                    return currentCreature->attack(enemy);
                case 4:
                    currentCreature->shield();
                    msg="Shield enabled";
                    break;
                case 5:
                    return currentCreature->attack(enemy, true);
                case 6:
                    currentCreature->levelUp();
                    msg=currentCreature->name+" Leveled up to "+std::to_string(currentCreature->level);
                    break;
            }
        }
    }
    bool fight(creature* e){//true=enemy's creature has been defeated,false=lost fight with enemy's creature (all creatures in pool defeated)
        if(currentCreature== nullptr)
            swapCreature();
        std::cout<<"You have chosen "<<currentCreature->name<<"!\n";
        while(gameFuns::hasAliveCreatures(&pool)){//while player has alive creatures
            if (playerTurn(enemyPool.at(round).currentCreature))//enemy's creature has been defeated
                return true;
            e->turn(currentCreature);//enemy's turn to fight
        }
        return false;

    }
    bool fight(enemy* e){//true=enemy has been defeated,false=lost fight with enemy (all creatures in pool defeated)
        while(gameFuns::hasAliveCreatures(&e->pool)){//while enemy has any alive creatures left
            if((enemyPool.at(round).currentCreature== nullptr)||(enemyPool.at(round).currentCreature->health<=0))
                enemyPool.at(round).chooseCreature();
            std::cout<<enemyPool.at(round).name<<" has chosen "<<enemyPool.at(round).currentCreature->name<<"!\n";
            if(!fight(enemyPool.at(round).currentCreature))//player defeated
                return false;
        }
        //all enemy's creatures have been defeated
        return true;
    }
    void start(){
        while(round<=enemyPool.size()){
            std::cout<<"Fighting "<<enemyPool.at(round).name<<"\n";
            if(fight(&enemyPool.at(round))){//battle won
                victories++;
                std::cout<<"You won the battle against "<<enemyPool.at(round).name<<"!\n";
            }
            else
                std::cout<<"You lost the battle against "<<enemyPool.at(round).name<<"!\n";
            round++;
            resetPoolHealth();
            enemyPool.at(round).setLevel(getPlayerAvgLevel());
        }
        //finished fighting all enemies
        std::cout<<victories<<"\n";
    }


    void printPools(){
        std::cout<<"Your pool:\n";
        for(creature* c:pool)
            std::cout<<"\t"<<c->name<<"\n";
        for(enemy e:enemyPool) {
            std::cout << "\n\n\n"<<e.name<<"'s pool:\n";
            for(creature* c:e.pool)
                std::cout<<"\t"<<c->name<<"\n";
        }

    }

};