/**
 * @file voertuig.h
 * @author senne
 * @date 19/03/2025
 * @brief Header voor de Voertuig klasse
 */

#ifndef VOERTUIG_H
#define VOERTUIG_H

#include <string>

/**
 * @brief Klasse die een voertuig in een verkeerssituatie voorstelt
 */
class Voertuig {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param type Het type van het voertuig (auto, bus, brandweerwagen, ziekenwagen, politiecombi)
     */
    Voertuig(const std::string& baan, double positie, const std::string& type = "auto");

    /**
     * @brief Copy constructor
     * @param other Het te kopiëren voertuig
     */
    Voertuig(const Voertuig& other);

    /**
     * @brief Destructor
     */
    ~Voertuig();

    /**
     * @brief Assignment operator
     * @param other Het voertuig waarvan de waarden worden overgenomen
     * @return Referentie naar dit voertuig
     */
    Voertuig& operator=(const Voertuig& other);

    /**
     * @brief Geeft de naam van de baan terug
     * @return De naam van de baan
     */
    std::string getBaanNaam() const;

    /**
     * @brief Geeft de positie van het voertuig terug
     * @return De positie op de baan
     */
    double getPositie() const;

    /**
     * @brief Zet de positie van het voertuig
     * @param nieuwePositie De nieuwe positie
     */
    void setPositie(double nieuwePositie);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return Het type van het voertuig
     */
    std::string getType() const;

    /**
     * @brief Beweegt het voertuig vooruit
     * @param tijdstap De tijdstap in seconden
     * @param maxSnelheid De maximale snelheid in meter per seconde
     * @param maxPositie De maximale positie op de baan
     * @return De nieuwe positie
     */
    double beweeg(double tijdstap, double maxSnelheid, double maxPositie);

    /**
     * @brief Controleert of dit voertuig botst met een ander voertuig
     * @param ander Het andere voertuig
     * @param veiligheidsAfstand De minimale veiligheidsafstand tussen voertuigen
     * @return true als er een botsing is, anders false
     */
    bool botst(const Voertuig& ander, double veiligheidsAfstand) const;

    /**
     * @brief Berekent de snelheid van het voertuig gebaseerd op het type
     * @param basisSnelheid De basissnelheid voor normale voertuigen
     * @return De aangepaste snelheid gebaseerd op het type voertuig
     */
    double berekenSnelheid(double basisSnelheid) const;

private:
    std::string baanNaam; ///< Naam van de baan waar het voertuig zich bevindt
    double positie;       ///< Positie van het voertuig op de baan
    std::string type;     ///< Type van het voertuig
};

#endif // VOERTUIG_H