//
// Created by senne on 27/02/2025.
//

#ifndef BAAN_H
#define BAAN_H

#include <string>

using namespace std;

class Baan {
private:
    string naam;
    int lengte;

public:
    // Default constructor toevoegen
    Baan();

    // Bestaande constructor behouden
    Baan(const string& naam, int lengte);

    [[nodiscard]] string getNaam() const;
    [[nodiscard]] int getLengte() const;
};

#endif // BAAN_H