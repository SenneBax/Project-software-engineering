//
// Created by senne on 27/02/2025.
//


#include <iostream>
#include <string>
#include "Situatie.h"


using namespace std;

int main(int argc, char* argv[]) {
    std::cout << " Programma gestart!" << std::endl;
    if (argc != 2) {
        std::cerr << " ERROR: Geen XML-bestand opgegeven! Gebruik: ./release_target verkeer.xml" << std::endl;
        return 1;
    }
    std::cout << " Bestandsnaam: " << argv[1] << std::endl;


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
