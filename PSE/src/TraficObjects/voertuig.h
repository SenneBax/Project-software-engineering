/**
 * @file voertuig.h
 * @brief Header voor de Voertuig klasse (Herzien met polymorfisme)
 */

#ifndef VOERTUIG_H
#define VOERTUIG_H

#include <string>
#include <map>
#include <memory>

/**
 * @brief Abstracte basisklasse die een voertuig in een verkeerssituatie voorstelt
 */
class Voertuig {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     */
    Voertuig(const std::string& baan, double positie);

    /**
     * @brief Constructor met snelheid en versnelling
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     */
    Voertuig(const std::string& baan, double positie, double snelheid, double versnelling);

    /**
     * @brief Virtual destructor voor correcte afhandeling van afgeleide klassen
     */
    virtual ~Voertuig();

    /**
     * @brief Copy constructor
     * @param other Het te kopiëren voertuig
     */
    Voertuig(const Voertuig& other);

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
     * @brief Alias voor getBaanNaam voor compatibiliteit
     * @return De naam van de baan
     */
    std::string getBaan() const;

    /**
     * @brief Stelt de naam van de baan in
     * @param nieuweNaam De nieuwe baannaam
     */
    void setBaanNaam(const std::string& nieuweNaam);

    /**
     * @brief Alias voor setBaanNaam voor compatibiliteit
     * @param baan De nieuwe baannaam
     */
    void setBaan(const std::string& baan);

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
     * @brief Geeft de snelheid van het voertuig terug
     * @return De snelheid in m/s
     */
    double getSnelheid() const;

    /**
     * @brief Zet de snelheid van het voertuig
     * @param nieuweSnelheid De nieuwe snelheid
     */
    void setSnelheid(double nieuweSnelheid);

    /**
     * @brief Geeft de versnelling van het voertuig terug
     * @return De versnelling in m/s²
     */
    double getVersnelling() const;

    /**
     * @brief Zet de versnelling van het voertuig
     * @param nieuweVersnelling De nieuwe versnelling
     */
    void setVersnelling(double nieuweVersnelling);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return Het type van het voertuig als string
     */
    virtual std::string getType() const = 0;

    /**
     * @brief Controleert of het voertuig een prioriteitsvoertuig is
     * @return True als het voertuig een prioriteitsvoertuig is, false anders
     */
    virtual bool isPrioriteitsvoertuig() const = 0;

    /**
     * @brief Controleert of het voertuig een bus is
     * @return True als het voertuig een bus is, false anders
     */
    virtual bool isBus() const = 0;

    /**
     * @brief Geeft de lengte van het voertuig terug
     * @return De lengte van het voertuig in meters
     */
    virtual double getLengte() const = 0;

    /**
     * @brief Geeft de maximumsnelheid van het voertuig terug
     * @return De maximumsnelheid in m/s
     */
    virtual double getMaxSnelheid() const = 0;

    /**
     * @brief Geeft de maximumversnelling van het voertuig terug
     * @return De maximumversnelling in m/s²
     */
    virtual double getMaxVersnelling() const = 0;

    /**
     * @brief Geeft de maximale remfactor van het voertuig terug
     * @return De maximale remfactor in m/s²
     */
    virtual double getMaxRemFactor() const = 0;

    /**
     * @brief Geeft de minimale volgafstand van het voertuig terug
     * @return De minimale volgafstand in meters
     */
    virtual double getMinVolgafstand() const = 0;

    /**
     * @brief Geeft de visuele representatie van het voertuig terug
     * @return Een karakter dat het voertuig voorstelt
     */
    virtual std::string getVisueleRepresentatie() const = 0;

    /**
     * @brief Berekent de versnelling van het voertuig
     * @param voorliggendVoertuig Pointer naar het voorliggende voertuig
     * @param isEersteVoertuig Of dit het eerste voertuig op de baan is
     * @param verkeersLichtVertraagFactor Factor voor verkeerslicht vertraging
     * @param bushalteVertraagFactor Factor voor bushalte vertraging
     */
    void berekenVersnelling(Voertuig* voorliggendVoertuig, bool isEersteVoertuig,
                           double verkeersLichtVertraagFactor, double bushalteVertraagFactor);

    /**
     * @brief Update de positie en snelheid van het voertuig
     * @param tijdstap De tijdstap voor de update
     */
    void updatePositieEnSnelheid(double tijdstap);

    /**
     * @brief Zet de wachtstatus bij een bushalte
     * @param waiting Of het voertuig wacht bij een bushalte
     */
    void setIsWaitingAtStop(bool waiting);

    /**
     * @brief Geeft de wachtstatus bij een bushalte terug
     * @return True als het voertuig wacht bij een bushalte
     */
    bool getIsWaitingAtStop() const;

    /**
     * @brief Voert een noodstop uit
     */
    void noodStop();

    /**
     * @brief Clone methode voor het maken van kopieën
     * @return Een unique_ptr naar een kopie van het voertuig
     */
    virtual std::unique_ptr<Voertuig> clone() const = 0;

    /**
     * @brief Controleert of het object correct geïnitialiseerd is
     * @return True als het object correct geïnitialiseerd is
     */
    bool properlyInitialized() const;

    /**
     * @brief Factory methode voor het maken van voertuigen
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param type Het type voertuig
     * @return Een unique_ptr naar het nieuwe voertuig
     */
    static std::unique_ptr<Voertuig> maakVoertuig(const std::string& baan, double positie, const std::string& type);

    /**
     * @brief Factory methode voor het maken van voertuigen met snelheid en versnelling
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     * @param type Het type voertuig
     * @return Een unique_ptr naar het nieuwe voertuig
     */
    static std::unique_ptr<Voertuig> maakVoertuig(const std::string& baan, double positie,
                                                 double snelheid, double versnelling, const std::string& type);

private:
    std::string baanNaam;           ///< Naam van de baan waar het voertuig zich bevindt
    double positie;                 ///< Positie van het voertuig op de baan
    double snelheid;                ///< Snelheid van het voertuig
    double versnelling;             ///< Versnelling van het voertuig
    bool isWaitingAtStop;           ///< Of het voertuig wacht bij een bushalte

    Voertuig* _initCheck;
};

/**
 * @brief Auto class - specifieke implementatie voor personenauto's
 */
class Auto : public Voertuig {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     */
    Auto(const std::string& baan, double positie);

    /**
     * @brief Constructor met snelheid en versnelling
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     */
    Auto(const std::string& baan, double positie, double snelheid, double versnelling);

    /**
     * @brief Implementatie van getType voor Auto
     * @return "auto" als string
     */
    std::string getType() const override;

    /**
     * @brief Implementatie van isPrioriteitsvoertuig voor Auto
     * @return false (auto's zijn geen prioriteitsvoertuigen)
     */
    bool isPrioriteitsvoertuig() const override;

    /**
     * @brief Implementatie van isBus voor Auto
     * @return false (auto's zijn geen bussen)
     */
    bool isBus() const override;

    /**
     * @brief Implementatie van getLengte voor Auto
     * @return De lengte van een auto in meters
     */
    double getLengte() const override;

    /**
     * @brief Implementatie van getMaxSnelheid voor Auto
     * @return De maximumsnelheid van een auto in m/s
     */
    double getMaxSnelheid() const override;

    /**
     * @brief Implementatie van getMaxVersnelling voor Auto
     * @return De maximumversnelling van een auto in m/s²
     */
    double getMaxVersnelling() const override;

    /**
     * @brief Implementatie van getMaxRemFactor voor Auto
     * @return De maximale remfactor van een auto in m/s²
     */
    double getMaxRemFactor() const override;

    /**
     * @brief Implementatie van getMinVolgafstand voor Auto
     * @return De minimale volgafstand van een auto in meters
     */
    double getMinVolgafstand() const override;

    /**
     * @brief Implementatie van getVisueleRepresentatie voor Auto
     * @return "A" als visuele representatie
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Clone method implementatie voor Auto
     * @return Een unieke pointer naar een nieuwe Auto met dezelfde eigenschappen
     */
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Bus class - specifieke implementatie voor bussen
 */
class Bus : public Voertuig {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     */
    Bus(const std::string& baan, double positie);

    /**
     * @brief Constructor met snelheid en versnelling
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     */
    Bus(const std::string& baan, double positie, double snelheid, double versnelling);

    std::string getType() const override;
    bool isPrioriteitsvoertuig() const override;
    bool isBus() const override;
    double getLengte() const override;
    double getMaxSnelheid() const override;
    double getMaxVersnelling() const override;
    double getMaxRemFactor() const override;
    double getMinVolgafstand() const override;
    std::string getVisueleRepresentatie() const override;
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Brandweerwagen class - specifieke implementatie voor brandweerwagens
 */
class Brandweerwagen : public Voertuig {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     */
    Brandweerwagen(const std::string& baan, double positie);

    /**
     * @brief Constructor met snelheid en versnelling
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     */
    Brandweerwagen(const std::string& baan, double positie, double snelheid, double versnelling);

    std::string getType() const override;
    bool isPrioriteitsvoertuig() const override;
    bool isBus() const override;
    double getLengte() const override;
    double getMaxSnelheid() const override;
    double getMaxVersnelling() const override;
    double getMaxRemFactor() const override;
    double getMinVolgafstand() const override;
    std::string getVisueleRepresentatie() const override;
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Ziekenwagen class - specifieke implementatie voor ziekenwagens
 */
class Ziekenwagen : public Voertuig {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     */
    Ziekenwagen(const std::string& baan, double positie);

    /**
     * @brief Constructor met snelheid en versnelling
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     */
    Ziekenwagen(const std::string& baan, double positie, double snelheid, double versnelling);

    std::string getType() const override;
    bool isPrioriteitsvoertuig() const override;
    bool isBus() const override;
    double getLengte() const override;
    double getMaxSnelheid() const override;
    double getMaxVersnelling() const override;
    double getMaxRemFactor() const override;
    double getMinVolgafstand() const override;
    std::string getVisueleRepresentatie() const override;
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Politiecombi class - specifieke implementatie voor politiecombis
 */
class Politiecombi : public Voertuig {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     */
    Politiecombi(const std::string& baan, double positie);

    /**
     * @brief Constructor met snelheid en versnelling
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     */
    Politiecombi(const std::string& baan, double positie, double snelheid, double versnelling);

    std::string getType() const override;
    bool isPrioriteitsvoertuig() const override;
    bool isBus() const override;
    double getLengte() const override;
    double getMaxSnelheid() const override;
    double getMaxVersnelling() const override;
    double getMaxRemFactor() const override;
    double getMinVolgafstand() const override;
    std::string getVisueleRepresentatie() const override;
    std::unique_ptr<Voertuig> clone() const override;
};

#endif // VOERTUIG_H