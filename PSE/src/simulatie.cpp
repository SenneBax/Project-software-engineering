//
// Created by senne on 11/03/2025.
//

#include "simulatie.h"
#include "Situatie.h"
#include <algorithm>
#include <iostream>

// Constructor
simulatie::simulatie(VerkeersSituatie& situatie, double tijdstap)
    : verkeerssituatie(situatie), tijdstap(tijdstap) {}

// methode om de stap te runnen voor alles
void simulatie::stap() {
    // Get access voor voertuigen en banen
    std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();
    const std::map<std::string, Baan>& banen = verkeerssituatie.getBanen();

    // tijdelijke vector om de voertuigen die verwijderd moeten worden in op te slagen
    std::vector<int> teVerwijderenVoertuigen;

    // Processen van elk voertuig
    for (int i = 0; i < voertuigen.size(); i++) {
        Voertuig& voertuig = voertuigen[i];

        // 1. bereken nieuwe snelheid en positie van voertuigen
        voertuig.rijd(tijdstap);

        // 2. bereken de nieuwe acceleratie
        // hangt af van zzaken zoals afstand tot het volgend voertuig, verkeerslichten etc.
        updateVersnelling(voertuig);

        // 3 & 4. controlleer of voertuigen nog wel op de weg zijn anders worden ze markeerd om verwijderd te worden
        const Baan& huidigeWeg = banen.at(voertuig.getBaan());
        if (voertuig.getPositie() >= huidigeWeg.getLengte()) {
            teVerwijderenVoertuigen.push_back(i);
        }
    }

    // verwijder de voertuigen die niet meer op een weg staan
    std::sort(teVerwijderenVoertuigen.begin(), teVerwijderenVoertuigen.end(), std::greater<int>());
    for (int index : teVerwijderenVoertuigen) {
        verkeerssituatie.verwijderVoertuig(index);
    }
}

// Methode om voertuig versnelling te updaten op de voorwaarden dat er versneld mag worden
void simulatie::updateVersnelling(Voertuig& voertuig) {


    const double doelSnelheid = 13.89; // Target speed: 50 km/h = 13.89 m/s
    const double maxVersnelling = 2.0; // Maximum acceleration: 2 m/s²              moet nog aangepast worden
    const double maxVertraging = -4.0; // Maximum deceleration: -4 m/s²

    // controlleer of er obstakels zijn (e.g., voertuigen, verkeerslichten)
    double afstandTotObstakel = afstandNaarNaastObstakel(voertuig);

    // als er geen obstakels zijn, accelereer naar de doelsnelheid.
    if (afstandTotObstakel > 100 || afstandTotObstakel < 0) {
        if (voertuig.getSnelheid() < doelSnelheid) {
            voertuig.setVersnelling(maxVersnelling);
        } else {
            voertuig.setVersnelling(0.0);
        }
    } else {
        // bereken de benodigde versnelling of vertraging
        // gebruikt v² = v₀² + 2as -> a = (v² - v₀²)/(2s)
        double huidigeSnelheid = voertuig.getSnelheid();
        double vereistVertraging = -(huidigeSnelheid * huidigeSnelheid) / (2 * afstandTotObstakel);

        // decelereer als het nodig is, anders behoud de snelheid
        if (vereistVertraging < 0 && vereistVertraging > maxVertraging) {
            voertuig.setVersnelling(vereistVertraging);
        } else if (vereistVertraging < maxVertraging) {
            voertuig.setVersnelling(maxVertraging);
        } else if (huidigeSnelheid < doelSnelheid) {
            voertuig.setVersnelling(maxVersnelling);
        } else {
            voertuig.setVersnelling(0.0);
        }
    }
}

// Methode om de afstand tot het kortste obstakel weer te geven
double simulatie::afstandNaarNaastObstakel(const Voertuig& voertuig) {
    const std::vector<Voertuig>& alleVoertuigen = verkeerssituatie.getVoertuigen();
    const std::vector<Verkeerslicht>& verkeerslichten = verkeerssituatie.getVerkeerslichten();
    const std::map<std::string, Baan>& banen = verkeerssituatie.getBanen();

    double kortsteAfstand = -1; // Negatief betekent dat er geen obstakels zijn gevonden positief is dit wel zo

    // controleer de afstand tussen 2 voertuigen op dezelfde baan
    for (const Voertuig& anderVoertuig : alleVoertuigen) {
        // Skip als het hetzelfde voertuig is of ze op een andere baan staan
        if (&voertuig == &anderVoertuig || voertuig.getBaan() != anderVoertuig.getBaan()) {
            continue;
        }

        // bereken de afstand als het andere voertuig ervoor rijdt
        if (anderVoertuig.getPositie() > voertuig.getPositie()) {
            double afstand = anderVoertuig.getPositie() - voertuig.getPositie();
            if (kortsteAfstand < 0 || afstand < kortsteAfstand) {
                kortsteAfstand = afstand;
            }
        }
    }

    // controleer of er verkeerslichten op de weg staan
    for (const Verkeerslicht& verkeerslicht : verkeerslichten) {
        // Skip als het op een andere weg staat
        if (voertuig.getBaan() != verkeerslicht.getBaan()) {
            continue;
        }

        // bereken de afstand tussen het verkeerslicht en de wagen en of het licht rood of groen is
        if (verkeerslicht.getPositie() > voertuig.getPositie() && isVerkeerslichtRood(verkeerslicht)) {
            double afstand = verkeerslicht.getPositie() - voertuig.getPositie();
            if (kortsteAfstand < 0 || afstand < kortsteAfstand) {
                kortsteAfstand = afstand;
            }
        }
    }

    // controleer de afstand tot aan het begin van de weg
    const Baan& huidigeWeg = banen.at(voertuig.getBaan());
    double afstandTotEinde = huidigeWeg.getLengte() - voertuig.getPositie();
    if (kortsteAfstand < 0 || afstandTotEinde < kortsteAfstand) {
        kortsteAfstand = afstandTotEinde;
    }

    return kortsteAfstand;
}

// Methode om te controleren of verkeerslicht rood is (moet nog aangepast orden als lichten ook oranje kunnen zijn
bool simulatie::isVerkeerslichtRood(const Verkeerslicht& verkeerslicht) {

    int cycle = verkeerslicht.getCyclus();
    return (int(huidigeSimulatieTijd) % cycle) < (cycle / 2);
}
