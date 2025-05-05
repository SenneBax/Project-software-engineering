/**
 * @file verkeerslicht.h
 * @brief Definitie van de verkeerslicht klasse (Herzien met ondersteuning voor oranje licht)
 */

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H

#include <string>

/**
 * @class Verkeerslicht
 * @brief Klasse die een verkeerslicht in de verkeerssimulatie voorstelt
 */
class Verkeerslicht {
public:
    /**
     * @brief Enum voor de verkeerslicht kleuren
     */
    enum class Kleur {
        GROEN,
        ORANJE,
        ROOD
    };

private:
    std::string baan;         /**< Naam van de baan waar het verkeerslicht zich bevindt */
    double positie;           /**< Positie van het verkeerslicht op de baan in meters */
    int cyclus;               /**< Cyclustijd van het verkeerslicht in seconden */
    double tijdSindsLaatsteWissel; /**< Tijd sinds de laatste kleurverandering */
    Kleur huidigeKleur;       /**< Huidige kleur van het verkeerslicht */
    bool heeftOranje;         /**< Of dit verkeerslicht een oranje status heeft */
    bool isSlim;              /**< Of dit een slim verkeerslicht is */
    int voertuigenVoorLicht;  /**< Aantal voertuigen wachtend voor het licht */
    Verkeerslicht* _initCheck;



public:
    /**
     * @brief Constructor voor het verkeerslicht
     * @param baan Naam van de baan
     * @param positie Positie op de baan in meters
     * @param cyclus Cyclustijd in seconden
     * @param heeftOranje Of dit verkeerslicht een oranje status heeft
     * @param isSlim Of dit een slim verkeerslicht is
     */
    Verkeerslicht(const std::string& baan, double positie, int cyclus, bool heeftOranje = false, bool isSlim = false);

    /**
     * @brief Getter voor de baannaam van het verkeerslicht
     * @return De naam van de baan
     */
    bool properlyInitialized() const;

    std::string getBaan() const;

    /**
     * @brief Getter voor de positie van het verkeerslicht
     * @return De positie in meters
     */
    double getPositie() const;

    /**
     * @brief Getter voor de cyclustijd van het verkeerslicht
     * @return De cyclustijd in seconden
     */
    int getCyclus() const;

    /**
     * @brief Getter voor de huidige kleur van het verkeerslicht
     * @return De huidige kleur
     */
    Kleur getKleur() const;

    /**
     * @brief Setter voor de huidige kleur van het verkeerslicht
     * @param kleur De nieuwe kleur
     */
    void setKleur(Kleur kleur);

    /**
     * @brief Haal kleur op als string
     * @return String-weergave van de huidige kleur
     */
    std::string getKleurAsString() const;

    /**
     * @brief Controleert of het verkeerslicht rood is
     * @return True als het licht rood is, false indien niet
     */
    bool isRood() const;

    /**
     * @brief Controleert of het verkeerslicht oranje is
     * @return True als het licht oranje is, false indien niet
     */
    bool isOranje() const;

    /**
     * @brief Controleert of het verkeerslicht groen is
     * @return True als het licht groen is, false indien niet
     */
    bool isGroen() const;

    /**
     * @brief Controleert of dit verkeerslicht een oranje status heeft
     * @return True als het licht een oranje status heeft, false indien niet
     */
    bool getHeeftOranje() const;

    /**
     * @brief Controleert of dit een slim verkeerslicht is
     * @return True als het licht slim is, false indien niet
     */
    bool getIsSlim() const;

    /**
     * @brief Update het verkeerslicht op basis van verstreken tijd
     * @param tijdstap De huidige simulatie-tijdstap
     */
    void update(double tijdstap);

    /**
     * @brief Registreer een voertuig wachtend voor dit licht (voor slimme verkeerslichten)
     */
    void registerVoertuigVoorLicht();

    /**
     * @brief Deregistreer een voertuig dat aan dit licht wachtte (voor slimme verkeerslichten)
     */
    void unregisterVoertuigVoorLicht();

    /**
     * @brief Haal het aantal voertuigen op dat voor dit licht wacht
     * @return Het aantal voertuigen
     */
    int getVoertuigenVoorLicht() const;

    /**
     * @brief Reset het aantal voertuigen dat voor dit licht wacht
     */
    void resetVoertuigenVoorLicht();


};

#endif // VERKEERSLICHT_H