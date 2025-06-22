/**
 * @file kruispunt.cpp
 * @brief Implementatie van de kruispunt klasse - Gecorrigeerde versie met DBC
 */

#include "kruispunt.h"
#include <algorithm>
#include <random>
#include <cassert>
#include "DesignByContract.h"

/**
 * @brief Constructor voor het kruispunt
 * @post ENSURE(properlyInitialized(),"Constructor moet eindigen in een geldige toestand.");
 */
Kruispunt::Kruispunt() : _initCheck(this) {
    // Initialiseer container expliciet
    banen.clear();

    ENSURE(properlyInitialized(),"Constructor moet eindigen in een geldige toestand.");
}

/**
 * @brief Copy constructor
 * @param other Het te kopiëren kruispunt
 * @pre REQUIRE(other.properlyInitialized(), "Te kopiëren kruispunt moet geldig zijn");
 * @post ENSURE(properlyInitialized(), "Nieuw object moet geldig zijn na copy");
 */
Kruispunt::Kruispunt(const Kruispunt& other) : _initCheck(this) {
    REQUIRE(other.properlyInitialized(), "Te kopiëren kruispunt moet geldig zijn");

    try {
        banen = other.banen;  // Deep copy van de vector
    } catch (...) {
        // Als kopiëren faalt, maak leeg object
        banen.clear();
    }

    ENSURE(properlyInitialized(), "Nieuw object moet geldig zijn na copy");
}

/**
 * @brief Assignment operator
 * @param other Het toe te wijzen kruispunt
 * @return Referentie naar dit object
 * @pre REQUIRE(properlyInitialized(), "Dit object moet geldig zijn voor assignment");
 * @pre REQUIRE(other.properlyInitialized(), "Te kopiëren kruispunt moet geldig zijn");
 * @post ENSURE(properlyInitialized(), "Object moet geldig blijven na assignment");
 */
Kruispunt& Kruispunt::operator=(const Kruispunt& other) {
    REQUIRE(properlyInitialized(), "Dit object moet geldig zijn voor assignment");
    REQUIRE(other.properlyInitialized(), "Te kopiëren kruispunt moet geldig zijn");

    if (this != &other) {
        try {
            banen = other.banen;  // Deep copy van de vector
        } catch (...) {
            // Als kopiëren faalt, behoud huidige staat
        }
    }

    ENSURE(properlyInitialized(), "Object moet geldig blijven na assignment");
    return *this;
}

/**
 * @brief Destructor
 */
Kruispunt::~Kruispunt() {
    // Markeer object als niet meer geldig
    _initCheck = nullptr;
    // Vector wordt automatisch opgeruimd
}

/**
 * @brief Controleer of object correct geïnitialiseerd is
 * @return true als object geldig is, false anders
 */
bool Kruispunt::properlyInitialized() const {
    return _initCheck == this;
}

/**
 * @brief Voeg een baan toe aan het kruispunt
 * @param baanNaam Naam van de baan
 * @param positie Positie op de baan
 * @return True als de baan succesvol werd toegevoegd, false anders
 * @pre REQUIRE(properlyInitialized(), "Kruispunt moet geldig geïnitialiseerd zijn");
 * @pre REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn");
 * @pre REQUIRE(positie >= 0.0, "positie moet positief zijn");
 * @post ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
 */
bool Kruispunt::voegBaanToe(const std::string& baanNaam, double positie) {
    REQUIRE(properlyInitialized(), "Kruispunt moet geldig geïnitialiseerd zijn");
    REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn");
    REQUIRE(positie >= 0.0, "positie moet positief zijn");

    // Controleer of de baan al bestaat
    if (bevatBaan(baanNaam)) {
        ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
        return false;
    }

    try {
        // Voeg veilig toe
        banen.emplace_back(baanNaam, positie);
        ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
        return true;
    } catch (...) {
        // Als toevoegen faalt (bijv. geheugenprobleem)
        ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
        return false;
    }
}

/**
 * @brief Krijg alle banen die verbonden zijn met dit kruispunt
 * @return Vector van paren met baannamen en posities
 * @pre REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij getBanen");
 * @post ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
 */
std::vector<std::pair<std::string, double>> Kruispunt::getBanen() const {
    REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij getBanen");

    std::vector<std::pair<std::string, double>> result;

    try {
        result.reserve(banen.size());  // Pre-alloceer geheugen

        for (const auto& baan : banen) {
            result.emplace_back(baan.naam, baan.positie);
        }
    } catch (...) {
        // Als er een probleem is, retourneer lege vector
        result.clear();
    }

    ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
    return result;
}

/**
 * @brief Controleer of een baan deel uitmaakt van dit kruispunt
 * @param baanNaam Naam van de baan
 * @return True als de baan deel uitmaakt van dit kruispunt, false anders
 * @pre REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij bevatBaan");
 * @pre REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn");
 * @post ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
 */
bool Kruispunt::bevatBaan(const std::string& baanNaam) const {
    REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij bevatBaan");
    REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn");

    bool result = false;
    try {
        result = std::any_of(banen.begin(), banen.end(),
                            [&baanNaam](const BaanPositie& baan) {
                                return baan.naam == baanNaam;
                            });
    } catch (...) {
        result = false;
    }

    ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
    return result;
}

/**
 * @brief Krijg positie op een specifieke baan
 * @param baanNaam Naam van de baan
 * @return Positie op de baan, -1 als baan niet gevonden
 * @pre REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij getPositieOpBaan");
 * @pre REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn");
 * @post ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
 */
double Kruispunt::getPositieOpBaan(const std::string& baanNaam) const {
    REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij getPositieOpBaan");
    REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn");

    double result = -1.0;
    try {
        for (const auto& baan : banen) {
            if (baan.naam == baanNaam) {
                result = baan.positie;
                break;
            }
        }
    } catch (...) {
        result = -1.0;
    }

    ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
    return result;
}

/**
 * @brief Kies een willekeurige baan om verder te gaan vanaf het kruispunt
 * @param huidigeWeg Huidige baannaam (om uit te sluiten van mogelijkheden)
 * @return Naam van de gekozen baan, lege string als er geen geldige baan bestaat
 * @pre REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij kiesRandomBaan");
 * @pre REQUIRE(!huidigeWeg.empty(), "huidigeWeg mag niet leeg zijn");
 * @post ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
 */
std::string Kruispunt::kiesRandomBaan(const std::string& huidigeWeg) const {
    REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij kiesRandomBaan");
    REQUIRE(!huidigeWeg.empty(), "huidigeWeg mag niet leeg zijn");

    std::string result = "";
    try {
        // Verzamel potentiële banen
        std::vector<std::string> mogelijkeBanen;
        mogelijkeBanen.reserve(banen.size());

        for (const auto& baan : banen) {
            if (baan.naam != huidigeWeg) {
                mogelijkeBanen.push_back(baan.naam);
            }
        }

        // Als er geen geldige banen zijn
        if (!mogelijkeBanen.empty()) {
            // Kies willekeurig
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<size_t> distr(0, mogelijkeBanen.size() - 1);

            result = mogelijkeBanen[distr(gen)];
        }

    } catch (...) {
        result = "";
    }

    ENSURE(properlyInitialized(), "Kruispunt moet geldig blijven na operatie");
    return result;
}