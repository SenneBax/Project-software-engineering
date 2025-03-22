/**
 * @file situatie.h
 * @author Senne
 * @date 11/03/2025
 * @brief Header voor de VerkeersSituatie klasse
 */

#ifndef SITUATIE_H
#define SITUATIE_H

#include <string>
#include <vector>
#include <map>
#include "baan.h"
#include "voertuig.h"
#include "verkeerslicht.h"
#include "voertuiggenerator.h"

/**
 * @brief Klasse die een verkeerssituatie representeert
 */
class VerkeersSituatie {
public:
    /**
     * @brief Methode om een baan toe te voegen
     * @param baan De toe te voegen baan
     * @return true als de toevoeging succesvol was, false als de baan al bestaat
     */
    bool voegBaanToe(const Baan& baan);

    /**
     * @brief Methode om een voertuig toe te voegen
     * @param voertuig Het toe te voegen voertuig
     * @return true als de toevoeging succesvol was, false als de baan niet bestaat of de positie ongeldig is
     */
    bool voegVoertuigToe(const Voertuig& voertuig);

    /**
     * @brief Methode om een verkeerslicht toe te voegen
     * @param verkeerslicht Het toe te voegen verkeerslicht
     * @return true als de toevoeging succesvol was, false als de baan niet bestaat, de positie ongeldig is of de cyclus ongeldig is
     */
    bool voegVerkeerslichtToe(const Verkeerslicht& verkeerslicht);

    /**
     * @brief Methode om consistentie te verifiëren
     * @return true als de verkeerssituatie consistent is, anders false
     */
    bool verificeerConsistentie() const;

    /**
     * @brief Methode om een voertuig te verwijderen
     * @param index De index van het te verwijderen voertuig
     * @return true als het verwijderen succesvol was, false als de index ongeldig is
     */
    bool verwijderVoertuig(int index);

    bool voegVoertuigGeneratorToe(const VoertuigGenerator& voertuiggenerator);

    /**
     * @brief Getter voor de map met banen (const versie)
     * @return Constante referentie naar de map met banen
     */
    const std::map<std::string, Baan>& getBanen() const;

    /**
     * @brief Getter voor de map met banen (non-const versie)
     * @return Referentie naar de map met banen
     */
    std::map<std::string, Baan>& getBanen();

    /**
     * @brief Getter voor de vector met voertuigen (const versie)
     * @return Constante referentie naar de vector met voertuigen
     */
    const std::vector<Voertuig>& getVoertuigen() const;

    /**
     * @brief Getter voor de vector met voertuigen (non-const versie)
     * @return Referentie naar de vector met voertuigen
     */
    std::vector<Voertuig>& getVoertuigen();

    /**
     * @brief Getter voor de vector met verkeerslichten (const versie)
     * @return Constante referentie naar de vector met verkeerslichten
     */
    const std::vector<Verkeerslicht>& getVerkeerslichten() const;

    /**
     * @brief Getter voor de vector met verkeerslichten (non-const versie)
     * @return Referentie naar de vector met verkeerslichten
     */
    std::vector<Verkeerslicht>& getVerkeerslichten();

private:
    std::map<std::string, Baan> banen;              ///< Map met banen (key: naam, value: Baan object)
    std::vector<Voertuig> voertuigen;               ///< Vector met voertuigen
    std::vector<Verkeerslicht> verkeerslichten;     ///< Vector met verkeerslichten
};

#endif // SITUATIE_H