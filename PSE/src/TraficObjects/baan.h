

#ifndef BAAN_H
#define BAAN_H

#include <string>

using namespace std;

/**
 * @class Baan
 * @brief Klasse voor een baan in de verkeerssimulatie
 *
 * Een Baan object representeert een wegvak met een naam en lengte.
 * Banen zijn de fundamentele infrastructuur elementen waarop voertuigen
 * zich bewegen. Elke baan heeft een unieke naam binnen een verkeerssituatie
 * en een positieve lengte in meters.
 *
 * Design by Contract:
 * - Namen mogen niet leeg zijn
 * - Lengtes moeten positief zijn
 * - Object moet correct geïnitialiseerd zijn via properlyInitialized()
 */
class Baan {
private:
    string naam;   /**< Naam van de baan (moet uniek zijn binnen verkeerssituatie) */
    int lengte;    /**< Lengte van de baan in meters (moet positief zijn) */
    Baan* _initCheck; /**< Pointer voor Design by Contract verificatie */

public:
    /**
     * @brief Standaard constructor voor lege baan
     * @pre Geen specifieke precondities
     * @post properlyInitialized() == true
     * @post getNaam() == ""
     * @post getLengte() == 0
     *
     * Maakt een lege baan aan die later geïnitialiseerd kan worden.
     * Standaard waarden zijn lege naam en lengte 0.
     */
    Baan();

    /**
     * @brief Constructor met parameters voor baan
     * @param naam Naam van de baan
     * @param lengte Lengte van de baan in meters
     * @pre !naam.empty()
     * @pre lengte > 0
     * @post properlyInitialized() == true
     * @post getNaam() == naam
     * @post getLengte() == lengte
     *
     * Maakt een nieuwe baan aan met opgegeven naam en lengte.
     * De naam moet uniek zijn binnen de verkeerssituatie en
     * de lengte moet een positief getal zijn.
     */
    Baan(const string& naam, int lengte);

    /**
     * @brief Getter voor de naam van de baan
     * @return De naam van de baan als string
     * @pre properlyInitialized() == true
     * @post return waarde is de huidige naam van de baan
     * @post return waarde kan leeg zijn bij standaard constructor
     *
     * Haalt de naam op van deze baan. De naam wordt gebruikt
     * voor identificatie binnen de verkeerssituatie.
     */
    [[nodiscard]] string getNaam() const;

    /**
     * @brief Getter voor de lengte van de baan
     * @return De lengte van de baan in meters
     * @pre properlyInitialized() == true
     * @post return waarde >= 0
     * @post return waarde is de huidige lengte in meters
     *
     * Haalt de lengte op van deze baan in meters. Deze wordt
     * gebruikt voor het bepalen van voertuigposities en het
     * detecteren wanneer voertuigen de baan verlaten.
     */
    [[nodiscard]] int getLengte() const;

    /**
     * @brief Controleren of het baan object goed is geïnitialiseerd
     * @return true als _initCheck == this, false anders
     * @post return waarde geeft aan of object in geldige toestand verkeert
     *
     * Design by Contract verificatie methode. Controleert of het
     * object correct is geïnitialiseerd en in een geldige toestand
     * verkeert. Moet true zijn voordat andere methoden aangeroepen worden.
     */
    [[nodiscard]] bool properlyInitialized() const;
};

#endif // BAAN_H