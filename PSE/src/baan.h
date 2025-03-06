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
    Baan(const string& naam, int lengte);

    string getNaam() const;
    int getLengte() const;
};

#endif // BAAN_H
