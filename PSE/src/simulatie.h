//
// Created by senne on 11/03/2025.
//

#ifndef SIMULATIE_H
#define SIMULATIE_H

#include "Situatie.h"

class simulatie {
private:
    VerkeersSituatie& verkeerssituatie;
    double tijdstap;
    double huidigeSimulatieTijd;
    bool autoGenereerVoertuigen = false;

    // Standaardwaarden volgens sectie B.6
    double voertuigLengte;       // l - lengte van een voertuig
    double maxSnelheid;          // Vmax - absolute maximale snelheid
    double maxVersnelling;       // amax - maximale versnelling
    double maxRemFactor;         // bmax - maximale remfactor
    double minVolgafstand;       // fmin - minimale volgafstand
    double vertraagAfstand;      // Δxs - vertraagafstand
    double stopAfstand;          // Δxs0 - stopafstand
    double vertraagFactor;       // s - vertraagfactor

    // Statistieken
    int aantalVoertuigen = 0;
    double gemiddeldeSnelheid = 0.0;
    int verwijderdeVoertuigenTeller = 0;
    int totaalVerwijderdeVoertuigen = 0;
    double totaleTijd = 0.0;

    // Ondersteunende methoden
    void updatePositieEnSnelheid(Voertuig& voertuig);
    void updateVersnelling(Voertuig& voertuig, Voertuig* voorgaandVoertuig, bool isEersteVoertuig);
    const Verkeerslicht* zoekEerstvolgendVerkeerslicht(const Voertuig& voertuig);
    bool isVerkeerslichtRood(const Verkeerslicht& verkeerslicht);
    void genereerNieuweVoertuigen();
    void verhoogVerwijderdeVoertuigenTeller();
    void verzamelStatistieken();

public:
    simulatie(VerkeersSituatie& situatie, double tijdstap);

    void stap();
    void setAutoGenereerVoertuigen(bool genereer);

    // Getters
    double getHuidigeSimulatieTijd() const;
    double getTijdstap() const;
    int getAantalVoertuigen() const;
    double getGemiddeldeSnelheid() const;
    int getTotaalVerwijderdeVoertuigen() const;
    double getTotaleTijd() const;
};

#endif //SIMULATIE_H