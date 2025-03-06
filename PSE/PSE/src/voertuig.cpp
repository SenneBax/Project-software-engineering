//
// Created by senne on 27/02/2025.
//

#include "Voertuig.h"
#include <iostream>
using namespace std;

Voertuig::Voertuig(const string& baan, int positie)
    : baan(baan), positie(positie) {}

string Voertuig::getBaan() const {
    return baan;
}

int Voertuig::getPositie() const {
    return positie;
}