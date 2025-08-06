/**
* @file voertuiggenerator.h
 * @brief Header voor de VoertuigGenerator klasse
 */

#ifndef VOERTUIGGENERATOR_H
#define VOERTUIGGENERATOR_H

#include <string>

/**
 * @brief Klasse die voertuigen genereert op een baan
 */
class VoertuigGenerator {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan
     * @param frequentie Generatiefrequentie (voertuigen/tijdseenheid)
     * @param type Voertuigtype (standaard "auto")
     * @pre !baan.empty(), "Baannaam mag niet leeg zijn"
     * @pre frequentie > 0, "Frequentie moet positief zijn"
     * @post properlyInitialized()
     * @post getBaanNaam() == baan
     * @post getFrequentie() == frequentie
     * @post getType() == type
     */
    VoertuigGenerator(const std::string& baan, int frequentie, const std::string& type = "auto");

    /**
     * @brief Geeft de baannaam terug
     * @return Niet-lege baannaam
     * @pre properlyInitialized()
     * @post !return.empty()
     */
    std::string getBaanNaam() const;

    /**
     * @brief Geeft de generatiefrequentie terug
     * @return Positieve frequentie
     * @pre properlyInitialized()
     * @post return > 0
     */
    int getFrequentie() const;

    /**
     * @brief Geeft het voertuigtype terug
     * @return Geldig voertuigtype
     * @pre properlyInitialized()
     * @post !return.empty()
     */
    std::string getType() const;

   /**
     * @brief Controleer of het VerkeersSituatie object correct is geïnitialiseerd
     * @return true als het object correct is geïnitialiseerd, false anders
     * @post return waarde geeft aan of object in geldige toestand verkeert
     *
     * Design by Contract verificatie methode om object integriteit
     * te controleren.
     */
    bool properlyInitialized() const;



private:
    std::string baanNaam;  ///< Naam van de baan waar voertuigen gegenereerd worden
    int frequentie;        ///< Frequentie waarmee voertuigen gegenereerd worden
    std::string type;      ///< Type voertuigen dat gegenereerd wordt
    VoertuigGenerator* _initCheck;

};

#endif // VOERTUIGGENERATOR_H