//
// Created by senne on 27/02/2025.
//

#include "Verkeerslicht.h"
#include <iostream>

Verkeerslicht::Verkeerslicht(const std::string& baan, int positie, int cyclus)
    : baan(baan), positie(positie), cyclus(cyclus) {}

std::string Verkeerslicht::getBaan() const {
    return baan;
}

int Verkeerslicht::getPositie() const {
    return positie;
}

int Verkeerslicht::getCyclus() const {
    return cyclus;
}