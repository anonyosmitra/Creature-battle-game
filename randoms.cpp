#include <random>
namespace randoms{
    int randomInRange(int a, int b) {//random num between a-b
        return a + random() % b - a;
        //return a + rand() % b - a;
    }
    int randomInRange(int b){//random num between 0-b
        return randomInRange(0,b);
    }
    bool dice(int s){//odds with 1 out of s chances for true
        return (0== randomInRange(s));
    }
}