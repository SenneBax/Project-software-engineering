/**
 * @file simulatie.cpp
 * @author senne
 * @date 11/03/2025
 * @brief Implementatie van de simulatie klasse
 */

#include "simulatie.h"
#include "situatie.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>

/**
 * @brief Constructor
 * @param situatie Referentie naar de te simuleren verkeerssituatie
 * @param tijdstap De tijdstap voor de simulatie in seconden
 */
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

/**
 * @brief Methode om de stap te runnen voor alles
 */
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
        std::sort(paar.second.begin(), paar.second.end(), [&voertuigen](const size_t a, const size_t b) {
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
            const size_t voertuigIndex = voertuigIndices[i];
            Voertuig& voertuig = voertuigen[voertuigIndex];

            // Bepaal of dit het eerste voertuig op de baan is
            const bool isEersteVoertuig = (i == 0);

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
    for (const int index : teVerwijderenVoertuigen) {
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

/**
 * @brief Methode om positie en snelheid te updaten volgens formules B.2
 * @param voertuig Het voertuig waarvan de positie en snelheid worden bijgewerkt
 */
void simulatie::updatePositieEnSnelheid(Voertuig& voertuig) const {
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
        v = std::max(0.0, v); // Zorg ervoor dat de snelheid niet negatief wordt

        // Bereken nieuwe positie
        double nieuweX = x + v * tijdstap + (a * tijdstap * tijdstap) / 2;

        // Controleer of het voertuig zich op een baan bevindt
        const auto& banen = verkeerssituatie.getBanen();
        auto baanIt = banen.find(voertuig.getBaan());

        if (baanIt != banen.end()) {
            const Baan& baan = baanIt->second;

            // Controleer of er een verkeerslicht is vlakbij het einde van de baan
            for (const auto& verkeerslicht : verkeerssituatie.getVerkeerslichten()) {
                if (verkeerslicht.getBaan() == voertuig.getBaan() &&
                    verkeerslicht.getPositie() > x &&
                    verkeerslicht.getPositie() < baan.getLengte() &&
                    isVerkeerslichtRood(verkeerslicht)) {

                    // Als we het verkeerslicht zouden passeren met deze update, blijf ervoor staan
                    if (nieuweX > verkeerslicht.getPositie()) {
                        nieuweX = verkeerslicht.getPositie() - 0.1;
                        v = 0.0; // Stop volledig
                    }
                }
            }

            // Zorg ervoor dat we niet voorbij het einde van de baan gaan
            if (nieuweX >= baan.getLengte()) {
                nieuweX = baan.getLengte() - 0.1;
            }
        }

        x = nieuweX;
    }

    // Update het voertuig
    voertuig.setSnelheid(v);
    voertuig.setPositie(x);
}

/**
 * @brief Methode om voertuig versnelling te updaten volgens formules B.3-B.5
 * @param voertuig Het voertuig waarvan de versnelling wordt bijgewerkt
 * @param voorgaandVoertuig Pointer naar het voorgaande voertuig (nullptr als er geen is)
 * @param isEersteVoertuig Boolean die aangeeft of dit het eerste voertuig op de baan is
 */
void simulatie::updateVersnelling(Voertuig& voertuig, const Voertuig* voorgaandVoertuig, const bool isEersteVoertuig) {
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
                // Bereken geschatte remafstand op basis van huidige snelheid
                double geschatteRemafstand = (v * v) / (2 * maxRemFactor);

                // Als we niet op tijd kunnen stoppen met normale vertraging, gebruik noodremming
                if (afstandTotVerkeerslicht <= std::max(stopAfstand, geschatteRemafstand)) {
                    // Sterkere vertraging voor noodremming
                    double a = -maxRemFactor * 1.5; // Verhoog remkracht
                    voertuig.setVersnelling(a);
                    return;
                } else if (afstandTotVerkeerslicht <= vertraagAfstand) {
                    // Begin eerder/agressiever te vertragen
                    vmax = vertraagFactor * 0.8 * maxSnelheid; // Verlaag doelsnelheid verder
                }
            }
        }
    }

    // Bereken versnelling volgens formules B.3
    double delta = 0.0;

    if (voorgaandVoertuig != nullptr) {
        // Bereken de volgafstand
        const double deltaX = voorgaandVoertuig->getPositie() - voertuig.getPositie() - voertuigLengte;

        // Voorkom negatieve of te kleine deltaX waarden die tot NaN of infinity kunnen leiden
        if (deltaX <= 0.1) {
            // Noodremming bij zeer kleine afstand
            voertuig.setVersnelling(-maxRemFactor);
            return;
        }

        // Bereken het snelheidsverschil
        const double deltaV = v - voorgaandVoertuig->getSnelheid();

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

/**
 * @brief Methode om het eerstvolgende verkeerslicht te vinden
 * @param voertuig Het voertuig waarvoor het eerstvolgende verkeerslicht wordt gezocht
 * @return Pointer naar het eerstvolgende verkeerslicht, of nullptr als er geen is
 */
const Verkeerslicht* simulatie::zoekEerstvolgendVerkeerslicht(const Voertuig& voertuig) const {
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

/**
 * @brief Methode om te controleren of verkeerslicht rood is
 * @param verkeerslicht Het te controleren verkeerslicht
 * @return true als het verkeerslicht rood is, anders false
 */
bool simulatie::isVerkeerslichtRood(const Verkeerslicht& verkeerslicht) const {
    const int cyclus = verkeerslicht.getCyclus();
    // Berekent de huidige cyclus-status (0 = rood, 1 = groen)
    const int cyclusStatus = static_cast<int>(huidigeSimulatieTijd / (cyclus / 2)) % 2;
    return cyclusStatus == 0; // Rood als cyclusStatus 0 is
}

/**
 * @brief Methode om periodiek nieuwe voertuigen te genereren
 * @note Voertuiggenerator toch wel opgeleverd
 */
void simulatie::genereerNieuweVoertuigen() const {
    static double laatstGenereerTijd = 0.0;
    constexpr double genereerInterval = 5.0; // Genereer een voertuig elke 5 seconden

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
/**
 * @brief Methode om auto-generatie van voertuigen aan/uit te zetten
 *
 */

void simulatie::setAutoGenereerVoertuigen(bool genereer) {
    autoGenereerVoertuigen = genereer;
}

/**
 * @brief Methode om statistieken te bewaren
 *
 */
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
/**
 * @brief Methode om de verwijderde voertuigen te tellen
 *
 */
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