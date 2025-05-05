/**
 * @file kruispunt.cpp
 * @brief Implementatie van de kruispunt klasse
 */

#include "kruispunt.h"
#include <algorithm>
#include <random>
#include <cassert>

#include "DesignByContract.h"

/**
 * @brief Constructor voor het kruispunt
 */
Kruispunt::Kruispunt()
{
    _initCheck = this;
    ENSURE(properlyInitialized(),"Constructor moet eindigen in een geldige toestand.");

}

/**
 * @brief Voeg een weg toe aan het kruispunt
 * @param baanNaam Naam van de weg
 * @param positie Positie op de weg
 * @return True als de weg succesvol werd toegevoegd, false anders
 */
bool Kruispunt::voegBaanToe(const std::string& baanNaam, double positie) {
    REQUIRE(!baanNaam.empty(), "baanNaam is leeg");
    REQUIRE(positie >= 0.0, "positie moet positive zijn.");
    ENSURE(properlyInitialized(),"Constructor moet eindigen in een geldige toestand.");

    // Controleer of de weg al deel uitmaakt van dit kruispunt
    if (bevatBaan(baanNaam)) {
        return false;
    }

    banen.emplace_back(baanNaam, positie);
    return true;
}


bool Kruispunt::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Krijg alle wegen die verbonden zijn met dit kruispunt
 * @return Vector van paren met wegnamen en posities
 */
std::vector<std::pair<std::string, double>> Kruispunt::getBanen() const {
    //REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij getBanen");

    std::vector<std::pair<std::string, double>> result;

    for (const auto& baan : banen) {
        result.emplace_back(baan.naam, baan.positie);
    }

    return result;
}

/**
 * @brief Controleer of een weg deel uitmaakt van dit kruispunt
 * @param baanNaam Naam van de weg
 * @return True als de weg deel uitmaakt van dit kruispunt, false anders
 */
bool Kruispunt::bevatBaan(const std::string& baanNaam) const {
    //REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij bevatBaan");
    REQUIRE(!baanNaam.empty(), "baanNaam is empty");
    return std::any_of(banen.begin(), banen.end(),
                       [&baanNaam](const BaanPositie& baan) {
                           return baan.naam == baanNaam;
                       });

    //ENSURE(properlyInitialized(), "Constructor moet eindigen in geen geldige staat.");
}

/**
 * @brief Krijg positie op een specifieke weg
 * @param baanNaam Naam van de weg
 * @return Positie op de weg, -1 als de weg niet gevonden wordt
 */
double Kruispunt::getPositieOpBaan(const std::string& baanNaam) const {
    REQUIRE(!baanNaam.empty(), "baanNaam is leeg");
    for (const auto& baan : banen) {
        if (baan.naam == baanNaam) {
            return baan.positie;
        }
    }

    return -1.0; // Weg niet gevonden
}

/**
 * @brief Kies een willekeurige weg om door te gaan vanaf het kruispunt
 * @param huidigeWeg Huidige wegnaam (om uit te sluiten van mogelijkheden)
 * @return Naam van de gekozen weg, lege string als er geen geldige weg bestaat
 */
std::string Kruispunt::kiesRandomBaan(const std::string& huidigeWeg) const {
    REQUIRE(!huidigeWeg.empty(), "huidigeWeg is leeg");
    // Verzamel potentiële wegen (exclusief de huidige)
    std::vector<std::string> mogelijkeBanen;

    for (const auto& baan : banen) {
        // Sla de huidige weg over en wegen die eindigen bij dit kruispunt (positie is gelijk aan de weglengte)
        if (baan.naam != huidigeWeg) {
            mogelijkeBanen.push_back(baan.naam);
        }
    }

    // Als er geen geldige wegen zijn om door te gaan, retourneer een lege string
    if (mogelijkeBanen.empty()) {
        return "";
    }

    // Kies een willekeurige weg
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, mogelijkeBanen.size() - 1);
    return mogelijkeBanen[distr(gen)];
}