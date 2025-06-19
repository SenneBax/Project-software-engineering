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
    : baanNaam(baan), positie(positie), snelheid(0.0), versnelling(0.0), waitingAtStop(false) {
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "Positie moet positief zijn.");

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

Voertuig::Voertuig(const std::string& baan, double positie, double snelheid, double versnelling)
    : baanNaam(baan), positie(positie), snelheid(snelheid), versnelling(versnelling), waitingAtStop(false) {
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "Positie moet positief zijn.");
    REQUIRE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

Voertuig::~Voertuig() {
    // Geen dynamisch geheugen om vrij te geven
}

Voertuig::Voertuig(const Voertuig& other)
    : baanNaam(other.baanNaam), positie(other.positie), snelheid(other.snelheid),
      versnelling(other.versnelling), waitingAtStop(other.waitingAtStop) {
    _initCheck = this;
    ENSURE(properlyInitialized(), "Copy constructor moet eindigen in een geldige toestand.");
}

Voertuig& Voertuig::operator=(const Voertuig& other) {
    if (this != &other) {
        baanNaam = other.baanNaam;
        positie = other.positie;
        snelheid = other.snelheid;
        versnelling = other.versnelling;
        waitingAtStop = other.waitingAtStop;
    }
    return *this;
}

bool Voertuig::properlyInitialized() const {
    return _initCheck == this;
}

std::string Voertuig::getBaanNaam() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    return baanNaam;
}

std::string Voertuig::getBaan() const {
    return getBaanNaam();
}

void Voertuig::setBaanNaam(const std::string& nieuweNaam) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(!nieuweNaam.empty(), "Nieuwe baannaam mag niet leeg zijn.");
    baanNaam = nieuweNaam;
    ENSURE(getBaanNaam() == nieuweNaam, "Baannaam werd niet correct ingesteld.");
}

double Voertuig::getPositie() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    return positie;
}

void Voertuig::setPositie(double nieuwePositie) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(nieuwePositie >= 0.0, "Nieuwe positie mag niet negatief zijn.");
    positie = nieuwePositie;
    ENSURE(getPositie() == nieuwePositie, "Positie werd niet correct ingesteld.");
}

double Voertuig::getSnelheid() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    return snelheid;
}

void Voertuig::setSnelheid(double nieuweSnelheid) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(nieuweSnelheid >= 0.0, "Nieuwe snelheid mag niet negatief zijn.");
    snelheid = nieuweSnelheid;
    ENSURE(getSnelheid() == nieuweSnelheid, "Snelheid werd niet correct ingesteld.");
}

double Voertuig::getVersnelling() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    return versnelling;
}

void Voertuig::setVersnelling(double nieuweVersnelling) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    versnelling = nieuweVersnelling;
    ENSURE(getVersnelling() == nieuweVersnelling, "Versnelling werd niet correct ingesteld.");
}

void Voertuig::setIsWaitingAtBusStop(bool isWaiting) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    waitingAtStop = isWaiting;
    ENSURE(waitingAtStop == isWaiting, "Waiting status werd niet correct ingesteld.");
}

bool Voertuig::isWaitingAtBusStop() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    return waitingAtStop;
}

bool Voertuig::isWaitingAtStop() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    return waitingAtStop;
}

void Voertuig::setIsWaitingAtStop(bool waiting) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    waitingAtStop = waiting;
    ENSURE(isWaitingAtStop() == waiting, "Waiting status werd niet correct ingesteld.");
}

void Voertuig::updatePositieEnSnelheid(double tijdstap) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(tijdstap >= 0.0, "Tijdstap moet positief zijn.");

    // Formules uit B.2 van de specificatie
    if (snelheid + versnelling * tijdstap < 0) {
        // Snelheid zou negatief worden, pas positie aan en zet snelheid op 0
        positie = positie - (snelheid * snelheid) / (2 * versnelling);
        snelheid = 0.0;
    } else {
        // Normale situatie, update snelheid en dan positie
        snelheid = snelheid + versnelling * tijdstap;
        snelheid = std::max(0.0, snelheid); // Zorg ervoor dat de snelheid niet negatief wordt

        // Bereken nieuwe positie
        positie = positie + snelheid * tijdstap + (versnelling * tijdstap * tijdstap) / 2;
    }
    ENSURE(snelheid >= 0.0, "Snelheid moet positief zijn.");
}

void Voertuig::berekenVersnelling(const Voertuig* voorgaandVoertuig, bool isEersteVoertuig,
                                 double verkeersLichtVertraagFactor, double doelSnelheid) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    // Als dit een prioriteitsvoertuig is, hoeft het niet af te remmen voor verkeerslichten
    if (isPrioriteitsvoertuig() && isEersteVoertuig) {
        // Prioriteitsvoertuigen gebruiken hun maximumsnelheid als doel
        doelSnelheid = getMaxSnelheid();
    } else if (doelSnelheid < 0) {
        // Als geen doelsnelheid is opgegeven, gebruik maximumsnelheid
        doelSnelheid = getMaxSnelheid();
    }

    // Bereken delta op basis van formules in B.3
    double delta = 0.0;

    if (voorgaandVoertuig != nullptr) {
        // Bereken volgafstand
        const double deltaX = voorgaandVoertuig->getPositie() - positie - voorgaandVoertuig->getLengte();

        // Voorkom negatieve of te kleine deltaX waarden die kunnen leiden tot NaN of oneindigheid
        if (deltaX <= 0.1) {
            // Noodstop als afstand zeer klein is
            versnelling = -getMaxRemFactor();
            return;
        }

        // Bereken snelheidsverschil
        const double deltaV = snelheid - voorgaandVoertuig->getSnelheid();

        // Bereken interactieterm delta
        double s_star = getMinVolgafstand() +
                        std::max(0.0, snelheid + snelheid * deltaV /
                        (2 * std::sqrt(getMaxVersnelling() * getMaxRemFactor())));
        delta = pow(s_star / deltaX, 2);
    }

    // Bereken versnelling volgens formule in B.3
    double a = getMaxVersnelling() * (1 - pow(snelheid / doelSnelheid, 4) - delta);

    // Beperk versnelling tussen -maxRemFactor en maxVersnelling
    a = std::max(-getMaxRemFactor(), std::min(getMaxVersnelling(), a));

    versnelling = a;

    ENSURE(versnelling >= -getMaxRemFactor() && versnelling <= getMaxVersnelling(), "Versnelling buiten toegelaten grenzen.");
}

void Voertuig::noodStop() {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    // Formule uit B.5
    versnelling = -getMaxRemFactor() * snelheid / getMaxSnelheid();
    ENSURE(versnelling <= 0, "Noodstop moet negatieve versnelling geven.");
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
    return 11.4;
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