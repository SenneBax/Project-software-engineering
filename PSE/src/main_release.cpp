//
// Created by senne on 27/02/2025.
//


#include <iostream>
#include <string>
#include "Situatie.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Gebruik: " << argv[0] << " <voorbeeld1.xml>" << endl;
        return 1;
    }

    string bestandsnaam = argv[1];
    VerkeersSituatie situatie;

    if (leesVerkeersSituatie(bestandsnaam, situatie)) {
        cout << "Verkeerssituatie succesvol ingelezen." << endl;
        situatie.printInfo();
        return 0;
    } else {
        cerr << "Fout bij inlezen verkeerssituatie." << endl;
        return 1;
    }
}
