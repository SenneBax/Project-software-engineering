/**
 * @file voertuig.cpp
 * @brief Implementatie van de Voertuig klasse (Herzien met polymorfisme)
 */

#include "voertuig.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "DesignByContract.h"

// =============================================================================
// Basisklasse Voertuig implementatie
// =============================================================================

/**
 * @brief Constructor voor Voertuig met standaard snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 *
 * @pre REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
 * @pre REQUIRE(positie > 0.0, "Positie moet positief zijn.");
 * @post ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 */
Voertuig::Voertuig(const std::string& baan, double positie)
    : baanNaam(baan), positie(positie), snelheid(0.0), versnelling(0.0) {
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    //REQUIRE(positie > 0.0, "Positie moet positief zijn.");

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

/**
 * @brief Constructor voor Voertuig met snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 * @param snelheid Snelheid van het voertuig
 * @param versnelling Versnelling van het voertuig
 *
 * @pre REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
 * @pre REQUIRE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");
 * @post ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 */
Voertuig::Voertuig(const std::string& baan, double positie, double snelheid, double versnelling)
    : baanNaam(baan), positie(positie), snelheid(snelheid), versnelling(versnelling) {
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    REQUIRE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

/**
 * @brief Destructor voor Voertuig
 */
Voertuig::~Voertuig() {
    // Geen dynamisch geheugen om vrij te geven
}

/**
 * @brief Copy constructor voor Voertuig
 * @param other Te kopiëren voertuig
 *
 * @post ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 */
Voertuig::Voertuig(const Voertuig& other)
    : baanNaam(other.baanNaam), positie(other.positie), snelheid(other.snelheid),
      versnelling(other.versnelling) {
    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

/**
 * @brief Assignment operator voor Voertuig
 * @param other Voertuig om toe te wijzen
 * @return Referentie naar dit voertuig
 */
Voertuig& Voertuig::operator=(const Voertuig& other) {
    if (this != &other) {
        baanNaam = other.baanNaam;
        positie = other.positie;
        snelheid = other.snelheid;
        versnelling = other.versnelling;
    }
    return *this;
}

/**
 * @brief Getter voor de baannaam van het voertuig
 * @return De naam van de baan
 *
 * @pre REQUIRE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 * @post ENSURE(!baan.empty(), "Baanaam mag niet leeg zijn.");
 */
std::string Voertuig::getBaanNaam() const {
    REQUIRE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
    return baanNaam;
}

/**
 * @brief Alias voor getBaanNaam
 * @return De naam van de baan
 */
std::string Voertuig::getBaan() const {
    return baanNaam;
}

/**
 * @brief Setter voor de baannaam van het voertuig
 * @param nieuweNaam De nieuwe baannaam
 *
 * @pre REQUIRE(properlyInitialized(), "setBaanNaam moet eindigen in een geldige toestand.");
 * @pre REQUIRE(!nieuweNaam.empty(), "nieuweNaam mag niet leeg zijn.");
 * @post ENSURE(baanNaam == nieuweNaam, "Baannaam werd niet correct ingesteld");
 */
void Voertuig::setBaanNaam(const std::string& nieuweNaam) {
    REQUIRE(properlyInitialized(), "setBaanNaam moet eindigen in een geldige toestand.");
    REQUIRE(!nieuweNaam.empty(), "nieuweNaam mag niet leeg zijn.");
    baanNaam = nieuweNaam;
    ENSURE(baanNaam == nieuweNaam, "Baannaam werd niet correct ingesteld");
}

/**
 * @brief Alias voor setBaanNaam
 * @param baan De nieuwe baannaam
 */
void Voertuig::setBaan(const std::string& baan) {
    setBaanNaam(baan);
}

/**
 * @brief Getter voor de positie van het voertuig
 * @return De positie op de baan
 *
 * @pre REQUIRE(properlyInitialized(), "getPositie moet eindigen in een geldige toestand.");
 */
double Voertuig::getPositie() const {
    REQUIRE(properlyInitialized(), "getPositie moet eindigen in een geldige toestand.");
    return positie;
}

/**
 * @brief Setter voor de positie van het voertuig
 * @param nieuwePositie De nieuwe positie
 *
 * @pre REQUIRE(properlyInitialized(), "setPositie moet eindigen in een geldige toestand.");
 * @pre REQUIRE(nieuwePositie >= 0.0, "nieuwePositie mag niet negatief zijn.");
 * @post ENSURE(positie == nieuwePositie, "Positie werd niet correct ingesteld.");
 */
void Voertuig::setPositie(double nieuwePositie) {
    REQUIRE(properlyInitialized(), "setPositie moet eindigen in een geldige toestand.");
    REQUIRE(nieuwePositie >= 0.0, "nieuwePositie mag niet negatief zijn.");
    positie = nieuwePositie;
    ENSURE(positie == nieuwePositie, "Positie werd niet correct ingesteld.");
}

/**
 * @brief Getter voor de snelheid van het voertuig
 * @return De snelheid in m/s
 *
 * @pre REQUIRE(properlyInitialized(), "getSnelheid moet eindigen in een geldige toestand.");
 */
double Voertuig::getSnelheid() const {
    REQUIRE(properlyInitialized(), "getSnelheid moet eindigen in een geldige toestand.");
    return snelheid;
}

/**
 * @brief Setter voor de snelheid van het voertuig
 * @param nieuweSnelheid De nieuwe snelheid
 *
 * @pre REQUIRE(properlyInitialized(), "setSnelheid moet eindigen in een geldige toestand.");
 * @pre REQUIRE(nieuweSnelheid >= 0.0, "nieuweSnelheid mag niet negatief zijn.");
 * @post ENSURE(snelheid == nieuweSnelheid, "Snelheid werd niet correct ingesteld.");
 */
void Voertuig::setSnelheid(double nieuweSnelheid) {
    REQUIRE(properlyInitialized(), "setSnelheid moet eindigen in een geldige toestand.");
    REQUIRE(nieuweSnelheid >= 0.0, "nieuweSnelheid mag niet negatief zijn.");
    snelheid = nieuweSnelheid;
    ENSURE(snelheid == nieuweSnelheid, "Snelheid werd niet correct ingesteld.");
}

/**
 * @brief Getter voor de versnelling van het voertuig
 * @return De versnelling in m/s²
 *
 * @pre REQUIRE(properlyInitialized(), "getVersnelling moet eindigen in een geldige toestand.");
 */
double Voertuig::getVersnelling() const {
    REQUIRE(properlyInitialized(), "getVersnelling moet eindigen in een geldige toestand.");
    return versnelling;
}

/**
 * @brief Setter voor de versnelling van het voertuig
 * @param nieuweVersnelling De nieuwe versnelling
 *
 * @pre REQUIRE(properlyInitialized(), "setVersnelling moet eindigen in een geldige toestand.");
 * @post ENSURE(versnelling == nieuweVersnelling, "Versnelling werd niet correct ingesteld.");
 */
void Voertuig::setVersnelling(double nieuweVersnelling) {
    REQUIRE(properlyInitialized(), "setVersnelling moet eindigen in een geldige toestand.");
    versnelling = nieuweVersnelling;
    ENSURE(versnelling == nieuweVersnelling, "Versnelling werd niet correct ingesteld.");
}

/**
 * @brief Berekent de versnelling van het voertuig
 * @param voorliggendVoertuig Pointer naar het voorliggende voertuig
 * @param isEersteVoertuig Of dit het eerste voertuig op de baan is
 * @param verkeersLichtVertraagFactor Factor voor verkeerslicht vertraging
 * @param bushalteVertraagFactor Factor voor bushalte vertraging
 *
 * @pre REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
 */
void Voertuig::berekenVersnelling(Voertuig* voorliggendVoertuig, bool isEersteVoertuig,
                                 double verkeersLichtVertraagFactor, double bushalteVertraagFactor) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    // Basis implementatie - kan overridden worden door afgeleide klassen
    if (getIsWaitingAtStop()) {
        versnelling = 0.0;
        return;
    }

    // Eenvoudige versnellingsberekening
    if (voorliggendVoertuig != nullptr) {
        double afstand = voorliggendVoertuig->getPositie() - positie - getLengte();
        if (afstand < getMinVolgafstand()) {
            // Remmen
            versnelling = -getMaxRemFactor() * 0.5;
        } else if (snelheid < getMaxSnelheid()) {
            // Versnellen
            versnelling = getMaxVersnelling() * 0.5;
        } else {
            versnelling = 0.0;
        }
    } else if (snelheid < getMaxSnelheid()) {
        // Geen voorliggend voertuig, versnellen tot max snelheid
        versnelling = getMaxVersnelling();
    } else {
        versnelling = 0.0;
    }

    // Verkeerslicht en bushalte factoren toepassen
    if (verkeersLichtVertraagFactor >= 0.0) {
        versnelling *= verkeersLichtVertraagFactor;
    }
    if (bushalteVertraagFactor >= 0.0) {
        versnelling *= bushalteVertraagFactor;
    }
}

/**
 * @brief Update de positie en snelheid van het voertuig
 * @param tijdstap De tijdstap voor de update
 *
 * @pre REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
 * @pre REQUIRE(tijdstap > 0, "Tijdstap moet positief zijn.");
 */
void Voertuig::updatePositieEnSnelheid(double tijdstap) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(tijdstap > 0, "Tijdstap moet positief zijn.");

    // Nieuwe snelheid berekenen
    double nieuweSnelheid = snelheid + versnelling * tijdstap;

    // Zorg dat snelheid niet negatief wordt
    if (nieuweSnelheid < 0) {
        nieuweSnelheid = 0;
    }

    // Zorg dat snelheid niet boven maximum komt
    if (nieuweSnelheid > getMaxSnelheid()) {
        nieuweSnelheid = getMaxSnelheid();
    }

    // Nieuwe positie berekenen (gemiddelde snelheid over tijdstap)
    double gemiddeldeSnelheid = (snelheid + nieuweSnelheid) / 2.0;
    positie += gemiddeldeSnelheid * tijdstap;

    // Snelheid updaten
    snelheid = nieuweSnelheid;
}

/**
 * @brief Zet de wachtstatus bij een bushalte
 * @param waiting Of het voertuig wacht bij een bushalte
 *
 * @pre REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
 */
void Voertuig::setIsWaitingAtStop(bool waiting) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    // Deze implementatie moet leeg blijven omdat de methode virtueel kan zijn
    // Afgeleide klassen kunnen hun eigen implementatie geven
}

/**
 * @brief Geeft de wachtstatus bij een bushalte terug
 * @return True als het voertuig wacht bij een bushalte
 *
 * @pre REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
 */
bool Voertuig::getIsWaitingAtStop() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    // Default implementatie: geen voertuig wacht standaard
    return false;
}

/**
 * @brief Alias voor getIsWaitingAtStop
 * @return True als het voertuig wacht bij een bushalte
 *
 * @pre REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
 */
bool Voertuig::isWaitingAtStop() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    return getIsWaitingAtStop();
}

/**
 * @brief Voert een noodstop uit
 *
 * @pre REQUIRE(properlyInitialized(), "noodStop moet starten in een geldige toestand.");
 * @post ENSURE(versnelling <= 0, "Noodstop moet negatieve versnelling geven.");
 */
void Voertuig::noodStop() {
    REQUIRE(properlyInitialized(), "noodStop moet starten in een geldige toestand.");
    // Formule uit B.5
    versnelling = -getMaxRemFactor() * snelheid / getMaxSnelheid();
    ENSURE(versnelling <= 0, "Noodstop moet negatieve versnelling geven.");
}

/**
 * @brief Controleert of het object correct geïnitialiseerd is
 * @return True als het object correct geïnitialiseerd is
 */
bool Voertuig::properlyInitialized() const {
    return _initCheck == this;
}

// =============================================================================
// Factory methods
// =============================================================================

/**
 * @brief Factory methode voor het maken van voertuigen
 * @param baan De naam van de baan
 * @param positie De positie op de baan
 * @param type Het type voertuig
 * @return Een unique_ptr naar het nieuwe voertuig
 */
std::unique_ptr<Voertuig> Voertuig::maakVoertuig(const std::string& baan, double positie,
                                                 const std::string& type) {
    if (type == "auto") {
        return std::make_unique<Auto>(baan, positie);
    } else if (type == "bus") {
        return std::make_unique<Bus>(baan, positie);
    } else if (type == "brandweerwagen") {
        return std::make_unique<Brandweerwagen>(baan, positie);
    } else if (type == "ziekenwagen") {
        return std::make_unique<Ziekenwagen>(baan, positie);
    } else if (type == "politiecombi") {
        return std::make_unique<Politiecombi>(baan, positie);
    } else {
        // Default naar auto als type niet herkend wordt
        return std::make_unique<Auto>(baan, positie);
    }
}

/**
 * @brief Factory methode voor het maken van voertuigen met snelheid en versnelling
 * @param baan De naam van de baan
 * @param positie De positie op de baan
 * @param snelheid De snelheid van het voertuig
 * @param versnelling De versnelling van het voertuig
 * @param type Het type voertuig
 * @return Een unique_ptr naar het nieuwe voertuig
 */
std::unique_ptr<Voertuig> Voertuig::maakVoertuig(const std::string& baan, double positie,
                                                 double snelheid, double versnelling,
                                                 const std::string& type) {
    if (type == "auto") {
        return std::make_unique<Auto>(baan, positie, snelheid, versnelling);
    } else if (type == "bus") {
        return std::make_unique<Bus>(baan, positie, snelheid, versnelling);
    } else if (type == "brandweerwagen") {
        return std::make_unique<Brandweerwagen>(baan, positie, snelheid, versnelling);
    } else if (type == "ziekenwagen") {
        return std::make_unique<Ziekenwagen>(baan, positie, snelheid, versnelling);
    } else if (type == "politiecombi") {
        return std::make_unique<Politiecombi>(baan, positie, snelheid, versnelling);
    } else {
        // Default naar auto als type niet herkend wordt
        return std::make_unique<Auto>(baan, positie, snelheid, versnelling);
    }
}

// =============================================================================
// Auto implementatie
// =============================================================================

/**
 * @brief Constructor voor Auto met standaard snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 *
 * @post ENSURE(getType() == "auto", "Auto type moet correct zijn.");
 * @post ENSURE(!isPrioriteitsvoertuig(), "Auto mag geen prioriteitsvoertuig zijn.");
 * @post ENSURE(!isBus(), "Auto mag geen bus zijn.");
 */
Auto::Auto(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "auto", "Auto type moet correct zijn.");
    ENSURE(!isPrioriteitsvoertuig(), "Auto mag geen prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Auto mag geen bus zijn.");
}

/**
 * @brief Constructor voor Auto met snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 * @param snelheid Snelheid van het voertuig
 * @param versnelling Versnelling van het voertuig
 *
 * @post ENSURE(getType() == "auto", "Auto type moet correct zijn.");
 * @post ENSURE(!isPrioriteitsvoertuig(), "Auto mag geen prioriteitsvoertuig zijn.");
 * @post ENSURE(!isBus(), "Auto mag geen bus zijn.");
 */
Auto::Auto(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "auto", "Auto type moet correct zijn.");
    ENSURE(!isPrioriteitsvoertuig(), "Auto mag geen prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Auto mag geen bus zijn.");
}

/**
 * @brief Implementatie van getType voor Auto
 * @return "auto" als string
 */
std::string Auto::getType() const {
    return "auto";
}

/**
 * @brief Implementatie van isPrioriteitsvoertuig voor Auto
 * @return false (auto's zijn geen prioriteitsvoertuigen)
 */
bool Auto::isPrioriteitsvoertuig() const {
    return false;
}

/**
 * @brief Implementatie van isBus voor Auto
 * @return false (auto's zijn geen bussen)
 */
bool Auto::isBus() const {
    return false;
}

/**
 * @brief Implementatie van getLengte voor Auto
 * @return De lengte van een auto in meters
 */
double Auto::getLengte() const {
    return 4.0;
}

/**
 * @brief Implementatie van getMaxSnelheid voor Auto
 * @return De maximumsnelheid van een auto in m/s
 */
double Auto::getMaxSnelheid() const {
    return 16.6;
}

/**
 * @brief Implementatie van getMaxVersnelling voor Auto
 * @return De maximumversnelling van een auto in m/s²
 */
double Auto::getMaxVersnelling() const {
    return 1.44;
}

/**
 * @brief Implementatie van getMaxRemFactor voor Auto
 * @return De maximale remfactor van een auto in m/s²
 */
double Auto::getMaxRemFactor() const {
    return 4.61;
}

/**
 * @brief Implementatie van getMinVolgafstand voor Auto
 * @return De minimale volgafstand van een auto in meters
 */
double Auto::getMinVolgafstand() const {
    return 4.0;
}

/**
 * @brief Implementatie van getVisueleRepresentatie voor Auto
 * @return "A" als visuele representatie
 */
std::string Auto::getVisueleRepresentatie() const {
    return "A";
}

/**
 * @brief Clone method implementatie voor Auto
 * @return Een unieke pointer naar een nieuwe Auto met dezelfde eigenschappen
 */
std::unique_ptr<Voertuig> Auto::clone() const {
    return std::make_unique<Auto>(*this);
}

// =============================================================================
// Bus implementatie
// =============================================================================

/**
 * @brief Constructor voor Bus met standaard snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 *
 * @post ENSURE(getType() == "bus", "Bus type moet correct zijn.");
 * @post ENSURE(!isPrioriteitsvoertuig(), "Bus mag geen prioriteitsvoertuig zijn.");
 * @post ENSURE(isBus(), "Bus moet een bus zijn.");
 */
Bus::Bus(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "bus", "Bus type moet correct zijn.");
    ENSURE(!isPrioriteitsvoertuig(), "Bus mag geen prioriteitsvoertuig zijn.");
    ENSURE(isBus(), "Bus moet een bus zijn.");
}

/**
 * @brief Constructor voor Bus met snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 * @param snelheid Snelheid van het voertuig
 * @param versnelling Versnelling van het voertuig
 *
 * @post ENSURE(getType() == "bus", "Bus type moet correct zijn.");
 * @post ENSURE(!isPrioriteitsvoertuig(), "Bus mag geen prioriteitsvoertuig zijn.");
 * @post ENSURE(isBus(), "Bus moet een bus zijn.");
 */
Bus::Bus(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "bus", "Bus type moet correct zijn.");
    ENSURE(!isPrioriteitsvoertuig(), "Bus mag geen prioriteitsvoertuig zijn.");
    ENSURE(isBus(), "Bus moet een bus zijn.");
}

/**
 * @brief Implementatie van getType voor Bus
 * @return "bus" als string
 */
std::string Bus::getType() const {
    return "bus";
}

/**
 * @brief Implementatie van isPrioriteitsvoertuig voor Bus
 * @return false (bussen zijn geen prioriteitsvoertuigen)
 */
bool Bus::isPrioriteitsvoertuig() const {
    return false;
}

/**
 * @brief Implementatie van isBus voor Bus
 * @return true (bussen zijn bussen)
 */
bool Bus::isBus() const {
    return true;
}

/**
 * @brief Implementatie van getLengte voor Bus
 * @return De lengte van een bus in meters
 */
double Bus::getLengte() const {
    return 12.0;
}

/**
 * @brief Implementatie van getMaxSnelheid voor Bus
 * @return De maximumsnelheid van een bus in m/s
 */
double Bus::getMaxSnelheid() const {
    return 11.1;
}

/**
 * @brief Implementatie van getMaxVersnelling voor Bus
 * @return De maximumversnelling van een bus in m/s²
 */
double Bus::getMaxVersnelling() const {
    return 1.22;
}

/**
 * @brief Implementatie van getMaxRemFactor voor Bus
 * @return De maximale remfactor van een bus in m/s²
 */
double Bus::getMaxRemFactor() const {
    return 4.29;
}

/**
 * @brief Implementatie van getMinVolgafstand voor Bus
 * @return De minimale volgafstand van een bus in meters
 */
double Bus::getMinVolgafstand() const {
    return 12.0;
}

/**
 * @brief Implementatie van getVisueleRepresentatie voor Bus
 * @return "B" als visuele representatie
 */
std::string Bus::getVisueleRepresentatie() const {
    return "B";
}

/**
 * @brief Clone method implementatie voor Bus
 * @return Een unieke pointer naar een nieuwe Bus met dezelfde eigenschappen
 */
std::unique_ptr<Voertuig> Bus::clone() const {
    return std::make_unique<Bus>(*this);
}

// =============================================================================
// Brandweerwagen implementatie
// =============================================================================

/**
 * @brief Constructor voor Brandweerwagen met standaard snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 *
 * @post ENSURE(getType() == "brandweerwagen", "Brandweerwagen type moet correct zijn.");
 * @post ENSURE(isPrioriteitsvoertuig(), "Brandweerwagen moet een prioriteitsvoertuig zijn.");
 * @post ENSURE(!isBus(), "Brandweerwagen mag geen bus zijn.");
 */
Brandweerwagen::Brandweerwagen(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "brandweerwagen", "Brandweerwagen type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Brandweerwagen moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Brandweerwagen mag geen bus zijn.");
}

/**
 * @brief Constructor voor Brandweerwagen met snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 * @param snelheid Snelheid van het voertuig
 * @param versnelling Versnelling van het voertuig
 *
 * @post ENSURE(getType() == "brandweerwagen", "Brandweerwagen type moet correct zijn.");
 * @post ENSURE(isPrioriteitsvoertuig(), "Brandweerwagen moet een prioriteitsvoertuig zijn.");
 * @post ENSURE(!isBus(), "Brandweerwagen mag geen bus zijn.");
 */
Brandweerwagen::Brandweerwagen(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "brandweerwagen", "Brandweerwagen type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Brandweerwagen moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Brandweerwagen mag geen bus zijn.");
}

/**
 * @brief Implementatie van getType voor Brandweerwagen
 * @return "brandweerwagen" als string
 */
std::string Brandweerwagen::getType() const {
    return "brandweerwagen";
}

/**
 * @brief Implementatie van isPrioriteitsvoertuig voor Brandweerwagen
 * @return true (brandweerwagens zijn prioriteitsvoertuigen)
 */
bool Brandweerwagen::isPrioriteitsvoertuig() const {
    return true;
}

/**
 * @brief Implementatie van isBus voor Brandweerwagen
 * @return false (brandweerwagens zijn geen bussen)
 */
bool Brandweerwagen::isBus() const {
    return false;
}

/**
 * @brief Implementatie van getLengte voor Brandweerwagen
 * @return De lengte van een brandweerwagen in meters
 */
double Brandweerwagen::getLengte() const {
    return 10.0;
}

/**
 * @brief Implementatie van getMaxSnelheid voor Brandweerwagen
 * @return De maximumsnelheid van een brandweerwagen in m/s
 */
double Brandweerwagen::getMaxSnelheid() const {
    return 14.6;
}

/**
 * @brief Implementatie van getMaxVersnelling voor Brandweerwagen
 * @return De maximumversnelling van een brandweerwagen in m/s²
 */
double Brandweerwagen::getMaxVersnelling() const {
    return 1.33;
}

/**
 * @brief Implementatie van getMaxRemFactor voor Brandweerwagen
 * @return De maximale remfactor van een brandweerwagen in m/s²
 */
double Brandweerwagen::getMaxRemFactor() const {
    return 4.56;
}

/**
 * @brief Implementatie van getMinVolgafstand voor Brandweerwagen
 * @return De minimale volgafstand van een brandweerwagen in meters
 */
double Brandweerwagen::getMinVolgafstand() const {
    return 10.0;
}

/**
 * @brief Implementatie van getVisueleRepresentatie voor Brandweerwagen
 * @return "F" als visuele representatie
 */
std::string Brandweerwagen::getVisueleRepresentatie() const {
    return "F";
}

/**
 * @brief Clone method implementatie voor Brandweerwagen
 * @return Een unieke pointer naar een nieuwe Brandweerwagen met dezelfde eigenschappen
 */
std::unique_ptr<Voertuig> Brandweerwagen::clone() const {
    return std::make_unique<Brandweerwagen>(*this);
}

// =============================================================================
// Ziekenwagen implementatie
// =============================================================================

/**
 * @brief Constructor voor Ziekenwagen met standaard snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 *
 * @post ENSURE(getType() == "ziekenwagen", "Ziekenwagen type moet correct zijn.");
 * @post ENSURE(isPrioriteitsvoertuig(), "Ziekenwagen moet een prioriteitsvoertuig zijn.");
 * @post ENSURE(!isBus(), "Ziekenwagen mag geen bus zijn.");
 */
Ziekenwagen::Ziekenwagen(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "ziekenwagen", "Ziekenwagen type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Ziekenwagen moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Ziekenwagen mag geen bus zijn.");
}

/**
 * @brief Constructor voor Ziekenwagen met snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 * @param snelheid Snelheid van het voertuig
 * @param versnelling Versnelling van het voertuig
 *
 * @post ENSURE(getType() == "ziekenwagen", "Ziekenwagen type moet correct zijn.");
 * @post ENSURE(isPrioriteitsvoertuig(), "Ziekenwagen moet een prioriteitsvoertuig zijn.");
 * @post ENSURE(!isBus(), "Ziekenwagen mag geen bus zijn.");
 */
Ziekenwagen::Ziekenwagen(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "ziekenwagen", "Ziekenwagen type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Ziekenwagen moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Ziekenwagen mag geen bus zijn.");
}

/**
 * @brief Implementatie van getType voor Ziekenwagen
 * @return "ziekenwagen" als string
 */
std::string Ziekenwagen::getType() const {
    return "ziekenwagen";
}

/**
 * @brief Implementatie van isPrioriteitsvoertuig voor Ziekenwagen
 * @return true (ziekenwagens zijn prioriteitsvoertuigen)
 */
bool Ziekenwagen::isPrioriteitsvoertuig() const {
    return true;
}

/**
 * @brief Implementatie van isBus voor Ziekenwagen
 * @return false (ziekenwagens zijn geen bussen)
 */
bool Ziekenwagen::isBus() const {
    return false;
}

/**
 * @brief Implementatie van getLengte voor Ziekenwagen
 * @return De lengte van een ziekenwagen in meters
 */
double Ziekenwagen::getLengte() const {
    return 8.0;
}

/**
 * @brief Implementatie van getMaxSnelheid voor Ziekenwagen
 * @return De maximumsnelheid van een ziekenwagen in m/s
 */
double Ziekenwagen::getMaxSnelheid() const {
    return 15.5;
}

/**
 * @brief Implementatie van getMaxVersnelling voor Ziekenwagen
 * @return De maximumversnelling van een ziekenwagen in m/s²
 */
double Ziekenwagen::getMaxVersnelling() const {
    return 1.44;
}

/**
 * @brief Implementatie van getMaxRemFactor voor Ziekenwagen
 * @return De maximale remfactor van een ziekenwagen in m/s²
 */
double Ziekenwagen::getMaxRemFactor() const {
    return 4.47;
}

/**
 * @brief Implementatie van getMinVolgafstand voor Ziekenwagen
 * @return De minimale volgafstand van een ziekenwagen in meters
 */
double Ziekenwagen::getMinVolgafstand() const {
    return 8.0;
}

/**
 * @brief Implementatie van getVisueleRepresentatie voor Ziekenwagen
 * @return "Z" als visuele representatie
 */
std::string Ziekenwagen::getVisueleRepresentatie() const {
    return "Z";
}

/**
 * @brief Clone method implementatie voor Ziekenwagen
 * @return Een unieke pointer naar een nieuwe Ziekenwagen met dezelfde eigenschappen
 */
std::unique_ptr<Voertuig> Ziekenwagen::clone() const {
    return std::make_unique<Ziekenwagen>(*this);
}

// =============================================================================
// Politiecombi implementatie
// =============================================================================

/**
 * @brief Constructor voor Politiecombi met standaard snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 *
 * @post ENSURE(getType() == "politiecombi", "Politiecombi type moet correct zijn.");
 * @post ENSURE(isPrioriteitsvoertuig(), "Politiecombi moet een prioriteitsvoertuig zijn.");
 * @post ENSURE(!isBus(), "Politiecombi mag geen bus zijn.");
 */
Politiecombi::Politiecombi(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "politiecombi", "Politiecombi type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Politiecombi moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Politiecombi mag geen bus zijn.");
}

/**
 * @brief Constructor voor Politiecombi met snelheid en versnelling
 * @param baan Naam van de baan
 * @param positie Positie op de baan
 * @param snelheid Snelheid van het voertuig
 * @param versnelling Versnelling van het voertuig
 *
 * @post ENSURE(getType() == "politiecombi", "Politiecombi type moet correct zijn.");
 * @post ENSURE(isPrioriteitsvoertuig(), "Politiecombi moet een prioriteitsvoertuig zijn.");
 * @post ENSURE(!isBus(), "Politiecombi mag geen bus zijn.");
 */
Politiecombi::Politiecombi(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "politiecombi", "Politiecombi type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Politiecombi moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Politiecombi mag geen bus zijn.");
}

/**
 * @brief Implementatie van getType voor Politiecombi
 * @return "politiecombi" als string
 */
std::string Politiecombi::getType() const {
    return "politiecombi";
}

/**
 * @brief Implementatie van isPrioriteitsvoertuig voor Politiecombi
 * @return true (politiecombis zijn prioriteitsvoertuigen)
 */
bool Politiecombi::isPrioriteitsvoertuig() const {
    return true;
}

/**
 * @brief Implementatie van isBus voor Politiecombi
 * @return false (politiecombis zijn geen bussen)
 */
bool Politiecombi::isBus() const {
    return false;
}

/**
 * @brief Implementatie van getLengte voor Politiecombi
 * @return De lengte van een politiecombi in meters
 */
double Politiecombi::getLengte() const {
    return 6.0;
}

/**
 * @brief Implementatie van getMaxSnelheid voor Politiecombi
 * @return De maximumsnelheid van een politiecombi in m/s
 */
double Politiecombi::getMaxSnelheid() const {
    return 17.2;
}

/**
 * @brief Implementatie van getMaxVersnelling voor Politiecombi
 * @return De maximumversnelling van een politiecombi in m/s²
 */
double Politiecombi::getMaxVersnelling() const {
    return 1.55;
}

/**
 * @brief Implementatie van getMaxRemFactor voor Politiecombi
 * @return De maximale remfactor van een politiecombi in m/s²
 */
double Politiecombi::getMaxRemFactor() const {
    return 4.92;
}

/**
 * @brief Implementatie van getMinVolgafstand voor Politiecombi
 * @return De minimale volgafstand van een politiecombi in meters
 */
double Politiecombi::getMinVolgafstand() const {
    return 6.0;
}

/**
 * @brief Implementatie van getVisueleRepresentatie voor Politiecombi
 * @return "P" als visuele representatie
 */
std::string Politiecombi::getVisueleRepresentatie() const {
    return "P";
}

/**
 * @brief Clone method implementatie voor Politiecombi
 * @return Een unieke pointer naar een nieuwe Politiecombi met dezelfde eigenschappen
 */
std::unique_ptr<Voertuig> Politiecombi::clone() const {
    return std::make_unique<Politiecombi>(*this);
}