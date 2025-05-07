/**
 * @file voertuig.cpp
 * @brief Implementatie van de Voertuig klasse (Herzien met polymorfisme)
 */

#include "voertuig.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "DesignByContract.h"

// Basisklasse Voertuig implementatie
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
    snelheid = std::max(0.0, nieuweSnelheid); // Voorkom negatieve snelheid
    ENSURE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");
    ENSURE(snelheid == std::max(0.0, nieuweSnelheid), "Snelheid werd niet correct ingesteld.");
}

double Voertuig::getVersnelling() const {
    REQUIRE(properlyInitialized(), "getVersnelling moet eindigen in een geldige toestand.");
    return versnelling;
}

void Voertuig::setVersnelling(double nieuweVersnelling) {
    REQUIRE(properlyInitialized(), "setVersnelling moet eindigen in een geldige toestand.");
    versnelling = nieuweVersnelling;
    ENSURE(versnelling == nieuweVersnelling, "versnelling werd niet correct ingesteld.");
}

void Voertuig::setIsWaitingAtBusStop(bool isWaiting) {
    REQUIRE(properlyInitialized(), "setIsWaitingAtBusStop moet eindigen in een geldige toestand.");
    isWaitingAtStop = isWaiting;
    ENSURE(isWaitingAtStop == isWaiting, "isWaitingAtStop werd niet correct ingesteld.");
}

bool Voertuig::isWaitingAtBusStop() const {
    REQUIRE(properlyInitialized(), "setIsWaitingAtBusStop moet eindigen in een geldige toestand.");
    return isWaitingAtStop;
}

void Voertuig::updatePositieEnSnelheid(double tijdstap) {
    REQUIRE(properlyInitialized(), "updatePositieEnSnelheid moet eindigen in een geldige toestand.");
    REQUIRE(tijdstap >= 0.0, "tijd moet positief zijn.");

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
    ENSURE(snelheid >= 0.0, "snelheid moet positief zijn.");
}

void Voertuig::berekenVersnelling(const Voertuig* voorgaandVoertuig, bool isEersteVoertuig,
                                 double verkeersLichtVertraagFactor, double doelSnelheid) {
    REQUIRE(properlyInitialized(), "updatePositieEnSnelheid moet eindigen in een geldige toestand.");

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

    ENSURE(versnelling >= -getMaxRemFactor() && versnelling <= getMaxVersnelling(), "versnelling buiten toegelaten grenzen.");
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

// Factory methode implementatie
std::unique_ptr<Voertuig> Voertuig::maakVoertuig(const std::string& baan, double positie, const std::string& type) {
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

std::unique_ptr<Voertuig> Voertuig::maakVoertuig(const std::string& baan, double positie, double snelheid,
                               double versnelling, const std::string& type) {
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

// Auto implementatie
Auto::Auto(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
}

Auto::Auto(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
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

std::unique_ptr<Voertuig> Auto::clone() const {
    return std::make_unique<Auto>(*this);
}

// Bus implementatie
Bus::Bus(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
}

Bus::Bus(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
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

std::unique_ptr<Voertuig> Bus::clone() const {
    return std::make_unique<Bus>(*this);
}

// Brandweerwagen implementatie
Brandweerwagen::Brandweerwagen(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
}

Brandweerwagen::Brandweerwagen(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
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

std::unique_ptr<Voertuig> Brandweerwagen::clone() const {
    return std::make_unique<Brandweerwagen>(*this);
}

// Ziekenwagen implementatie
Ziekenwagen::Ziekenwagen(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
}

Ziekenwagen::Ziekenwagen(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
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

std::unique_ptr<Voertuig> Ziekenwagen::clone() const {
    return std::make_unique<Ziekenwagen>(*this);
}

// Politiecombi implementatie
Politiecombi::Politiecombi(const std::string& baan, double positie)
    : Voertuig(baan, positie) {
}

Politiecombi::Politiecombi(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {
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

std::unique_ptr<Voertuig> Politiecombi::clone() const {
    return std::make_unique<Politiecombi>(*this);
}