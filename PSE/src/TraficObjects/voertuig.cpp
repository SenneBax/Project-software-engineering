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

Voertuig::Voertuig(const std::string& baan, double positie)
    : baanNaam(baan), positie(positie), snelheid(0.0), versnelling(0.0), isWaitingAtStop(false) {
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    //REQUIRE(positie > 0.0, "Positie moet positief zijn.");

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

Voertuig::Voertuig(const std::string& baan, double positie, double snelheid, double versnelling)
    : baanNaam(baan), positie(positie), snelheid(snelheid), versnelling(versnelling), isWaitingAtStop(false) {
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    REQUIRE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

Voertuig::~Voertuig() {
    // Geen dynamisch geheugen om vrij te geven
}

Voertuig::Voertuig(const Voertuig& other)
    : baanNaam(other.baanNaam), positie(other.positie), snelheid(other.snelheid),
      versnelling(other.versnelling), isWaitingAtStop(other.isWaitingAtStop) {
    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

Voertuig& Voertuig::operator=(const Voertuig& other) {
    if (this != &other) {
        baanNaam = other.baanNaam;
        positie = other.positie;
        snelheid = other.snelheid;
        versnelling = other.versnelling;
        isWaitingAtStop = other.isWaitingAtStop;
    }
    return *this;
}

std::string Voertuig::getBaanNaam() const {
    REQUIRE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
    return baanNaam;
}

std::string Voertuig::getBaan() const {
    return baanNaam;
}

void Voertuig::setBaanNaam(const std::string& nieuweNaam) {
    REQUIRE(properlyInitialized(), "setBaanNaam moet eindigen in een geldige toestand.");
    REQUIRE(!nieuweNaam.empty(), "nieuweNaam mag niet leeg zijn.");
    baanNaam = nieuweNaam;
    ENSURE(baanNaam == nieuweNaam, "Baannaam werd niet correct ingesteld");
}

void Voertuig::setBaan(const std::string& baan) {
    setBaanNaam(baan);
}

double Voertuig::getPositie() const {
    REQUIRE(properlyInitialized(), "getPositie moet eindigen in een geldige toestand.");
    return positie;
}

void Voertuig::setPositie(double nieuwePositie) {
    REQUIRE(properlyInitialized(), "setPositie moet eindigen in een geldige toestand.");
    REQUIRE(nieuwePositie >= 0.0, "nieuwePositie mag niet negatief zijn.");
    positie = nieuwePositie;
    ENSURE(positie == nieuwePositie, "Positie werd niet correct ingesteld.");
}

double Voertuig::getSnelheid() const {
    REQUIRE(properlyInitialized(), "getSnelheid moet eindigen in een geldige toestand.");
    return snelheid;
}

void Voertuig::setSnelheid(double nieuweSnelheid) {
    REQUIRE(properlyInitialized(), "setSnelheid moet eindigen in een geldige toestand.");
    REQUIRE(nieuweSnelheid >= 0.0, "nieuweSnelheid mag niet negatief zijn.");
    snelheid = nieuweSnelheid;
    ENSURE(snelheid == nieuweSnelheid, "Snelheid werd niet correct ingesteld.");
}

double Voertuig::getVersnelling() const {
    REQUIRE(properlyInitialized(), "getVersnelling moet eindigen in een geldige toestand.");
    return versnelling;
}

void Voertuig::setVersnelling(double nieuweVersnelling) {
    REQUIRE(properlyInitialized(), "setVersnelling moet eindigen in een geldige toestand.");
    versnelling = nieuweVersnelling;
    ENSURE(versnelling == nieuweVersnelling, "Versnelling werd niet correct ingesteld.");
}

void Voertuig::berekenVersnelling(Voertuig* voorliggendVoertuig, bool isEersteVoertuig, 
                                 double verkeersLichtVertraagFactor, double bushalteVertraagFactor) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    
    // Basis implementatie - kan overridden worden door afgeleide klassen
    if (isWaitingAtStop) {
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

void Voertuig::setIsWaitingAtStop(bool waiting) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    isWaitingAtStop = waiting;
}

bool Voertuig::getIsWaitingAtStop() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    return isWaitingAtStop;
}

void Voertuig::noodStop() {
    REQUIRE(properlyInitialized(), "noodStop moet starten in een geldige toestand.");
    // Formule uit B.5
    versnelling = -getMaxRemFactor() * snelheid / getMaxSnelheid();
    ENSURE(versnelling <= 0, "Noodstop moet negatieve versnelling geven.");
}

bool Voertuig::properlyInitialized() const {
    return _initCheck == this;
}

// =============================================================================
// Factory methods
// =============================================================================

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

std::unique_ptr<Voertuig> Voertuig::maakVoertuig(const std::string& baan, double positie,
                                                 double snelheid, double versnelling,
                                                 const std::string& type) {
    if (type == "auto") {
        return std::make_unique<Auto>(baan, positie, snelheid, versnelling);
    }
}

// =============================================================================
// Auto implementatie
// =============================================================================

Auto::Auto(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "auto", "Auto type moet correct zijn.");
    ENSURE(!isPrioriteitsvoertuig(), "Auto mag geen prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Auto mag geen bus zijn.");
}

Auto::Auto(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "auto", "Auto type moet correct zijn.");
    ENSURE(!isPrioriteitsvoertuig(), "Auto mag geen prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Auto mag geen bus zijn.");
}

std::string Auto::getType() const {
    return "auto";
}

bool Auto::isPrioriteitsvoertuig() const {
    return false;
}

bool Auto::isBus() const {
    return false;
}

double Auto::getLengte() const {
    return 4.0;
}

double Auto::getMaxSnelheid() const {
    return 16.6;
}

double Auto::getMaxVersnelling() const {
    return 1.44;
}

double Auto::getMaxRemFactor() const {
    return 4.61;
}

double Auto::getMinVolgafstand() const {
    return 4.0;
}

std::string Auto::getVisueleRepresentatie() const {
    return "A";
}

std::unique_ptr<Voertuig> Auto::clone() const {
    return std::make_unique<Auto>(*this);
}

// =============================================================================
// Bus implementatie
// =============================================================================

Bus::Bus(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "bus", "Bus type moet correct zijn.");
    ENSURE(!isPrioriteitsvoertuig(), "Bus mag geen prioriteitsvoertuig zijn.");
    ENSURE(isBus(), "Bus moet een bus zijn.");
}

Bus::Bus(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "bus", "Bus type moet correct zijn.");
    ENSURE(!isPrioriteitsvoertuig(), "Bus mag geen prioriteitsvoertuig zijn.");
    ENSURE(isBus(), "Bus moet een bus zijn.");
}

std::string Bus::getType() const {
    return "bus";
}

bool Bus::isPrioriteitsvoertuig() const {
    return false;
}

bool Bus::isBus() const {
    return true;
}

double Bus::getLengte() const {
    return 12.0;
}

double Bus::getMaxSnelheid() const {
    return 11.1;
}

double Bus::getMaxVersnelling() const {
    return 1.22;
}

double Bus::getMaxRemFactor() const {
    return 4.29;
}

double Bus::getMinVolgafstand() const {
    return 12.0;
}

std::string Bus::getVisueleRepresentatie() const {
    return "B";
}

std::unique_ptr<Voertuig> Bus::clone() const {
    return std::make_unique<Bus>(*this);
}

// =============================================================================
// Brandweerwagen implementatie
// =============================================================================

Brandweerwagen::Brandweerwagen(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "brandweerwagen", "Brandweerwagen type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Brandweerwagen moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Brandweerwagen mag geen bus zijn.");
}

Brandweerwagen::Brandweerwagen(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "brandweerwagen", "Brandweerwagen type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Brandweerwagen moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Brandweerwagen mag geen bus zijn.");
}

std::string Brandweerwagen::getType() const {
    return "brandweerwagen";
}

bool Brandweerwagen::isPrioriteitsvoertuig() const {
    return true;
}

bool Brandweerwagen::isBus() const {
    return false;
}

double Brandweerwagen::getLengte() const {
    return 10.0;
}

double Brandweerwagen::getMaxSnelheid() const {
    return 14.6;
}

double Brandweerwagen::getMaxVersnelling() const {
    return 1.33;
}

double Brandweerwagen::getMaxRemFactor() const {
    return 4.56;
}

double Brandweerwagen::getMinVolgafstand() const {
    return 10.0;
}

std::string Brandweerwagen::getVisueleRepresentatie() const {
    return "F";
}

std::unique_ptr<Voertuig> Brandweerwagen::clone() const {
    return std::make_unique<Brandweerwagen>(*this);
}

// =============================================================================
// Ziekenwagen implementatie
// =============================================================================

Ziekenwagen::Ziekenwagen(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "ziekenwagen", "Ziekenwagen type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Ziekenwagen moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Ziekenwagen mag geen bus zijn.");
}

Ziekenwagen::Ziekenwagen(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "ziekenwagen", "Ziekenwagen type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Ziekenwagen moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Ziekenwagen mag geen bus zijn.");
}

std::string Ziekenwagen::getType() const {
    return "ziekenwagen";
}

bool Ziekenwagen::isPrioriteitsvoertuig() const {
    return true;
}

bool Ziekenwagen::isBus() const {
    return false;
}

double Ziekenwagen::getLengte() const {
    return 8.0;
}

double Ziekenwagen::getMaxSnelheid() const {
    return 15.5;
}

double Ziekenwagen::getMaxVersnelling() const {
    return 1.44;
}

double Ziekenwagen::getMaxRemFactor() const {
    return 4.47;
}

double Ziekenwagen::getMinVolgafstand() const {
    return 8.0;
}

std::string Ziekenwagen::getVisueleRepresentatie() const {
    return "Z";
}

std::unique_ptr<Voertuig> Ziekenwagen::clone() const {
    return std::make_unique<Ziekenwagen>(*this);
}

// =============================================================================
// Politiecombi implementatie
// =============================================================================

Politiecombi::Politiecombi(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
    ENSURE(getType() == "politiecombi", "Politiecombi type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Politiecombi moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Politiecombi mag geen bus zijn.");
}

Politiecombi::Politiecombi(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
    ENSURE(getType() == "politiecombi", "Politiecombi type moet correct zijn.");
    ENSURE(isPrioriteitsvoertuig(), "Politiecombi moet een prioriteitsvoertuig zijn.");
    ENSURE(!isBus(), "Politiecombi mag geen bus zijn.");
}

std::string Politiecombi::getType() const {
    return "politiecombi";
}

bool Politiecombi::isPrioriteitsvoertuig() const {
    return true;
}

bool Politiecombi::isBus() const {
    return false;
}

double Politiecombi::getLengte() const {
    return 6.0;
}

double Politiecombi::getMaxSnelheid() const {
    return 17.2;
}

double Politiecombi::getMaxVersnelling() const {
    return 1.55;
}

double Politiecombi::getMaxRemFactor() const {
    return 4.92;
}

double Politiecombi::getMinVolgafstand() const {
    return 6.0;
}

std::string Politiecombi::getVisueleRepresentatie() const {
    return "P";
}

std::unique_ptr<Voertuig> Politiecombi::clone() const {
    return std::make_unique<Politiecombi>(*this);
}
