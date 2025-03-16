/**
 * @file Situatie.h
 * @brief Header bestand voor de VerkeersSituatie klasse
 */

#ifndef SITUATIE_H
#define SITUATIE_H

#include <map>
#include <vector>
#include <string>
#include "baan.h"
#include "voertuig.h"
#include "verkeerslicht.h"
#include <iostream>

using namespace std;

/**
 * @class VerkeersSituatie
 * @brief Klasse die een verkeerssituatie representeert met banen, voertuigen en verkeerslichten
 */
class VerkeersSituatie {
private:
    map<string, Baan> banen;               /**< Map van alle banen in de verkeerssituatie */
    vector<Voertuig> voertuigen;           /**< Vector van alle voertuigen in de verkeerssituatie */
    vector<Verkeerslicht> verkeerslichten; /**< Vector van alle verkeerslichten in de verkeerssituatie */

public:
    /**
     * @brief Voegt een baan toe aan de verkeerssituatie
     * @param baan De toe te voegen baan
     * @return true als de baan succesvol is toegevoegd, false anders
     */
    bool voegBaanToe(const Baan& baan);

    /**
     * @brief Voegt een voertuig toe aan de verkeerssituatie
     * @param voertuig Het toe te voegen voertuig
     * @return true als het voertuig succesvol is toegevoegd, false anders
     */
    bool voegVoertuigToe(const Voertuig& voertuig);

    /**
     * @brief Voegt een verkeerslicht toe aan de verkeerssituatie
     * @param verkeerslicht Het toe te voegen verkeerslicht
     * @return true als het verkeerslicht succesvol is toegevoegd, false anders
     */
    bool voegVerkeerslichtToe(const Verkeerslicht& verkeerslicht);

    /**
     * @brief Verifieert de consistentie van de verkeerssituatie
     * @return true als de verkeerssituatie consistent is, false anders
     */
    [[nodiscard]] bool verificeerConsistentie() const;

    /**
     * @brief Print informatie over de verkeerssituatie naar de console
     */
    void printInfo() const;

    /**
     * @brief Methodes om de simulatie te ondersteunen
     * @return Referentie naar de vector van voertuigen
     */
    std::vector<Voertuig>& getVoertuigen() { return voertuigen; }

    /**
     * @brief Constant getter voor de vector van voertuigen
     * @return Constante referentie naar de vector van voertuigen
     */
    [[nodiscard]] const std::vector<Voertuig>& getVoertuigen() const { return voertuigen; }

    /**
     * @brief Getter voor de vector van verkeerslichten
     * @return Referentie naar de vector van verkeerslichten
     */
    std::vector<Verkeerslicht>& getVerkeerslichten() { return verkeerslichten; }

    /**
     * @brief Constant getter voor de vector van verkeerslichten
     * @return Constante referentie naar de vector van verkeerslichten
     */
    [[nodiscard]] const std::vector<Verkeerslicht>& getVerkeerslichten() const { return verkeerslichten; }

    /**
     * @brief Constant getter voor de map van banen
     * @return Constante referentie naar de map van banen
     */
    [[nodiscard]] const std::map<string, Baan>& getBanen() const { return banen; }

    /**
     * @brief Methode om een voertuig te verwijderen op een bepaalde plaats
     * @param index De index van het te verwijderen voertuig
     * @return true als het voertuig succesvol is verwijderd, false anders
     */
    bool verwijderVoertuig(int index);
};

/**
 * @brief Leest een verkeerssituatie uit een bestand
 * @param bestandsnaam De naam van het bestand
 * @param situatie Referentie naar een VerkeersSituatie object dat gevuld wordt
 * @return true als het bestand succesvol is gelezen, false anders
 */
bool leesVerkeersSituatie(const string& bestandsnaam, VerkeersSituatie& situatie);

#endif // SITUATIE_H