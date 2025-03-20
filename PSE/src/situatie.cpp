/**
 * @file situatie.cpp
 * @author Senne
 * @date 11/03/2025
 * @brief Implementatie van de VerkeersSituatie klasse
 */

#include "situatie.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include "tinyxml.h"

/**
 * @brief Methode om een baan toe te voegen
 * @param baan De toe te voegen baan
 * @return true als de toevoeging succesvol was, false als de baan al bestaat
 */
bool VerkeersSituatie::voegBaanToe(const Baan& baan) {
    // Controleer of de baan al bestaat
    if (banen.find(baan.getNaam()) != banen.end()) {
        return false;
    }

    // Voeg de baan toe
    banen[baan.getNaam()] = baan;
    return true;
}

/**
 * @brief Methode om een voertuig toe te voegen
 * @param voertuig Het toe te voegen voertuig
 * @return true als de toevoeging succesvol was, false als de baan niet bestaat of de positie ongeldig is
 */
bool VerkeersSituatie::voegVoertuigToe(const Voertuig& voertuig) {
    // Controleer of de baan bestaat
    if (banen.find(voertuig.getBaan()) == banen.end()) {
        return false;
    }

    // Controleer of de positie geldig is
    const Baan& baan = banen[voertuig.getBaan()];
    if (voertuig.getPositie() < 0 || voertuig.getPositie() > baan.getLengte()) {
        return false;
    }

    // Voeg het voertuig toe
    voertuigen.push_back(voertuig);
    return true;
}

/**
 * @brief Methode om een verkeerslicht toe te voegen
 * @param verkeerslicht Het toe te voegen verkeerslicht
 * @return true als de toevoeging succesvol was, false als de baan niet bestaat, de positie ongeldig is of de cyclus ongeldig is
 */
bool VerkeersSituatie::voegVerkeerslichtToe(const Verkeerslicht& verkeerslicht) {
    // Controleer of de baan bestaat
    if (banen.find(verkeerslicht.getBaan()) == banen.end()) {
        return false;
    }

    // Controleer of de positie geldig is
    const Baan& baan = banen[verkeerslicht.getBaan()];
    if (verkeerslicht.getPositie() < 0 || verkeerslicht.getPositie() > baan.getLengte()) {
        return false;
    }

    // Controleer of de cyclus geldig is
    if (verkeerslicht.getCyclus() <= 0) {
        return false;
    }

    // Voeg het verkeerslicht toe
    verkeerslichten.push_back(verkeerslicht);
    return true;
}

/**
 * @brief Methode om consistentie te verifiëren
 * @return true als de verkeerssituatie consistent is, anders false
 */
bool VerkeersSituatie::verificeerConsistentie() const {
    bool isConsistent = true;

    // Controleer of er banen zijn
    if (banen.empty()) {
        isConsistent = false;
    }

    // Controleer of alle voertuigen op geldige banen staan
    for (const Voertuig& voertuig : voertuigen) {
        if (banen.find(voertuig.getBaan()) == banen.end()) {
            isConsistent = false;
        }
    }

    // Controleer of alle verkeerslichten op geldige banen staan
    for (const Verkeerslicht& verkeerslicht : verkeerslichten) {
        auto it = banen.find(verkeerslicht.getBaan());
        if (it == banen.end()) {
            isConsistent = false;
        } else {
            const Baan& baan = it->second;
            if (verkeerslicht.getPositie() < 0 || verkeerslicht.getPositie() > baan.getLengte()) {
                isConsistent = false;
            }
        }
    }

    return isConsistent;
}

/**
 * @brief Methode om een voertuig te verwijderen
 * @param index De index van het te verwijderen voertuig
 * @return true als het verwijderen succesvol was, false als de index ongeldig is
 */
bool VerkeersSituatie::verwijderVoertuig(int index) {
    if (index < 0 || index >= static_cast<int>(voertuigen.size())) {
        return false;
    }

    voertuigen.erase(voertuigen.begin() + index);
    return true;
}

/**
 * @brief Getter voor de map met banen (const versie)
 * @return Constante referentie naar de map met banen
 */
const std::map<std::string, Baan>& VerkeersSituatie::getBanen() const {
    return banen;
}

/**
 * @brief Getter voor de map met banen (non-const versie)
 * @return Referentie naar de map met banen
 */
std::map<std::string, Baan>& VerkeersSituatie::getBanen() {
    return banen;
}

/**
 * @brief Getter voor de vector met voertuigen (const versie)
 * @return Constante referentie naar de vector met voertuigen
 */
const std::vector<Voertuig>& VerkeersSituatie::getVoertuigen() const {
    return voertuigen;
}

/**
 * @brief Getter voor de vector met voertuigen (non-const versie)
 * @return Referentie naar de vector met voertuigen
 */
std::vector<Voertuig>& VerkeersSituatie::getVoertuigen() {
    return voertuigen;
}

/**
 * @brief Getter voor de vector met verkeerslichten (const versie)
 * @return Constante referentie naar de vector met verkeerslichten
 */
const std::vector<Verkeerslicht>& VerkeersSituatie::getVerkeerslichten() const {
    return verkeerslichten;
}

/**
 * @brief Getter voor de vector met verkeerslichten (non-const versie)
 * @return Referentie naar de vector met verkeerslichten
 */
std::vector<Verkeerslicht>& VerkeersSituatie::getVerkeerslichten() {
    return verkeerslichten;
}