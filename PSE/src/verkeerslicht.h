//
// Created by senne on 27/02/2025.
//

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H

#include <string>

class Verkeerslicht {
private:
    std::string baan;
    int positie;
    int cyclus;

public:
    Verkeerslicht(const std::string& baan, int positie, int cyclus);

    std::string getBaan() const;
    int getPositie() const;
    int getCyclus() const;
};

#endif // VERKEERSLICHT_H
