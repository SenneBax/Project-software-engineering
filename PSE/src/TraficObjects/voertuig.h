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
     * @return True als het voertuig een prioriteitsvoertuig is (brandweerwagen, ziekenwagen, politie)
     */
    virtual bool isPrioriteitsvoertuig() const = 0;

    /**
     * @brief Controleert of dit voertuig een bus is
     * @return True als het voertuig een bus is
     */
    virtual bool isBus() const = 0;

    /**
     * @brief Haalt de lengte van het voertuig op
     * @return De lengte in meters
     */
    virtual double getLengte() const = 0;

    /**
     * @brief Haalt de maximumsnelheid van het voertuig op
     * @return De maximumsnelheid in m/s
     */
    virtual double getMaxSnelheid() const = 0;

    /**
     * @brief Haalt de maximumversnelling van het voertuig op
     * @return De maximumversnelling in m/s²
     */
    virtual double getMaxVersnelling() const = 0;

    /**
     * @brief Haalt de maximale remfactor van het voertuig op
     * @return De maximale remfactor in m/s²
     */
    virtual double getMaxRemFactor() const = 0;

    /**
     * @brief Haalt de minimale volgafstand op
     * @return De minimale volgafstand in meters
     */
    virtual double getMinVolgafstand() const = 0;

    /**
     * @brief Stelt de wachtende bus vlag in
     * @param isWaiting Of de bus wacht bij een halte
     */
    void setIsWaitingAtBusStop(bool isWaiting);

    /**
     * @brief Controleert of de bus wacht bij een halte
     * @return True als de bus wacht, anders false
     */
    bool isWaitingAtBusStop() const;

    /**
     * @brief Controleert of het voertuig momenteel wacht bij een halte
     * @return true als het voertuig wacht bij een bushalte of verkeerslicht, false anders
     */
    bool isWaitingAtStop() const;

    /**
     * @brief Stelt in of het voertuig wacht bij een halte
     * @param waiting Of het voertuig wacht
     */
    void setIsWaitingAtStop(bool waiting);

    /**
     * @brief Update de positie en snelheid van het voertuig op basis van de huidige versnelling
     * @param tijdstap De tijdstap voor de update in seconden
     */
    void updatePositieEnSnelheid(double tijdstap);

    /**
     * @brief Bereken de versnelling van het voertuig op basis van voorliggend voertuig en andere factoren
     * @param voorgaandVoertuig Het voorliggende voertuig, nullptr als er geen voorliggend voertuig is
     * @param isEersteVoertuig Of dit voertuig het eerste is in de rij
     * @param verkeersLichtVertraagFactor Factor voor vertraging door verkeerslicht
     * @param doelSnelheid De gewenste snelheid (negatief voor maximumsnelheid)
     */
    void berekenVersnelling(const Voertuig* voorgaandVoertuig, bool isEersteVoertuig,
                           double verkeersLichtVertraagFactor, double doelSnelheid);

    /**
     * @brief Voer een noodstop uit
     */
    void noodStop();

    /**
     * @brief Geeft de visuele representatie van het voertuig terug
     * @return Karakter dat het voertuig voorstelt
     */
    virtual std::string getVisueleRepresentatie() const = 0;

    /**
     * @brief Factory method om voertuigen te maken op basis van type
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param type Het type voertuig als string
     * @return Unieke pointer naar een nieuw voertuig
     */
    static std::unique_ptr<Voertuig> maakVoertuig(const std::string& baan, double positie,
                                                 const std::string& type);

    /**
     * @brief Factory method om voertuigen te maken op basis van type met snelheid en versnelling
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     * @param type Het type voertuig als string
     * @return Unieke pointer naar een nieuw voertuig
     */
    static std::unique_ptr<Voertuig> maakVoertuig(const std::string& baan, double positie,
                                                 double snelheid, double versnelling,
                                                 const std::string& type);

    /**
     * @brief Clone method voor het maken van een kopie van dit voertuig
     * @return Een unieke pointer naar een nieuw voertuig met dezelfde eigenschappen
     */
    virtual std::unique_ptr<Voertuig> clone() const = 0;

    /**
     * @brief Controleren of het object goed is geïnitialiseerd
     * @return true als _initCheck == this
     */
    bool properlyInitialized() const;

protected:
    std::string baanNaam; ///< Naam van de baan waar het voertuig zich bevindt
    double positie;       ///< Positie van het voertuig op de baan
    double snelheid;      ///< Snelheid van het voertuig in m/s
    double versnelling;   ///< Versnelling van het voertuig in m/s²
    bool waitingAtStop;   ///< Of het voertuig wacht bij een halte

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

    /**
     * @brief Implementatie van getType voor Bus
     * @return "bus" als string
     */
    std::string getType() const override;

    /**
     * @brief Implementatie van isPrioriteitsvoertuig voor Bus
     * @return false (bussen zijn geen prioriteitsvoertuigen)
     */
    bool isPrioriteitsvoertuig() const override;

    /**
     * @brief Implementatie van isBus voor Bus
     * @return true (dit is een bus)
     */
    bool isBus() const override;

    /**
     * @brief Implementatie van getLengte voor Bus
     * @return De lengte van een bus in meters
     */
    double getLengte() const override;

    /**
     * @brief Implementatie van getMaxSnelheid voor Bus
     * @return De maximumsnelheid van een bus in m/s
     */
    double getMaxSnelheid() const override;

    /**
     * @brief Implementatie van getMaxVersnelling voor Bus
     * @return De maximumversnelling van een bus in m/s²
     */
    double getMaxVersnelling() const override;

    /**
     * @brief Implementatie van getMaxRemFactor voor Bus
     * @return De maximale remfactor van een bus in m/s²
     */
    double getMaxRemFactor() const override;

    /**
     * @brief Implementatie van getMinVolgafstand voor Bus
     * @return De minimale volgafstand van een bus in meters
     */
    double getMinVolgafstand() const override;

    /**
     * @brief Implementatie van getVisueleRepresentatie voor Bus
     * @return "B" als visuele representatie
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Clone method implementatie voor Bus
     * @return Een unieke pointer naar een nieuwe Bus met dezelfde eigenschappen
     */
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

    /**
     * @brief Implementatie van getType voor Brandweerwagen
     * @return "brandweerwagen" als string
     */
    std::string getType() const override;

    /**
     * @brief Implementatie van isPrioriteitsvoertuig voor Brandweerwagen
     * @return true (brandweerwagens zijn prioriteitsvoertuigen)
     */
    bool isPrioriteitsvoertuig() const override;

    /**
     * @brief Implementatie van isBus voor Brandweerwagen
     * @return false (brandweerwagens zijn geen bussen)
     */
    bool isBus() const override;

    /**
     * @brief Implementatie van getLengte voor Brandweerwagen
     * @return De lengte van een brandweerwagen in meters
     */
    double getLengte() const override;

    /**
     * @brief Implementatie van getMaxSnelheid voor Brandweerwagen
     * @return De maximumsnelheid van een brandweerwagen in m/s
     */
    double getMaxSnelheid() const override;

    /**
     * @brief Implementatie van getMaxVersnelling voor Brandweerwagen
     * @return De maximumversnelling van een brandweerwagen in m/s²
     */
    double getMaxVersnelling() const override;

    /**
     * @brief Implementatie van getMaxRemFactor voor Brandweerwagen
     * @return De maximale remfactor van een brandweerwagen in m/s²
     */
    double getMaxRemFactor() const override;

    /**
     * @brief Implementatie van getMinVolgafstand voor Brandweerwagen
     * @return De minimale volgafstand van een brandweerwagen in meters
     */
    double getMinVolgafstand() const override;

    /**
     * @brief Implementatie van getVisueleRepresentatie voor Brandweerwagen
     * @return "F" als visuele representatie
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Clone method implementatie voor Brandweerwagen
     * @return Een unieke pointer naar een nieuwe Brandweerwagen met dezelfde eigenschappen
     */
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

    /**
     * @brief Implementatie van getType voor Ziekenwagen
     * @return "ziekenwagen" als string
     */
    std::string getType() const override;

    /**
     * @brief Implementatie van isPrioriteitsvoertuig voor Ziekenwagen
     * @return true (ziekenwagens zijn prioriteitsvoertuigen)
     */
    bool isPrioriteitsvoertuig() const override;

    /**
     * @brief Implementatie van isBus voor Ziekenwagen
     * @return false (ziekenwagens zijn geen bussen)
     */
    bool isBus() const override;

    /**
     * @brief Implementatie van getLengte voor Ziekenwagen
     * @return De lengte van een ziekenwagen in meters
     */
    double getLengte() const override;

    /**
     * @brief Implementatie van getMaxSnelheid voor Ziekenwagen
     * @return De maximumsnelheid van een ziekenwagen in m/s
     */
    double getMaxSnelheid() const override;

    /**
     * @brief Implementatie van getMaxVersnelling voor Ziekenwagen
     * @return De maximumversnelling van een ziekenwagen in m/s²
     */
    double getMaxVersnelling() const override;

    /**
     * @brief Implementatie van getMaxRemFactor voor Ziekenwagen
     * @return De maximale remfactor van een ziekenwagen in m/s²
     */
    double getMaxRemFactor() const override;

    /**
     * @brief Implementatie van getMinVolgafstand voor Ziekenwagen
     * @return De minimale volgafstand van een ziekenwagen in meters
     */
    double getMinVolgafstand() const override;

    /**
     * @brief Implementatie van getVisueleRepresentatie voor Ziekenwagen
     * @return "Z" als visuele representatie
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Clone method implementatie voor Ziekenwagen
     * @return Een unieke pointer naar een nieuwe Ziekenwagen met dezelfde eigenschappen
     */
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Politiecombi class - specifieke implementatie voor politiecombi's
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

    /**
     * @brief Implementatie van getType voor Politiecombi
     * @return "politiecombi" als string
     */
    std::string getType() const override;

    /**
     * @brief Implementatie van isPrioriteitsvoertuig voor Politiecombi
     * @return true (politiecombi's zijn prioriteitsvoertuigen)
     */
    bool isPrioriteitsvoertuig() const override;

    /**
     * @brief Implementatie van isBus voor Politiecombi
     * @return false (politiecombi's zijn geen bussen)
     */
    bool isBus() const override;

    /**
     * @brief Implementatie van getLengte voor Politiecombi
     * @return De lengte van een politiecombi in meters
     */
    double getLengte() const override;

    /**
     * @brief Implementatie van getMaxSnelheid voor Politiecombi
     * @return De maximumsnelheid van een politiecombi in m/s
     */
    double getMaxSnelheid() const override;

    /**
     * @brief Implementatie van getMaxVersnelling voor Politiecombi
     * @return De maximumversnelling van een politiecombi in m/s²
     */
    double getMaxVersnelling() const override;

    /**
     * @brief Implementatie van getMaxRemFactor voor Politiecombi
     * @return De maximale remfactor van een politiecombi in m/s²
     */
    double getMaxRemFactor() const override;

    /**
     * @brief Implementatie van getMinVolgafstand voor Politiecombi
     * @return De minimale volgafstand van een politiecombi in meters
     */
    double getMinVolgafstand() const override;

    /**
     * @brief Implementatie van getVisueleRepresentatie voor Politiecombi
     * @return "P" als visuele representatie
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Clone method implementatie voor Politiecombi
     * @return Een unieke pointer naar een nieuwe Politiecombi met dezelfde eigenschappen
     */
    std::unique_ptr<Voertuig> clone() const override;
};

#endif // VOERTUIG_H