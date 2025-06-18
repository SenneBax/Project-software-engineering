/**
 * @file voertuig.cpp
 * @brief Implementatie van de Voertuig klasse en afgeleide klassen met verbeterde veiligheid
 * @author Generated with fixes for segmentation faults
 * @date 2025
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
 * @brief Constructor voor voertuig met minimale parameters
 */
Voertuig::Voertuig(const std::string& baan, double positie)
    : baanNaam(baan), positie(positie), snelheid(0.0), versnelling(0.0), waitingAtStop(false) {

    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "Positie moet positief zijn.");

    _initCheck = this;

    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
    ENSURE(getBaanNaam() == baan, "Baannaam werd niet correct ingesteld.");
    ENSURE(getPositie() == positie, "Positie werd niet correct ingesteld.");
    ENSURE(getSnelheid() == 0.0, "Snelheid moet 0 zijn bij initialisatie.");
    ENSURE(!isWaitingAtStop(), "Voertuig mag niet wachten bij initialisatie.");
}

/**
 * @brief Constructor voor voertuig met alle fysische parameters
 */
Voertuig::Voertuig(const std::string& baan, double positie, double snelheid, double versnelling)
    : baanNaam(baan), positie(positie), snelheid(snelheid), versnelling(versnelling), waitingAtStop(false) {

    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "Positie moet positief zijn.");
    REQUIRE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");

    _initCheck = this;

    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
    ENSURE(getBaanNaam() == baan, "Baannaam werd niet correct ingesteld.");
    ENSURE(getPositie() == positie, "Positie werd niet correct ingesteld.");
    ENSURE(getSnelheid() == snelheid, "Snelheid werd niet correct ingesteld.");
    ENSURE(getVersnelling() == versnelling, "Versnelling werd niet correct ingesteld.");
}

/**
 * @brief Virtual destructor voor correcte afhandeling van afgeleide klassen
 */
Voertuig::~Voertuig() {
    // Geen dynamisch geheugen om vrij te geven in de basisklasse
}

/**
 * @brief Copy constructor
 */
Voertuig::Voertuig(const Voertuig& other)
    : baanNaam(other.baanNaam), positie(other.positie), snelheid(other.snelheid),
      versnelling(other.versnelling), waitingAtStop(other.waitingAtStop) {

    REQUIRE(other.properlyInitialized(), "Source object moet properly initialized zijn.");

    _initCheck = this;

    ENSURE(properlyInitialized(), "Copy constructor moet eindigen in een geldige toestand.");
    ENSURE(getBaanNaam() == other.getBaanNaam(), "Baannaam werd niet correct gekopieerd.");
    ENSURE(getPositie() == other.getPositie(), "Positie werd niet correct gekopieerd.");
    ENSURE(getSnelheid() == other.getSnelheid(), "Snelheid werd niet correct gekopieerd.");
    ENSURE(getVersnelling() == other.getVersnelling(), "Versnelling werd niet correct gekopieerd.");
}

/**
 * @brief Assignment operator
 */
Voertuig& Voertuig::operator=(const Voertuig& other) {
    REQUIRE(properlyInitialized(), "This object moet properly initialized zijn.");
    REQUIRE(other.properlyInitialized(), "Source object moet properly initialized zijn.");

    if (this != &other) {
        baanNaam = other.baanNaam;
        positie = other.positie;
        snelheid = other.snelheid;
        versnelling = other.versnelling;
        waitingAtStop = other.waitingAtStop;
    }

    ENSURE(properlyInitialized(), "Assignment operator moet eindigen in een geldige toestand.");
    ENSURE(getBaanNaam() == other.getBaanNaam(), "Baannaam werd niet correct toegewezen.");
    ENSURE(getPositie() == other.getPositie(), "Positie werd niet correct toegewezen.");
    ENSURE(getSnelheid() == other.getSnelheid(), "Snelheid werd niet correct toegewezen.");
    ENSURE(getVersnelling() == other.getVersnelling(), "Versnelling werd niet correct toegewezen.");

    return *this;
}

/**
 * @brief Controleert of het voertuig correct geïnitialiseerd is
 */
bool Voertuig::properlyInitialized() const {
    return _initCheck == this;
}

/**
 * @brief Geeft de naam van de baan terug waar het voertuig zich bevindt
 */
std::string Voertuig::getBaanNaam() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    ENSURE(!baanNaam.empty(), "Baannaam mag niet leeg zijn.");
    return baanNaam;
}

/**
 * @brief Alias voor getBaanNaam voor compatibiliteit
 */
std::string Voertuig::getBaan() const {
    return getBaanNaam();
}

/**
 * @brief Stelt de naam van de baan in waar het voertuig zich bevindt
 */
void Voertuig::setBaanNaam(const std::string& nieuweNaam) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(!nieuweNaam.empty(), "Nieuwe baannaam mag niet leeg zijn.");

    baanNaam = nieuweNaam;

    ENSURE(getBaanNaam() == nieuweNaam, "Baannaam werd niet correct ingesteld.");
}

/**
 * @brief Geeft de huidige positie van het voertuig op de baan terug
 */
double Voertuig::getPositie() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    ENSURE(positie >= 0.0, "Positie moet positief zijn.");
    return positie;
}

/**
 * @brief Stelt de positie van het voertuig in
 */
void Voertuig::setPositie(double nieuwePositie) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(nieuwePositie >= 0.0, "Nieuwe positie mag niet negatief zijn.");

    positie = nieuwePositie;

    ENSURE(getPositie() == nieuwePositie, "Positie werd niet correct ingesteld.");
}

/**
 * @brief Geeft de huidige snelheid van het voertuig terug
 */
double Voertuig::getSnelheid() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    ENSURE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");
    return snelheid;
}

/**
 * @brief Stelt de snelheid van het voertuig in
 */
void Voertuig::setSnelheid(double nieuweSnelheid) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(nieuweSnelheid >= 0.0, "Nieuwe snelheid mag niet negatief zijn.");

    snelheid = nieuweSnelheid;

    ENSURE(getSnelheid() == nieuweSnelheid, "Snelheid werd niet correct ingesteld.");
}

/**
 * @brief Geeft de huidige versnelling van het voertuig terug
 */
double Voertuig::getVersnelling() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    return versnelling;
}

/**
 * @brief Stelt de versnelling van het voertuig in
 */
void Voertuig::setVersnelling(double nieuweVersnelling) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    versnelling = nieuweVersnelling;

    ENSURE(getVersnelling() == nieuweVersnelling, "Versnelling werd niet correct ingesteld.");
}

/**
 * @brief Controleert of het voertuig momenteel wacht bij een halte
 */
bool Voertuig::isWaitingAtStop() const {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    return waitingAtStop;
}

/**
 * @brief Stelt in of het voertuig wacht bij een halte
 */
void Voertuig::setWaitingAtStop(bool waiting) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");

    waitingAtStop = waiting;

    ENSURE(isWaitingAtStop() == waiting, "Waiting status werd niet correct ingesteld.");
}

/**
 * @brief Update de positie en snelheid van het voertuig gedurende een tijdstap
 */
void Voertuig::update(double deltaTime) {
    REQUIRE(properlyInitialized(), "Voertuig moet properly initialized zijn.");
    REQUIRE(deltaTime > 0.0, "Delta time moet positief zijn.");

    double oudePositie = positie;
    double oudeSnelheid = snelheid;

    // Bereken nieuwe positie: s = s₀ + v₀t + ½at²
    positie = oudePositie + oudeSnelheid * deltaTime + 0.5 * versnelling * deltaTime * deltaTime;

    // Bereken nieuwe snelheid: v = v₀ + at
    snelheid = oudeSnelheid + versnelling * deltaTime;

    // Voorkom negatieve snelheid (voertuigen kunnen niet achteruit rijden)
    if (snelheid < 0.0) {
        snelheid = 0.0;
    }

    // Voorkom negatieve positie
    if (positie < 0.0) {
        positie = 0.0;
    }

    ENSURE(properlyInitialized(), "Update moet eindigen in een geldige toestand.");
    ENSURE(getSnelheid() >= 0.0, "Snelheid mag nooit negatief worden.");
    ENSURE(getPositie() >= 0.0, "Positie mag nooit negatief worden.");
}

/**
 * @brief Factory methode voor het creëren van voertuigen van verschillende types
 */
std::unique_ptr<Voertuig> Voertuig::maakVoertuig(const std::string& baan, double positie, const std::string& type) {
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "Positie moet positief zijn.");
    REQUIRE(!type.empty(), "Type mag niet leeg zijn.");

    std::unique_ptr<Voertuig> voertuig = nullptr;

    try {
        if (type == "auto") {
            voertuig = std::make_unique<Auto>(baan, positie);
        } else if (type == "bus") {
            voertuig = std::make_unique<Bus>(baan, positie);
        } else if (type == "brandweerwagen") {
            voertuig = std::make_unique<Brandweerwagen>(baan, positie);
        } else if (type == "ziekenwagen") {
            voertuig = std::make_unique<Ziekenwagen>(baan, positie);
        } else if (type == "politiecombi") {
            voertuig = std::make_unique<Politiecombi>(baan, positie);
        }
        // Voor onbekende types blijft voertuig nullptr

    } catch (const std::exception& e) {
        // Bij fout tijdens constructie, return nullptr
        voertuig = nullptr;
    }

    ENSURE(voertuig == nullptr || voertuig->properlyInitialized(),
           "Gecreëerd voertuig moet properly initialized zijn.");
    ENSURE(voertuig == nullptr || voertuig->getBaanNaam() == baan,
           "Gecreëerd voertuig moet op juiste baan staan.");
    ENSURE(voertuig == nullptr || voertuig->getPositie() == positie,
           "Gecreëerd voertuig moet op juiste positie staan.");

    return voertuig;
}

// =============================================================================
// Auto klasse implementatie
// =============================================================================

/**
 * @brief Constructor voor een auto
 */
Auto::Auto(const std::string& baan, double positie)
    : Voertuig(baan, positie) {

    ENSURE(properlyInitialized(), "Auto constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "auto", "Auto type moet correct zijn.");
    ENSURE(!isPrioriteitsVoertuig(), "Auto mag geen prioriteitsvoertuig zijn.");
}

/**
 * @brief Constructor voor een auto met snelheid en versnelling
 */
Auto::Auto(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {

    ENSURE(properlyInitialized(), "Auto constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "auto", "Auto type moet correct zijn.");
    ENSURE(!isPrioriteitsVoertuig(), "Auto mag geen prioriteitsvoertuig zijn.");
}

/**
 * @brief Geeft het type van het voertuig terug
 */
std::string Auto::getType() const {
    REQUIRE(properlyInitialized(), "Auto moet properly initialized zijn.");
    return "auto";
}

/**
 * @brief Controleert of dit een prioriteitsvoertuig is
 */
bool Auto::isPrioriteitsVoertuig() const {
    REQUIRE(properlyInitialized(), "Auto moet properly initialized zijn.");
    return false;
}

/**
 * @brief Geeft de visuele representatie terug
 */
std::string Auto::getVisueleRepresentatie() const {
    REQUIRE(properlyInitialized(), "Auto moet properly initialized zijn.");

    ENSURE(std::string("A").length() == 1, "Visuele representatie moet 1 karakter zijn.");
    return "A";
}

/**
 * @brief Controleert of dit voertuig een bus is
 */
bool Auto::isBus() const {
    REQUIRE(properlyInitialized(), "Auto moet properly initialized zijn.");
    return false;
}

/**
 * @brief Maakt een kopie van dit voertuig
 */
std::unique_ptr<Voertuig> Auto::clone() const {
    REQUIRE(properlyInitialized(), "Auto moet properly initialized zijn.");
    return std::make_unique<Auto>(*this);
}

// =============================================================================
// Bus klasse implementatie
// =============================================================================

/**
 * @brief Constructor voor een bus
 */
Bus::Bus(const std::string& baan, double positie)
    : Voertuig(baan, positie) {

    ENSURE(properlyInitialized(), "Bus constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "bus", "Bus type moet correct zijn.");
    ENSURE(!isPrioriteitsVoertuig(), "Bus mag geen prioriteitsvoertuig zijn.");
}

/**
 * @brief Constructor voor een bus met snelheid en versnelling
 */
Bus::Bus(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {

    ENSURE(properlyInitialized(), "Bus constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "bus", "Bus type moet correct zijn.");
    ENSURE(!isPrioriteitsVoertuig(), "Bus mag geen prioriteitsvoertuig zijn.");
}

/**
 * @brief Geeft het type van het voertuig terug
 */
std::string Bus::getType() const {
    REQUIRE(properlyInitialized(), "Bus moet properly initialized zijn.");
    return "bus";
}

/**
 * @brief Controleert of dit een prioriteitsvoertuig is
 */
bool Bus::isPrioriteitsVoertuig() const {
    REQUIRE(properlyInitialized(), "Bus moet properly initialized zijn.");
    return false;
}

/**
 * @brief Geeft de visuele representatie terug
 */
std::string Bus::getVisueleRepresentatie() const {
    REQUIRE(properlyInitialized(), "Bus moet properly initialized zijn.");

    ENSURE(std::string("B").length() == 1, "Visuele representatie moet 1 karakter zijn.");
    return "B";
}

/**
 * @brief Controleert of dit voertuig een bus is
 */
bool Bus::isBus() const {
    REQUIRE(properlyInitialized(), "Bus moet properly initialized zijn.");
    return true;
}

/**
 * @brief Maakt een kopie van dit voertuig
 */
std::unique_ptr<Voertuig> Bus::clone() const {
    REQUIRE(properlyInitialized(), "Bus moet properly initialized zijn.");
    return std::make_unique<Bus>(*this);
}

// =============================================================================
// Brandweerwagen klasse implementatie
// =============================================================================

/**
 * @brief Constructor voor een brandweerwagen
 */
Brandweerwagen::Brandweerwagen(const std::string& baan, double positie)
    : Voertuig(baan, positie) {

    ENSURE(properlyInitialized(), "Brandweerwagen constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "brandweerwagen", "Brandweerwagen type moet correct zijn.");
    ENSURE(isPrioriteitsVoertuig(), "Brandweerwagen moet een prioriteitsvoertuig zijn.");
}

/**
 * @brief Constructor voor een brandweerwagen met snelheid en versnelling
 */
Brandweerwagen::Brandweerwagen(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {

    ENSURE(properlyInitialized(), "Brandweerwagen constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "brandweerwagen", "Brandweerwagen type moet correct zijn.");
    ENSURE(isPrioriteitsVoertuig(), "Brandweerwagen moet een prioriteitsvoertuig zijn.");
}

/**
 * @brief Geeft het type van het voertuig terug
 */
std::string Brandweerwagen::getType() const {
    REQUIRE(properlyInitialized(), "Brandweerwagen moet properly initialized zijn.");
    return "brandweerwagen";
}

/**
 * @brief Controleert of dit een prioriteitsvoertuig is
 */
bool Brandweerwagen::isPrioriteitsVoertuig() const {
    REQUIRE(properlyInitialized(), "Brandweerwagen moet properly initialized zijn.");
    return true;
}

/**
 * @brief Geeft de visuele representatie terug
 */
std::string Brandweerwagen::getVisueleRepresentatie() const {
    REQUIRE(properlyInitialized(), "Brandweerwagen moet properly initialized zijn.");

    ENSURE(std::string("F").length() == 1, "Visuele representatie moet 1 karakter zijn.");
    return "F";
}

/**
 * @brief Controleert of dit voertuig een bus is
 */
bool Brandweerwagen::isBus() const {
    REQUIRE(properlyInitialized(), "Brandweerwagen moet properly initialized zijn.");
    return false;
}

/**
 * @brief Maakt een kopie van dit voertuig
 */
std::unique_ptr<Voertuig> Brandweerwagen::clone() const {
    REQUIRE(properlyInitialized(), "Brandweerwagen moet properly initialized zijn.");
    return std::make_unique<Brandweerwagen>(*this);
}

// =============================================================================
// Ziekenwagen klasse implementatie
// =============================================================================

/**
 * @brief Constructor voor een ziekenwagen
 */
Ziekenwagen::Ziekenwagen(const std::string& baan, double positie)
    : Voertuig(baan, positie) {

    ENSURE(properlyInitialized(), "Ziekenwagen constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "ziekenwagen", "Ziekenwagen type moet correct zijn.");
    ENSURE(isPrioriteitsVoertuig(), "Ziekenwagen moet een prioriteitsvoertuig zijn.");
}

/**
 * @brief Constructor voor een ziekenwagen met snelheid en versnelling
 */
Ziekenwagen::Ziekenwagen(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {

    ENSURE(properlyInitialized(), "Ziekenwagen constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "ziekenwagen", "Ziekenwagen type moet correct zijn.");
    ENSURE(isPrioriteitsVoertuig(), "Ziekenwagen moet een prioriteitsvoertuig zijn.");
}

/**
 * @brief Geeft het type van het voertuig terug
 */
std::string Ziekenwagen::getType() const {
    REQUIRE(properlyInitialized(), "Ziekenwagen moet properly initialized zijn.");
    return "ziekenwagen";
}

/**
 * @brief Controleert of dit een prioriteitsvoertuig is
 */
bool Ziekenwagen::isPrioriteitsVoertuig() const {
    REQUIRE(properlyInitialized(), "Ziekenwagen moet properly initialized zijn.");
    return true;
}

/**
 * @brief Geeft de visuele representatie terug
 */
std::string Ziekenwagen::getVisueleRepresentatie() const {
    REQUIRE(properlyInitialized(), "Ziekenwagen moet properly initialized zijn.");

    ENSURE(std::string("Z").length() == 1, "Visuele representatie moet 1 karakter zijn.");
    return "Z";
}

/**
 * @brief Controleert of dit voertuig een bus is
 */
bool Ziekenwagen::isBus() const {
    REQUIRE(properlyInitialized(), "Ziekenwagen moet properly initialized zijn.");
    return false;
}

/**
 * @brief Maakt een kopie van dit voertuig
 */
std::unique_ptr<Voertuig> Ziekenwagen::clone() const {
    REQUIRE(properlyInitialized(), "Ziekenwagen moet properly initialized zijn.");
    return std::make_unique<Ziekenwagen>(*this);
}

// =============================================================================
// Politiecombi klasse implementatie
// =============================================================================

/**
 * @brief Constructor voor een politiecombi
 */
Politiecombi::Politiecombi(const std::string& baan, double positie)
    : Voertuig(baan, positie) {

    ENSURE(properlyInitialized(), "Politiecombi constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "politiecombi", "Politiecombi type moet correct zijn.");
    ENSURE(isPrioriteitsVoertuig(), "Politiecombi moet een prioriteitsvoertuig zijn.");
}

/**
 * @brief Constructor voor een politiecombi met snelheid en versnelling
 */
Politiecombi::Politiecombi(const std::string& baan, double positie, double snelheid, double versnelling)
    : Voertuig(baan, positie, snelheid, versnelling) {

    ENSURE(properlyInitialized(), "Politiecombi constructor moet eindigen in een geldige toestand.");
    ENSURE(getType() == "politiecombi", "Politiecombi type moet correct zijn.");
    ENSURE(isPrioriteitsVoertuig(), "Politiecombi moet een prioriteitsvoertuig zijn.");
}

/**
 * @brief Geeft het type van het voertuig terug
 */
std::string Politiecombi::getType() const {
    REQUIRE(properlyInitialized(), "Politiecombi moet properly initialized zijn.");
    return "politiecombi";
}

/**
 * @brief Controleert of dit een prioriteitsvoertuig is
 */
bool Politiecombi::isPrioriteitsVoertuig() const {
    REQUIRE(properlyInitialized(), "Politiecombi moet properly initialized zijn.");
    return true;
}

/**
 * @brief Geeft de visuele representatie terug
 */
std::string Politiecombi::getVisueleRepresentatie() const {
    REQUIRE(properlyInitialized(), "Politiecombi moet properly initialized zijn.");

    ENSURE(std::string("P").length() == 1, "Visuele representatie moet 1 karakter zijn.");
    return "P";
}

/**
 * @brief Controleert of dit voertuig een bus is
 */
bool Politiecombi::isBus() const {
    REQUIRE(properlyInitialized(), "Politiecombi moet properly initialized zijn.");
    return false;
}

/**
 * @brief Maakt een kopie van dit voertuig
 */
std::unique_ptr<Voertuig> Politiecombi::clone() const {
    REQUIRE(properlyInitialized(), "Politiecombi moet properly initialized zijn.");
    return std::make_unique<Politiecombi>(*this);
}