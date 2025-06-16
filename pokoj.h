#ifndef POKOJ_H
#define POKOJ_H

#include <vector>
#include <iostream>
using namespace std;

struct room{
    int xl,xr; //x lewy, x prawy
    int yd,yu; // y dolny, y gorny
}; //Pokoj z danymi o swoich "granicach", gdzie sie zaczyna i gdzie sie konczy


//funkcje pokoju np. wybor pokoju, tworzenie ich itd
vector<room> create_room(){
    vector<room> result;
    const int number_of_rooms=12;
    int xl, xr;
    int yd,yu;

    for (int i=1;i<=number_of_rooms;i++){ // na "sztywno" ustawione granice pokoji
        switch (i){
            case 1:{
                xl=18; xr=25;
                yu=6;yd=19;
                break;
            }
            case 2:{
                xl=28; xr=52;
                yu=6;yd=14;
                break;
            }
            case 3:{
                xl=54; xr=62;
                yu=6;yd=13;
                break;
            }
            case 4:{
                xl=65; xr=71;
                 yu=6;yd=15;
                break;
            }
            case 5:{
                xl=9; xr=22;
                 yu=20;yd=33;
                break;
            }
            case 6:{
                xl=28; xr=39;
                 yu=17;yd=28;
                break;
            }
            case 7:{
                xl=41; xr=52;
                 yu=17;yd=28;
                break;
            }
            case 8:{
                xl=54; xr=62;
                yu=15; yd=23;
                break;
            }
            case 9:{
                xl=64; xr=82;
                yu=20;yd=33;
                break;
            }
            case 10:{
                xl=23; xr=62;
                 yu=30;yd=35;
                break;
            }
            case 11:{
                xl=19; xr=39;
                yu=38;yd=47;
                break;
            }
            case 12:{
                xl=41; xr=71;
                yu=39;yd=47;
                break;
            }
        }
        room pokoj;
        pokoj.xl=xl;
        pokoj.xr=xr;
        pokoj.yd=yd;
        pokoj.yu=yu;
        result.push_back(pokoj);
    }
    return result;
}


#endif // POKOJ_H
