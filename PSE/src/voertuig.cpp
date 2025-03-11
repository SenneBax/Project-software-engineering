//
// Created by senne on 27/02/2025.
//

#include "Voertuig.h"
#include <iostream>
using namespace std;

Voertuig::Voertuig(const string& baan, int positie, double snelheid, double versnelling)
    : baan(baan), positie(positie), snelheid(snelheid), versnelling(versnelling) {}

string Voertuig::getBaan() const {
    return baan;
}

int Voertuig::getPositie() const {
    return positie;
}

double Voertuig::getSnelheid() const {
    return snelheid;
}

double Voertuig::getVersnelling() const {
    return versnelling;
}

void Voertuig::setPositie(int nieuwePositie) {
    positie = nieuwePositie;
}

void Voertuig::setSnelheid(double nieuweSnelheid) {
    snelheid = nieuweSnelheid;
}

void Voertuig::setVersnelling(double nieuweVersnelling) {
    versnelling = nieuweVersnelling;
}

void Voertuig::rijd(double tijdstap) {
    // Update snelheid eerst: v = v0 + a * t
    double nieuweSnelheid = snelheid + versnelling * tijdstap;

    // Ensure speed is not negative
    if (nieuweSnelheid < 0) {
        nieuweSnelheid = 0;
    }

    // Update positie: x = x0 + v0 * t + 0.5 * a * t²
    // gebruikt de gemiddelde snelheid: (initialSpeed + finalSpeed) / 2
    double gemiddeldeSnelheid = (snelheid + nieuweSnelheid) / 2;
    int nieuwePositie = positie + static_cast<int>(gemiddeldeSnelheid * tijdstap);

    // Update de waardes
    snelheid = nieuweSnelheid;
    positie = nieuwePositie;
}