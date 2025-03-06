//
// Created by senne on 27/02/2025.
//

#include "Baan.h"
#include <iostream>

// Implementatie van de default constructor
Baan::Baan()
    : naam(""), lengte(0) {}

Baan::Baan(const std::string& naam, int lengte)
    : naam(naam), lengte(lengte) {}

std::string Baan::getNaam() const {
    return naam;
}

int Baan::getLengte() const {
    return lengte;
}