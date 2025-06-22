/**
 * @file kruispunt.h
 * @brief Definitie van de kruispunt-klasse
 */

#ifndef KRUISPUNT_H
#define KRUISPUNT_H

#include <string>
#include <vector>
#include <utility>

/**
 * @class Kruispunt
 * @brief Klasse die een kruispunt in de verkeerssimulatie voorstelt
 *
 * Kruispunten bevatten meerdere banen met een specifieke positie
 * biedt een functionaliteit om iets op te vragen of toe tevoegen.
 */
class Kruispunt {
private:
    Kruispunt* _initCheck;
    struct BaanPositie {
        std::string naam;   /**< Naam van de baan */
        double positie;     /**< Positie op de baan */


        BaanPositie(const std::string& naam, double positie) : naam(naam), positie(positie) {}
    };

    std::vector<BaanPositie> banen;  /**< Lijst van banen verbonden met dit kruispunt */

public:
    /**
     * @brief Constructor voor het kruispunt
     * @post properlyInitialized() == true
     */
    Kruispunt();

    /**
     * @brief Voeg een baan toe aan het kruispunt
     * @param baanNaam Naam van de baan
     * @param positie Positie op de baan
     * @return True als de baan succesvol werd toegevoegd, false indien niet
     * @pre properlyInitialized() == true
     * @pre !baanNaam.empty()
     * @pre positie >= 0.0
     * @post indien true: baan toegevoegd aan getBanen()
     * @post indien false: bevatBaan(baanNaam) al true
     */
    bool voegBaanToe(const std::string& baanNaam, double positie);

    /**
     * @brief Haal alle banen op die verbonden zijn met dit kruispunt
     * @return Vector van paren met baannamen en posities
     * @pre properlyInitialized() == true
     * @post return bevat alle toegevoegde banen
     */
    std::vector<std::pair<std::string, double>> getBanen() const;

    /**
     * @brief Controleer of een baan deel uitmaakt van dit kruispunt
     * @param baanNaam Naam van de baan
     * @return True als de baan deel uitmaakt van dit kruispunt, false indien niet
     * @pre properlyInitialized() == true
     * @pre !baanNaam.empty()
     * @post return == true <=> baan aanwezig in getBanen()
     */
    bool bevatBaan(const std::string& baanNaam) const;

    /**
     * @brief Haal positie op op een specifieke baan
     * @param baanNaam Naam van de baan
     * @return Positie op de baan, -1 als baan niet gevonden
     * @pre properlyInitialized() == true
     * @pre !baanNaam.empty()
     * @post return >= 0.0 als gevonden, anders -1.0
     */
    double getPositieOpBaan(const std::string& baanNaam) const;

    /**
     * @brief Kies een willekeurige baan om verder te gaan vanaf het kruispunt
     * @param huidigeWeg Huidige baannaam (om uit te sluiten van mogelijkheden)
     * @return Naam van de gekozen baan, lege string als er geen geldige baan bestaat
     * @pre properlyInitialized() == true
     * @pre !huidigeWeg.empty()
     * @post return is leeg of verschilt van huidigeWeg
     */
    std::string kiesRandomBaan(const std::string& huidigeWeg) const;

    /**
     * @brief Controleer of het VerkeersSituatie object correct is geïnitialiseerd
     * @return true als het object correct is geïnitialiseerd, false anders
     * @post return waarde geeft aan of object in geldige toestand verkeert
     *
     * Design by Contract verificatie methode om object integriteit
     * te controleren.
     */
    bool properlyInitialized() const;
};

#endif // KRUISPUNT_H