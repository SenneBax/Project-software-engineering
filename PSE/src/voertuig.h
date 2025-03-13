//
// Created by senne on 27/02/2025.
//

// Updated Voertuig.h
#ifndef VOERTUIG_H
#define VOERTUIG_H

#include <string>
using namespace std;

class Voertuig {
private:
    string baan;
    int positie;
    double snelheid;     // in m/s
    double versnelling;  // in m/s²

public:
    // geüpdate constructor met snelheid en versnellingsparameters
    Voertuig(const string& baan, int positie, double snelheid = 0.0, double versnelling = 0.0);

    // Getters
    string getBaan() const;
    int getPositie() const;
    double getSnelheid() const;
    double getVersnelling() const;

    // Setters
    void setPositie(int nieuwePositie);
    void setSnelheid(double nieuweSnelheid);
    void setVersnelling(double nieuweVersnelling);

    // Methode om de voertuigpositie te updaten en ook de snelheid op voorbije tijd
    void rijd(double tijdstap);
};

#endif // VOERTUIG_H