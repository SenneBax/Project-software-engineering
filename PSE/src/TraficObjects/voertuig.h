/**
 * @file voertuig.h
 * @brief Header voor de Voertuig klasse met polymorfisme en verbeterde veiligheid
 * @author Generated with fixes for segmentation faults
 * @date 2025
 */

#ifndef VOERTUIG_H
#define VOERTUIG_H

#include <string>
#include <map>
#include <memory>

/**
 * @brief Abstracte basisklasse die een voertuig in een verkeerssituatie voorstelt
 *
 * Deze klasse definieert de interface en gemeenschappelijke functionaliteit
 * voor alle voertuigen in het verkeerssimulatie systeem. Voertuigen kunnen
 * verschillende types zijn (auto, bus, brandweerwagen, etc.) en hebben
 * verschillende eigenschappen zoals prioriteit en visuele representatie.
 *
 * @invariant properlyInitialized() == true
 * @invariant !getBaanNaam().empty()
 * @invariant getPositie() >= 0.0
 * @invariant getSnelheid() >= 0.0
 */
class Voertuig {
public:
    /**
     * @brief Constructor voor voertuig met minimale parameters
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan (in meters)
     *
     * @pre !baan.empty()
     * @pre positie >= 0.0
     * @post properlyInitialized() == true
     * @post getBaanNaam() == baan
     * @post getPositie() == positie
     * @post getSnelheid() == 0.0
     * @post getVersnelling() == 0.0
     * @post !isWaitingAtStop()
     */
    Voertuig(const std::string& baan, double positie);

    /**
     * @brief Constructor voor voertuig met alle fysische parameters
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan (in meters)
     * @param snelheid De initiële snelheid van het voertuig (in m/s)
     * @param versnelling De initiële versnelling van het voertuig (in m/s²)
     *
     * @pre !baan.empty()
     * @pre positie >= 0.0
     * @pre snelheid >= 0.0
     * @post properlyInitialized() == true
     * @post getBaanNaam() == baan
     * @post getPositie() == positie
     * @post getSnelheid() == snelheid
     * @post getVersnelling() == versnelling
     * @post !isWaitingAtStop()
     */
    Voertuig(const std::string& baan, double positie, double snelheid, double versnelling);

    /**
     * @brief Virtual destructor voor correcte afhandeling van afgeleide klassen
     *
     * Zorgt ervoor dat destructors van afgeleide klassen correct worden
     * aangeroepen bij polymorf gebruik.
     */
    virtual ~Voertuig();

    /**
     * @brief Copy constructor
     * @param other Het te kopiëren voertuig
     *
     * @pre other.properlyInitialized() == true
     * @post properlyInitialized() == true
     * @post getBaanNaam() == other.getBaanNaam()
     * @post getPositie() == other.getPositie()
     * @post getSnelheid() == other.getSnelheid()
     * @post getVersnelling() == other.getVersnelling()
     * @post isWaitingAtStop() == other.isWaitingAtStop()
     */
    Voertuig(const Voertuig& other);

    /**
     * @brief Assignment operator
     * @param other Het voertuig waarvan de waarden worden overgenomen
     * @return Referentie naar dit voertuig
     *
     * @pre other.properlyInitialized() == true
     * @post properlyInitialized() == true
     * @post getBaanNaam() == other.getBaanNaam()
     * @post getPositie() == other.getPositie()
     * @post getSnelheid() == other.getSnelheid()
     * @post getVersnelling() == other.getVersnelling()
     * @post isWaitingAtStop() == other.isWaitingAtStop()
     */
    Voertuig& operator=(const Voertuig& other);

    /**
     * @brief Controleert of het voertuig correct geïnitialiseerd is
     * @return true als het voertuig correct geïnitialiseerd is, false anders
     *
     * Deze methode wordt gebruikt voor Design by Contract validatie
     * om te verifiëren dat het object in een geldige staat verkeert.
     */
    bool properlyInitialized() const;

    /**
     * @brief Geeft de naam van de baan terug waar het voertuig zich bevindt
     * @return De naam van de baan
     *
     * @pre properlyInitialized() == true
     * @post !result.empty()
     */
    std::string getBaanNaam() const;

    /**
     * @brief Alias voor getBaanNaam voor compatibiliteit met bestaande code
     * @return De naam van de baan
     *
     * @pre properlyInitialized() == true
     * @post !result.empty()
     */
    std::string getBaan() const;

    /**
     * @brief Stelt de naam van de baan in waar het voertuig zich bevindt
     * @param nieuweNaam De nieuwe baannaam
     *
     * @pre properlyInitialized() == true
     * @pre !nieuweNaam.empty()
     * @post getBaanNaam() == nieuweNaam
     */
    void setBaanNaam(const std::string& nieuweNaam);

    /**
     * @brief Geeft de huidige positie van het voertuig op de baan terug
     * @return De positie op de baan in meters
     *
     * @pre properlyInitialized() == true
     * @post result >= 0.0
     */
    double getPositie() const;

    /**
     * @brief Stelt de positie van het voertuig in
     * @param nieuwePositie De nieuwe positie op de baan in meters
     *
     * @pre properlyInitialized() == true
     * @pre nieuwePositie >= 0.0
     * @post getPositie() == nieuwePositie
     */
    void setPositie(double nieuwePositie);

    /**
     * @brief Geeft de huidige snelheid van het voertuig terug
     * @return De snelheid in m/s
     *
     * @pre properlyInitialized() == true
     * @post result >= 0.0
     */
    double getSnelheid() const;

    /**
     * @brief Stelt de snelheid van het voertuig in
     * @param nieuweSnelheid De nieuwe snelheid in m/s
     *
     * @pre properlyInitialized() == true
     * @pre nieuweSnelheid >= 0.0
     * @post getSnelheid() == nieuweSnelheid
     */
    void setSnelheid(double nieuweSnelheid);

    /**
     * @brief Geeft de huidige versnelling van het voertuig terug
     * @return De versnelling in m/s²
     *
     * @pre properlyInitialized() == true
     */
    double getVersnelling() const;

    /**
     * @brief Stelt de versnelling van het voertuig in
     * @param nieuweVersnelling De nieuwe versnelling in m/s²
     *
     * @pre properlyInitialized() == true
     * @post getVersnelling() == nieuweVersnelling
     */
    void setVersnelling(double nieuweVersnelling);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return Het type van het voertuig als string (bijv. "auto", "bus", "brandweerwagen")
     *
     * @pre properlyInitialized() == true
     * @post !result.empty()
     *
     * Pure virtual methode die door afgeleide klassen moet worden geïmplementeerd
     * om het specifieke type voertuig aan te geven.
     */
    virtual std::string getType() const = 0;

    /**
     * @brief Controleert of het voertuig een prioriteitsvoertuig is
     * @return true als het voertuig prioriteit heeft (hulpdiensten), false anders
     *
     * @pre properlyInitialized() == true
     *
     * Prioriteitsvoertuigen zoals brandweerwagens, ambulances en politiewagens
     * hebben voorrang in het verkeer en kunnen verkeerslichten beïnvloeden.
     */
    virtual bool isPrioriteitsVoertuig() const = 0;

    /**
     * @brief Geeft de visuele representatie van het voertuig terug voor grafische output
     * @return Eén karakter dat het voertuig representeert (bijv. 'A' voor auto, 'B' voor bus)
     *
     * @pre properlyInitialized() == true
     * @post result.length() == 1
     *
     * Deze methode wordt gebruikt voor het genereren van grafische impressies
     * van de verkeerssituatie.
     */
    virtual std::string getVisueleRepresentatie() const = 0;

    /**
     * @brief Controleert of het voertuig momenteel wacht bij een halte
     * @return true als het voertuig wacht bij een bushalte of verkeerslicht, false anders
     *
     * @pre properlyInitialized() == true
     */
    bool isWaitingAtStop() const;

    /**
     * @brief Stelt in of het voertuig wacht bij een halte
     * @param waiting true als het voertuig moet wachten, false als het kan doorrijden
     *
     * @pre properlyInitialized() == true
     * @post isWaitingAtStop() == waiting
     */
    void setWaitingAtStop(bool waiting);

    /**
     * @brief Controleert of dit voertuig een bus is
     * @return true als het voertuig een bus is, false anders
     *
     * @pre properlyInitialized() == true
     */
    virtual bool isBus() const = 0;

    /**
     * @brief Maakt een kopie van dit voertuig
     * @return Unique pointer naar een kopie van dit voertuig
     *
     * @pre properlyInitialized() == true
     * @post result != nullptr
     * @post result->getType() == this->getType()
     */
    virtual std::unique_ptr<Voertuig> clone() const = 0;

    /**
     * @brief Update de positie en snelheid van het voertuig gedurende een tijdstap
     * @param deltaTime De tijdstap in seconden
     *
     * @pre properlyInitialized() == true
     * @pre deltaTime > 0.0
     * @post properlyInitialized() == true
     * @post getSnelheid() >= 0.0
     *
     * Deze methode simuleert de beweging van het voertuig volgens de wetten
     * van de fysica: nieuwe_positie = oude_positie + snelheid * tijd + 0.5 * versnelling * tijd²
     * nieuwe_snelheid = oude_snelheid + versnelling * tijd
     *
     * De snelheid wordt nooit negatief - voertuigen kunnen niet achteruit rijden.
     */
    virtual void update(double deltaTime);

    /**
     * @brief Factory methode voor het creëren van voertuigen van verschillende types
     * @param baan De naam van de baan waar het voertuig wordt geplaatst
     * @param positie De positie op de baan waar het voertuig wordt geplaatst
     * @param type Het type voertuig ("auto", "bus", "brandweerwagen", "ziekenwagen", "politiecombi")
     * @return Unique pointer naar het gecreëerde voertuig, of nullptr bij onbekend type
     *
     * @pre !baan.empty()
     * @pre positie >= 0.0
     * @pre !type.empty()
     * @post result == nullptr || result->properlyInitialized() == true
     * @post result == nullptr || result->getBaanNaam() == baan
     * @post result == nullptr || result->getPositie() == positie
     * @post result == nullptr || result->getType() == type
     *
     * Deze factory methode creëert voertuigen van het juiste type gebaseerd
     * op de type string. Ondersteunde types:
     * - "auto": Gewone personenauto
     * - "bus": Openbaar vervoer bus
     * - "brandweerwagen": Brandweer voertuig (prioriteit)
     * - "ziekenwagen": Ambulance (prioriteit)
     * - "politiecombi": Politie voertuig (prioriteit)
     */
    static std::unique_ptr<Voertuig> maakVoertuig(const std::string& baan, double positie, const std::string& type);

protected:
    std::string baanNaam;        ///< Naam van de baan waar het voertuig zich bevindt
    double positie;              ///< Positie op de baan in meters
    double snelheid;             ///< Huidige snelheid in m/s
    double versnelling;          ///< Huidige versnelling in m/s²
    bool waitingAtStop;          ///< Of het voertuig wacht bij een halte
    void* _initCheck;            ///< Pointer voor Design by Contract validatie

private:
    // Geen private members - alles is protected voor afgeleide klassen
};

/**
 * @brief Concrete implementatie van een gewone personenauto
 *
 * Een auto is een standaard voertuig zonder speciale prioriteit.
 * Het heeft normale verkeerseigenschappen en volgt alle verkeersregels.
 */
class Auto : public Voertuig {
public:
    /**
     * @brief Constructor voor een auto
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     */
    Auto(const std::string& baan, double positie);

    /**
     * @brief Constructor voor een auto met snelheid en versnelling
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param snelheid Initiële snelheid
     * @param versnelling Initiële versnelling
     */
    Auto(const std::string& baan, double positie, double snelheid, double versnelling);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return "auto"
     */
    std::string getType() const override;

    /**
     * @brief Controleert of dit een prioriteitsvoertuig is
     * @return false (auto's hebben geen prioriteit)
     */
    bool isPrioriteitsVoertuig() const override;

    /**
     * @brief Geeft de visuele representatie terug
     * @return "A" voor auto
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Controleert of dit voertuig een bus is
     * @return false (dit is een auto)
     */
    bool isBus() const override;

    /**
     * @brief Maakt een kopie van dit voertuig
     * @return Unique pointer naar een kopie van deze auto
     */
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Concrete implementatie van een bus voor openbaar vervoer
 *
 * Een bus is een groot voertuig dat bij bushaltes kan stoppen.
 * Het heeft geen prioriteit maar kan wel speciale rijstroken gebruiken.
 */
class Bus : public Voertuig {
public:
    /**
     * @brief Constructor voor een bus
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     */
    Bus(const std::string& baan, double positie);

    /**
     * @brief Constructor voor een bus met snelheid en versnelling
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param snelheid Initiële snelheid
     * @param versnelling Initiële versnelling
     */
    Bus(const std::string& baan, double positie, double snelheid, double versnelling);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return "bus"
     */
    std::string getType() const override;

    /**
     * @brief Controleert of dit een prioriteitsvoertuig is
     * @return false (bussen hebben geen prioriteit)
     */
    bool isPrioriteitsVoertuig() const override;

    /**
     * @brief Geeft de visuele representatie terug
     * @return "B" voor bus
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Controleert of dit voertuig een bus is
     * @return true (dit is een bus)
     */
    bool isBus() const override;

    /**
     * @brief Maakt een kopie van dit voertuig
     * @return Unique pointer naar een kopie van deze bus
     */
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Concrete implementatie van een brandweerwagen
 *
 * Een brandweerwagen is een prioriteitsvoertuig dat voorrang heeft
 * in het verkeer en verkeerslichten kan beïnvloeden.
 */
class Brandweerwagen : public Voertuig {
public:
    /**
     * @brief Constructor voor een brandweerwagen
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     */
    Brandweerwagen(const std::string& baan, double positie);

    /**
     * @brief Constructor voor een brandweerwagen met snelheid en versnelling
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param snelheid Initiële snelheid
     * @param versnelling Initiële versnelling
     */
    Brandweerwagen(const std::string& baan, double positie, double snelheid, double versnelling);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return "brandweerwagen"
     */
    std::string getType() const override;

    /**
     * @brief Controleert of dit een prioriteitsvoertuig is
     * @return true (brandweerwagens hebben prioriteit)
     */
    bool isPrioriteitsVoertuig() const override;

    /**
     * @brief Geeft de visuele representatie terug
     * @return "F" voor fire truck (brandweerwagen)
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Controleert of dit voertuig een bus is
     * @return false (dit is een brandweerwagen)
     */
    bool isBus() const override;

    /**
     * @brief Maakt een kopie van dit voertuig
     * @return Unique pointer naar een kopie van deze brandweerwagen
     */
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Concrete implementatie van een ziekenwagen (ambulance)
 *
 * Een ziekenwagen is een prioriteitsvoertuig dat voorrang heeft
 * in het verkeer en verkeerslichten kan beïnvloeden.
 */
class Ziekenwagen : public Voertuig {
public:
    /**
     * @brief Constructor voor een ziekenwagen
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     */
    Ziekenwagen(const std::string& baan, double positie);

    /**
     * @brief Constructor voor een ziekenwagen met snelheid en versnelling
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param snelheid Initiële snelheid
     * @param versnelling Initiële versnelling
     */
    Ziekenwagen(const std::string& baan, double positie, double snelheid, double versnelling);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return "ziekenwagen"
     */
    std::string getType() const override;

    /**
     * @brief Controleert of dit een prioriteitsvoertuig is
     * @return true (ziekenwagens hebben prioriteit)
     */
    bool isPrioriteitsVoertuig() const override;

    /**
     * @brief Geeft de visuele representatie terug
     * @return "Z" voor ziekenwagen
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Controleert of dit voertuig een bus is
     * @return false (dit is een ziekenwagen)
     */
    bool isBus() const override;

    /**
     * @brief Maakt een kopie van dit voertuig
     * @return Unique pointer naar een kopie van deze ziekenwagen
     */
    std::unique_ptr<Voertuig> clone() const override;
};

/**
 * @brief Concrete implementatie van een politiecombi
 *
 * Een politiecombi is een prioriteitsvoertuig dat voorrang heeft
 * in het verkeer en verkeerslichten kan beïnvloeden.
 */
class Politiecombi : public Voertuig {
public:
    /**
     * @brief Constructor voor een politiecombi
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     */
    Politiecombi(const std::string& baan, double positie);

    /**
     * @brief Constructor voor een politiecombi met snelheid en versnelling
     * @param baan De naam van de baan
     * @param positie De positie op de baan
     * @param snelheid Initiële snelheid
     * @param versnelling Initiële versnelling
     */
    Politiecombi(const std::string& baan, double positie, double snelheid, double versnelling);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return "politiecombi"
     */
    std::string getType() const override;

    /**
     * @brief Controleert of dit een prioriteitsvoertuig is
     * @return true (politiecombi's hebben prioriteit)
     */
    bool isPrioriteitsVoertuig() const override;

    /**
     * @brief Geeft de visuele representatie terug
     * @return "P" voor politie
     */
    std::string getVisueleRepresentatie() const override;

    /**
     * @brief Controleert of dit voertuig een bus is
     * @return false (dit is een politiecombi)
     */
    bool isBus() const override;

    /**
     * @brief Maakt een kopie van dit voertuig
     * @return Unique pointer naar een kopie van deze politiecombi
     */
    std::unique_ptr<Voertuig> clone() const override;
};

#endif // VOERTUIG_H