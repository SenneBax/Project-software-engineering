//
// Created by senne on 11/03/2025.
//

#include "simulatie.h"
#include "Situatie.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>

// Constructor
simulatie::simulatie(VerkeersSituatie& situatie, double tijdstap)
    : verkeerssituatie(situatie), tijdstap(tijdstap), huidigeSimulatieTijd(0.0) {
    // Controleer of de tijdstap geldig is
    if (tijdstap <= 0) {

        this->tijdstap = 0.0166;
    }

    // Initialiseer standaardwaarden volgens B.6
    voertuigLengte = 4.0;
    maxSnelheid = 16.6;
    maxVersnelling = 1.44;
    maxRemFactor = 4.61;
    minVolgafstand = 4.0;
    vertraagAfstand = 50.0;
    stopAfstand = 15.0;
    vertraagFactor = 0.4;
}

// Methode om de stap te runnen voor alles
void simulatie::stap() {
    // Get access voor voertuigen en banen
    std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();
    const std::map<std::string, Baan>& banen = verkeerssituatie.getBanen();

    // Tijdelijke vector om de voertuigen die verwijderd moeten worden in op te slagen
    std::vector<int> teVerwijderenVoertuigen;

    // Per baan de voertuigen ordenen en verwerken
    std::map<std::string, std::vector<size_t>> voertuigenPerBaan;

    // Voertuigen per baan indexeren
    for (size_t i = 0; i < voertuigen.size(); i++) {
        voertuigenPerBaan[voertuigen[i].getBaan()].push_back(i);
    }

    // Voor elke baan, sorteer de voertuigen op positie (aflopend)
    for (auto& paar : voertuigenPerBaan) {
        std::sort(paar.second.begin(), paar.second.end(), [&voertuigen](size_t a, size_t b) {
            return voertuigen[a].getPositie() > voertuigen[b].getPositie();
        });
    }

    // Processen van elk voertuig
    for (auto& baanPaar : voertuigenPerBaan) {
        const std::string& baanNaam = baanPaar.first;
        const std::vector<size_t>& voertuigIndices = baanPaar.second;

        // Controleer of de baan bestaat
        auto baanIt = banen.find(baanNaam);
        if (baanIt == banen.end()) {
            continue; // Sla deze baan over als hij niet bestaat
        }

        const Baan& baan = baanIt->second;

        // Verwerk voertuigen van achter naar voor (hoogste positie eerst)
        for (size_t i = 0; i < voertuigIndices.size(); i++) {
            size_t voertuigIndex = voertuigIndices[i];
            Voertuig& voertuig = voertuigen[voertuigIndex];

            // Bepaal of dit het eerste voertuig op de baan is
            bool isEersteVoertuig = (i == 0);

            // Bepaal voorgaand voertuig (als dit niet het eerste voertuig is)
            Voertuig* voorgaandVoertuig = nullptr;
            if (!isEersteVoertuig) {
                voorgaandVoertuig = &voertuigen[voertuigIndices[i-1]];
            }

            // Update de versnelling volgens de formules uit ed specificatie
            updateVersnelling(voertuig, voorgaandVoertuig, isEersteVoertuig);

            // Update positie en snelheid volgens de formules
            updatePositieEnSnelheid(voertuig);

            // Controleer of het voertuig nog op de baan is
            if (voertuig.getPositie() >= baan.getLengte()) {
                teVerwijderenVoertuigen.push_back(voertuigIndex);
            }
        }
    }

    // Verwijder de voertuigen die niet meer op een weg staan (van hoog naar laag om indexen correct te houden)
    std::sort(teVerwijderenVoertuigen.begin(), teVerwijderenVoertuigen.end(), std::greater<int>());
    for (int index : teVerwijderenVoertuigen) {
        verkeerssituatie.verwijderVoertuig(index);
        verhoogVerwijderdeVoertuigenTeller();
    }

    // Update de simulatietijd
    huidigeSimulatieTijd += tijdstap;

    // Voeg periodiek nieuwe voertuigen toe als optie is ingeschakeld
    if (autoGenereerVoertuigen) {
        genereerNieuweVoertuigen();
    }

    // Verzamel statistieken
    verzamelStatistieken();
}

// Methode om positie en snelheid te updaten volgens formules B.2
void simulatie::updatePositieEnSnelheid(Voertuig& voertuig) {
    double v = voertuig.getSnelheid();
    double a = voertuig.getVersnelling();
    double x = voertuig.getPositie();

    // Bereken nieuwe snelheid en positie volgens B.2
    if (v + a * tijdstap < 0) {
        // Snelheid zou negatief worden, pas positie aan en zet snelheid op 0
        x = x - (v * v) / (2 * a);
        v = 0.0;
    } else {
        // Normale situatie, update snelheid en dan positie
        v = v + a * tijdstap;
        x = x + v * tijdstap + (a * tijdstap * tijdstap) / 2;
    }

    // Update het voertuig
    voertuig.setSnelheid(v);
    voertuig.setPositie(x);
}

// Methode om voertuig versnelling te updaten volgens formules B.3-B.5
void simulatie::updateVersnelling(Voertuig& voertuig, Voertuig* voorgaandVoertuig, bool isEersteVoertuig) {
    double v = voertuig.getSnelheid();
    double vmax = maxSnelheid; // Standaard is dit de absolute maximum snelheid

    // Controleer verkeerslichten voor het eerste voertuig op de baan
    if (isEersteVoertuig) {
        // Zoek het eerstvolgende verkeerslicht op deze baan
        const Verkeerslicht* eerstvolgendVerkeerslicht = zoekEerstvolgendVerkeerslicht(voertuig);

        if (eerstvolgendVerkeerslicht != nullptr) {
            double afstandTotVerkeerslicht = eerstvolgendVerkeerslicht->getPositie() - voertuig.getPositie();

            // Check of het verkeerslicht rood is
            bool isRood = isVerkeerslichtRood(*eerstvolgendVerkeerslicht);

            if (isRood) {
                if (afstandTotVerkeerslicht <= stopAfstand) {
                    // Voertuig moet stoppen volgens B.5
                    double a = -maxRemFactor * v / maxSnelheid;
                    voertuig.setVersnelling(a);
                    return;
                } else if (afstandTotVerkeerslicht <= vertraagAfstand) {
                    // Voertuig moet vertragen volgens B.4
                    vmax = vertraagFactor * maxSnelheid;
                }
            }
        }
    }

    // Bereken versnelling volgens formules B.3
    double delta = 0.0;

    if (voorgaandVoertuig != nullptr) {
        // Bereken de volgafstand
        double deltaX = voorgaandVoertuig->getPositie() - voertuig.getPositie() - voertuigLengte;

        // Bereken het snelheidsverschil
        double deltaV = v - voorgaandVoertuig->getSnelheid();

        // Bereken de interactieterm delta
        double s_star = minVolgafstand + std::max(0.0, v + v * deltaV / (2 * std::sqrt(maxVersnelling * maxRemFactor)));
        delta = pow(s_star / deltaX, 2);
    }

    // Bereken de versnelling volgens de formule in B.3
    double a = maxVersnelling * (1 - pow(v / vmax, 4) - delta);

    // Begrens de versnelling tussen -maxRemFactor en maxVersnelling
    a = std::max(-maxRemFactor, std::min(maxVersnelling, a));

    voertuig.setVersnelling(a);
}

// Methode om het eerstvolgende verkeerslicht te vinden
const Verkeerslicht* simulatie::zoekEerstvolgendVerkeerslicht(const Voertuig& voertuig) {
    const std::vector<Verkeerslicht>& verkeerslichten = verkeerssituatie.getVerkeerslichten();
    const Verkeerslicht* eerstvolgend = nullptr;
    double kortsteAfstand = -1;

    for (const Verkeerslicht& verkeerslicht : verkeerslichten) {
        // Skip als het op een andere weg staat
        if (voertuig.getBaan() != verkeerslicht.getBaan()) {
            continue;
        }

        // Bereken de afstand tussen het verkeerslicht en de wagen
        if (verkeerslicht.getPositie() > voertuig.getPositie()) {
            double afstand = verkeerslicht.getPositie() - voertuig.getPositie();
            if (kortsteAfstand < 0 || afstand < kortsteAfstand) {
                kortsteAfstand = afstand;
                eerstvolgend = &verkeerslicht;
            }
        }
    }

    return eerstvolgend;
}

// Methode om te controleren of verkeerslicht rood is
bool simulatie::isVerkeerslichtRood(const Verkeerslicht& verkeerslicht) {
    int cyclus = verkeerslicht.getCyclus();
    // Berekent de huidige cyclus-status (0 = rood, 1 = groen)
    int cyclusStatus = static_cast<int>(huidigeSimulatieTijd / (cyclus / 2)) % 2;
    return cyclusStatus == 0; // Rood als cyclusStatus 0 is
}

// Methode om periodiek nieuwe voertuigen te genereren
// Voertuiggenerator toch wel opgeleverd
void simulatie::genereerNieuweVoertuigen() {
    static double laatstGenereerTijd = 0.0;
    const double genereerInterval = 5.0; // Genereer een voertuig elke 5 seconden

    if (huidigeSimulatieTijd - laatstGenereerTijd >= genereerInterval) {
        const std::map<std::string, Baan>& banen = verkeerssituatie.getBanen();

        if (!banen.empty()) {
            // Willekeurig een baan kiezen
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, banen.size() - 1);

            auto it = banen.begin();
            std::advance(it, distr(gen));

            // Nieuw voertuig aan het begin van de baan (positie 0)
            Voertuig nieuwVoertuig(it->first, 0.0, 0.0, 0.0);
            verkeerssituatie.voegVoertuigToe(nieuwVoertuig);

            laatstGenereerTijd = huidigeSimulatieTijd;
        }
    }
}

// Methode om auto-generatie van voertuigen aan/uit te zetten
void simulatie::setAutoGenereerVoertuigen(bool genereer) {
    autoGenereerVoertuigen = genereer;
}

// Methode om statistieken te verzamelen
void simulatie::verzamelStatistieken() {
    const std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();

    totaleTijd += tijdstap;

    // Tel het aantal voertuigen dat momenteel in de simulatie zit
    aantalVoertuigen = voertuigen.size();

    // Bereken gemiddelde snelheid van alle voertuigen
    double totaleSnelheid = 0.0;
    for (const Voertuig& voertuig : voertuigen) {
        totaleSnelheid += voertuig.getSnelheid();
    }

    if (!voertuigen.empty()) {
        gemiddeldeSnelheid = totaleSnelheid / voertuigen.size();
    }

    // Verhoog teller voor verwijderde voertuigen
    totaalVerwijderdeVoertuigen += verwijderdeVoertuigenTeller;
    verwijderdeVoertuigenTeller = 0;
}

// Methode om de verwijderde voertuigen teller te verhogen
void simulatie::verhoogVerwijderdeVoertuigenTeller() {
    verwijderdeVoertuigenTeller++;
}

// Getters voor simulatie eigenschappen en statistieken
double simulatie::getHuidigeSimulatieTijd() const {
    return huidigeSimulatieTijd;
}

double simulatie::getTijdstap() const {
    return tijdstap;
}

int simulatie::getAantalVoertuigen() const {
    return aantalVoertuigen;
}

double simulatie::getGemiddeldeSnelheid() const {
    return gemiddeldeSnelheid;
}

int simulatie::getTotaalVerwijderdeVoertuigen() const {
    return totaalVerwijderdeVoertuigen;
}

double simulatie::getTotaleTijd() const {
    return totaleTijd;
}