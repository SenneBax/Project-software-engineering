/**
* @file main.release.cpp
 * @author senne
 * @date 6/03/2025
 * @brief dit is de release file waar de die de output van het programma terecht hoort te komen. (dit is een executable).
 */


#include <iostream>
#include <string>
#include "Situatie.h"


using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {

        return 1;
    }

    const string bestandsnaam = argv[1];


    if (VerkeersSituatie situatie; leesVerkeersSituatie(bestandsnaam, situatie)) {
        cout << "Verkeerssituatie succesvol ingelezen." << endl;
        situatie.printInfo();
        return 0;
    }

    else {

        return 1;
    }

}
