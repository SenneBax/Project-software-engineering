/**
 * @file voertuig.cpp
 * @brief Implementation of the Voertuig class (Revised with vehicle types)
 */

#include "voertuig.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "DesignByContract.h"

// Initialize static const member variable with vehicle type parameters from Appendix C
const std::map<Voertuig::VoertuigType, Voertuig::VoertuigParams> Voertuig::typeParameters = {
    {Voertuig::VoertuigType::AUTO, Voertuig::VoertuigParams(4.0, 16.6, 1.44, 4.61, 4.0, false)},
    {Voertuig::VoertuigType::BUS, Voertuig::VoertuigParams(12.0, 11.4, 1.22, 4.29, 12.0, false)},
    {Voertuig::VoertuigType::BRANDWEERWAGEN, Voertuig::VoertuigParams(10.0, 14.6, 1.33, 4.56, 10.0, true)},
    {Voertuig::VoertuigType::ZIEKENWAGEN, Voertuig::VoertuigParams(8.0, 15.5, 1.44, 4.47, 8.0, true)},
    {Voertuig::VoertuigType::POLITIECOMBI, Voertuig::VoertuigParams(6.0, 17.2, 1.55, 4.92, 6.0, true)}
};

/**
 * @brief Constructor
 * @param baan De naam van de baan waar het voertuig zich bevindt
 * @param positie De positie van het voertuig op de baan
 * @param type Het type van het voertuig
 */
Voertuig::Voertuig(const std::string& baan, double positie, const std::string& typeStr)
    : baanNaam(baan), positie(positie), snelheid(0.0), versnelling(0.0),
      type(stringToType(typeStr)), isWaitingAtStop(false) {

    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    //REQUIRE(positie > 0.0, "Positie moet positief zijn.");
    REQUIRE(typeParameters.find(type) != typeParameters.end(), "Ongeldige voertuigtype.");

    _initCheck = this;

    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");

}

bool Voertuig::properlyInitialized() const {
    return _initCheck == this;
}


/**
 * @brief Constructor met snelheid en versnelling
 * @param baan De naam van de baan waar het voertuig zich bevindt
 * @param positie De positie van het voertuig op de baan
 * @param snelheid De snelheid van het voertuig
 * @param versnelling De versnelling van het voertuig
 * @param type Het type van het voertuig
 */
Voertuig::Voertuig(const std::string& baan, double positie, double snelheid, double versnelling, const std::string& typeStr)
    : baanNaam(baan), positie(positie), snelheid(snelheid), versnelling(versnelling),
      type(stringToType(typeStr)), isWaitingAtStop(false) {

    REQUIRE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");
    REQUIRE(versnelling >= 0.0, "Versnelling mag niet negatief zijn.");

    _initCheck = this;

    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

/**
 * @brief Copy constructor
 * @param other Het te kopiëren voertuig
 */
Voertuig::Voertuig(const Voertuig& other)
    : baanNaam(other.baanNaam), positie(other.positie), snelheid(other.snelheid),
      versnelling(other.versnelling), type(other.type), isWaitingAtStop(other.isWaitingAtStop) {

    _initCheck = this;

    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

/**
 * @brief Destructor
 */
Voertuig::~Voertuig() {
    // Geen dynamisch geheugen om vrij te geven
}

/**
 * @brief Assignment operator
 * @param other Het voertuig waarvan de waarden worden overgenomen
 * @return Referentie naar dit voertuig
 */
Voertuig& Voertuig::operator=(const Voertuig& other) {
    if (this != &other) {
        baanNaam = other.baanNaam;
        positie = other.positie;
        snelheid = other.snelheid;
        versnelling = other.versnelling;
        type = other.type;
        isWaitingAtStop = other.isWaitingAtStop;
    }
    return *this;
}

/**
 * @brief Geeft de naam van de baan terug
 * @return De naam van de baan
 */
std::string Voertuig::getBaanNaam() const {
    REQUIRE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");

    return baanNaam;
}

/**
 * @brief Alias voor getBaanNaam voor compatibiliteit
 * @return De naam van de baan
 */
std::string Voertuig::getBaan() const {
    return baanNaam;
}

/**
 * @brief Sets the name of the road
 * @param nieuweNaam The new road name
 */
void Voertuig::setBaanNaam(const std::string& nieuweNaam) {
    REQUIRE(properlyInitialized(), "setBaanNaam moet eindigen in een geldige toestand.");
    REQUIRE(!nieuweNaam.empty(), "nieuweNaam mag niet leeg zijn.");
    baanNaam = nieuweNaam;
    ENSURE(baanNaam == nieuweNaam, "Baannaam werd niet correct ingesteld");
}


/**
 * @brief Geeft de positie van het voertuig terug
 * @return De positie op de baan
 */
double Voertuig::getPositie() const {
    REQUIRE(properlyInitialized(), "getPositie moet eindigen in een geldige toestand.");
    return positie;
}

/**
 * @brief Zet de positie van het voertuig
 * @param nieuwePositie De nieuwe positie
 */
void Voertuig::setPositie(double nieuwePositie) {
    REQUIRE(properlyInitialized(), "setPositie moet eindigen in een geldige toestand.");
    REQUIRE(nieuwePositie >= 0.0, "nieuwePositie mag niet negatief zijn.");
    positie = nieuwePositie;
    ENSURE(positie == nieuwePositie, "Positie werd niet correct ingesteld.");
}

/**
 * @brief Geeft het type van het voertuig terug
 * @return Het type van het voertuig
 */
std::string Voertuig::getType() const {
    REQUIRE(properlyInitialized(), "getType moet eindigen in een geldige toestand.");
    return typeToString(type);
}

/**
 * @brief Gets the vehicle type as enum
 * @return The vehicle type enum
 */
Voertuig::VoertuigType Voertuig::getTypeEnum() const {
    REQUIRE(properlyInitialized(), "getTypeEnum moet eindigen in een geldige toestand.");

    return type;
}

/**
 * @brief Checks if the vehicle is a priority vehicle
 * @return True if the vehicle is a priority vehicle (fire truck, ambulance, police)
 */
bool Voertuig::isPrioriteitsvoertuig() const {
    REQUIRE(properlyInitialized(), "isPrioriteitsvoertuig moet eindigen in een geldige toestand.");
    return typeParameters.at(type).isPrioriteitsvoertuig;
}

/**
 * @brief Checks if this vehicle is a bus
 * @return True if the vehicle is a bus
 */
bool Voertuig::isBus() const {
    REQUIRE(properlyInitialized(), "isBus moet eindigen in een geldige toestand.");

    return type == VoertuigType::BUS;
}

/**
 * @brief Geeft de snelheid van het voertuig terug
 * @return De snelheid in m/s
 */
double Voertuig::getSnelheid() const {
    REQUIRE(properlyInitialized(), "getSnelheid moet eindigen in een geldige toestand.");

    return snelheid;
}

/**
 * @brief Zet de snelheid van het voertuig
 * @param nieuweSnelheid De nieuwe snelheid
 */
void Voertuig::setSnelheid(double nieuweSnelheid) {
    REQUIRE(properlyInitialized(), "setSnelheid moet eindigen in een geldige toestand.");
    snelheid = std::max(0.0, nieuweSnelheid); // Prevent negative speed
    ENSURE(snelheid >= 0.0, "Snelheid mag niet negatief zijn.");
    ENSURE(snelheid == std::max(0.0, nieuweSnelheid), "Snelheid werd niet correct ingesteld.");
}

/**
 * @brief Geeft de versnelling van het voertuig terug
 * @return De versnelling in m/s²
 */
double Voertuig::getVersnelling() const {
    REQUIRE(properlyInitialized(), "getVersnelling moet eindigen in een geldige toestand.");
    return versnelling;
}

/**
 * @brief Zet de versnelling van het voertuig
 * @param nieuweVersnelling De nieuwe versnelling
 */
void Voertuig::setVersnelling(double nieuweVersnelling) {
    REQUIRE(properlyInitialized(), "setVersnelling moet eindigen in een geldige toestand.");
    REQUIRE(nieuweVersnelling >= 0.0, "nieuweVersnelling mag niet negatief zijn.");
    versnelling = nieuweVersnelling;
    ENSURE(versnelling == nieuweVersnelling, "versnelling werd niet correct ingesteld.");
}

/**
 * @brief Gets the length of the vehicle
 * @return The length in meters
 */
double Voertuig::getLengte() const {
    REQUIRE(properlyInitialized(), "getLengte moet eindigen in een geldige toestand.");
    return typeParameters.at(type).lengte;
}

/**
 * @brief Gets the maximum speed of the vehicle
 * @return The maximum speed in m/s
 */
double Voertuig::getMaxSnelheid() const {
    REQUIRE(properlyInitialized(), "getMaxSnelheid moet eindigen in een geldige toestand.");
    return typeParameters.at(type).maxSnelheid;
}

/**
 * @brief Gets the maximum acceleration of the vehicle
 * @return The maximum acceleration in m/s²
 */
double Voertuig::getMaxVersnelling() const {
    REQUIRE(properlyInitialized(), "getMaxVersnelling moet eindigen in een geldige toestand.");
    return typeParameters.at(type).maxVersnelling;
}

/**
 * @brief Gets the maximum braking factor of the vehicle
 * @return The maximum braking factor in m/s²
 */
double Voertuig::getMaxRemFactor() const {
    REQUIRE(properlyInitialized(), "getMaxRemfactor moet eindigen in een geldige toestand.");
    return typeParameters.at(type).maxRemFactor;
}

/**
 * @brief Gets the minimum following distance
 * @return The minimum following distance in meters
 */
double Voertuig::getMinVolgafstand() const {
    REQUIRE(properlyInitialized(), "getMinVolgafstand moet eindigen in een geldige toestand.");
    return typeParameters.at(type).minVolgafstand;
}

/**
 * @brief Sets the bus waiting flag
 * @param isWaiting Whether the bus is waiting at a stop
 */
void Voertuig::setIsWaitingAtBusStop(bool isWaiting) {
    REQUIRE(properlyInitialized(), "setIsWaitingAtBusStop moet eindigen in een geldige toestand.");
    isWaitingAtStop = isWaiting;
    ENSURE(isWaitingAtStop == isWaiting, "isWaitingAtStop werd niet correct ingesteld.");
}

/**
 * @brief Checks if the bus is waiting at a stop
 * @return True if the bus is waiting, false otherwise
 */
bool Voertuig::isWaitingAtBusStop() const {
    REQUIRE(properlyInitialized(), "setIsWaitingAtBusStop moet eindigen in een geldige toestand.");
    return isWaitingAtStop;
}

/**
 * @brief Update the vehicle's position and speed based on the current acceleration
 * @param tijdstap The time step for the update in seconds
 */
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

/**
 * @brief Calculate the vehicle's acceleration based on preceding vehicle and other factors
 * @param voorgaandVoertuig The preceding vehicle, nullptr if none
 * @param isEersteVoertuig Whether this is the first vehicle on the road
 * @param verkeersLichtVertraagFactor Slowdown factor for a traffic light (0.4 by default)
 * @param doelSnelheid Target speed, uses vehicle's maximum speed by default
 */
void Voertuig::berekenVersnelling(const Voertuig* voorgaandVoertuig, bool isEersteVoertuig,
                                 double verkeersLichtVertraagFactor, double doelSnelheid) {
    REQUIRE(properlyInitialized(), "updatePositieEnSnelheid moet eindigen in een geldige toestand.");
    //REQUIRE(doelSnelheid >= 0.0, "doelSnelheid mag niet negatief zijn.");

    // If this is a priority vehicle, it doesn't have to slow down for traffic lights
    if (isPrioriteitsvoertuig() && isEersteVoertuig) {
        // Priority vehicles use their max speed as target
        doelSnelheid = getMaxSnelheid();
    } else if (doelSnelheid < 0) {
        // If no target speed is provided, use max speed
        doelSnelheid = getMaxSnelheid();
    }

    // Calculate delta based on formulas in B.3
    double delta = 0.0;

    if (voorgaandVoertuig != nullptr) {
        // Calculate following distance
        const double deltaX = voorgaandVoertuig->getPositie() - positie - voorgaandVoertuig->getLengte();

        // Prevent negative or too small deltaX values that could lead to NaN or infinity
        if (deltaX <= 0.1) {
            // Emergency braking when distance is very small
            versnelling = -getMaxRemFactor();
            return;
        }

        // Calculate speed difference
        const double deltaV = snelheid - voorgaandVoertuig->getSnelheid();

        // Calculate interaction term delta
        double s_star = getMinVolgafstand() +
                        std::max(0.0, snelheid + snelheid * deltaV /
                        (2 * std::sqrt(getMaxVersnelling() * getMaxRemFactor())));
        delta = pow(s_star / deltaX, 2);
    }

    // Calculate acceleration according to formula in B.3
    double a = getMaxVersnelling() * (1 - pow(snelheid / doelSnelheid, 4) - delta);

    // Limit acceleration between -maxRemFactor and maxVersnelling
    a = std::max(-getMaxRemFactor(), std::min(getMaxVersnelling(), a));

    versnelling = a;

    ENSURE(versnelling >= -getMaxRemFactor() && versnelling <= getMaxVersnelling(), "versnelling buiten toegelaten grenzen.");
}

/**
 * @brief Apply emergency braking (vehicle comes to a stop)
 */
void Voertuig::noodStop() {
    REQUIRE(properlyInitialized(), "noodStop moet starten in een geldige toestand.");
    // Formula from B.5
    versnelling = -getMaxRemFactor() * snelheid / getMaxSnelheid();
    ENSURE(versnelling <= 0, "Noodstop moet negatieve versnelling geven.");
}

/**
 * @brief Convert a string type to enum
 * @param typeStr String representation of the type
 * @return The corresponding enum value
 */
Voertuig::VoertuigType Voertuig::stringToType(const std::string& typeStr) {
    if (typeStr == "auto") return VoertuigType::AUTO;
    if (typeStr == "bus") return VoertuigType::BUS;
    if (typeStr == "brandweerwagen") return VoertuigType::BRANDWEERWAGEN;
    if (typeStr == "ziekenwagen") return VoertuigType::ZIEKENWAGEN;
    if (typeStr == "politiecombi") return VoertuigType::POLITIECOMBI;

    // Default to AUTO if type is not recognized
    return VoertuigType::AUTO;
}

/**
 * @brief Convert an enum type to string
 * @param type The enum value
 * @return String representation of the type
 */
std::string Voertuig::typeToString(VoertuigType type) {
    switch (type) {
        case VoertuigType::AUTO: return "auto";
        case VoertuigType::BUS: return "bus";
        case VoertuigType::BRANDWEERWAGEN: return "brandweerwagen";
        case VoertuigType::ZIEKENWAGEN: return "ziekenwagen";
        case VoertuigType::POLITIECOMBI: return "politiecombi";
        default: return "onbekend";
    }
}

/**
 * @brief Get the parameters for a specific vehicle type
 * @param type The vehicle type
 * @return The parameters for the given type
 */
Voertuig::VoertuigParams Voertuig::getVoertuigParams(VoertuigType type) {
    return typeParameters.at(type);
}