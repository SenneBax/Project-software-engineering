//
// Created by senne on 27/02/2025.
//


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
