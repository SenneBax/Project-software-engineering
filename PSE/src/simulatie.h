#ifndef SIMULATIE_H
#define SIMULATIE_H

#include "Situatie.h"
#include "Voertuig.h"
#include "Verkeerslicht.h"

class simulatie {
private:
    VerkeersSituatie& verkeerssituatie;
    double tijdstap;               // de tijdsstappen in seconden
    double huidigeSimulatieTijd;   // huidige tijd in seconden

    // hulpmethodes
    void updateVersnelling(Voertuig& voertuig);
    double afstandNaarNaastObstakel(const Voertuig& voertuig);
    bool isVerkeerslichtRood(const Verkeerslicht& verkeerslicht);

public:
    // Constructor
    simulatie(VerkeersSituatie& situatie, double tijdstap = 1.0);

    // run 1 stap van de simulatie
    void stap();

    // Get de simulatie -tijd op het huidige moment
    double getSimulatieTijd() const { return huidigeSimulatieTijd; }

    // Set de simulation tijd naar een bepaald moment
    void setSimulatieTijd(double tijd) { huidigeSimulatieTijd = tijd; }
};

#endif // SIMULATIE_H