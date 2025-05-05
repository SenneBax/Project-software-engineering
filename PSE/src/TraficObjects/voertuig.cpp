/**
 * @file voertuig.cpp
 * @brief Implementatie van de Voertuig klasse (Herzien met voertuigtypes)
 */

#include "voertuig.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "DesignByContract.h"

// Initialiseer statische const member variabele met voertuigtype parameters uit Appendix C
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
 * @brief Stelt de naam van de baan in
 * @param nieuweNaam De nieuwe baannaam
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
 * @brief Krijgt het voertuigtype als enum
 * @return Het voertuigtype enum
 */
Voertuig::VoertuigType Voertuig::getTypeEnum() const {
    REQUIRE(properlyInitialized(), "getTypeEnum moet eindigen in een geldige toestand.");

    return type;
}

/**
 * @brief Controleert of het voertuig een prioriteitsvoertuig is
 * @return True als het voertuig een prioriteitsvoertuig is (brandweerwagen, ziekenwagen, politie)
 */
bool Voertuig::isPrioriteitsvoertuig() const {
    REQUIRE(properlyInitialized(), "isPrioriteitsvoertuig moet eindigen in een geldige toestand.");
    return typeParameters.at(type).isPrioriteitsvoertuig;
}

/**
 * @brief Controleert of dit voertuig een bus is
 * @return True als het voertuig een bus is
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
    snelheid = std::max(0.0, nieuweSnelheid); // Voorkom negatieve snelheid
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
 * @brief Haalt de lengte van het voertuig op
 * @return De lengte in meters
 */
double Voertuig::getLengte() const {
    REQUIRE(properlyInitialized(), "getLengte moet eindigen in een geldige toestand.");
    return typeParameters.at(type).lengte;
}

/**
 * @brief Haalt de maximumsnelheid van het voertuig op
 * @return De maximumsnelheid in m/s
 */
double Voertuig::getMaxSnelheid() const {
    REQUIRE(properlyInitialized(), "getMaxSnelheid moet eindigen in een geldige toestand.");
    return typeParameters.at(type).maxSnelheid;
}

/**
 * @brief Haalt de maximumversnelling van het voertuig op
 * @return De maximumversnelling in m/s²
 */
double Voertuig::getMaxVersnelling() const {
    REQUIRE(properlyInitialized(), "getMaxVersnelling moet eindigen in een geldige toestand.");
    return typeParameters.at(type).maxVersnelling;
}

/**
 * @brief Haalt de maximale remfactor van het voertuig op
 * @return De maximale remfactor in m/s²
 */
double Voertuig::getMaxRemFactor() const {
    REQUIRE(properlyInitialized(), "getMaxRemfactor moet eindigen in een geldige toestand.");
    return typeParameters.at(type).maxRemFactor;
}

/**
 * @brief Haalt de minimale volgafstand op
 * @return De minimale volgafstand in meters
 */
double Voertuig::getMinVolgafstand() const {
    REQUIRE(properlyInitialized(), "getMinVolgafstand moet eindigen in een geldige toestand.");
    return typeParameters.at(type).minVolgafstand;
}

/**
 * @brief Stelt de wachtende bus vlag in
 * @param isWaiting Of de bus wacht bij een halte
 */
void Voertuig::setIsWaitingAtBusStop(bool isWaiting) {
    REQUIRE(properlyInitialized(), "setIsWaitingAtBusStop moet eindigen in een geldige toestand.");
    isWaitingAtStop = isWaiting;
    ENSURE(isWaitingAtStop == isWaiting, "isWaitingAtStop werd niet correct ingesteld.");
}

/**
 * @brief Controleert of de bus wacht bij een halte
 * @return True als de bus wacht, anders false
 */
bool Voertuig::isWaitingAtBusStop() const {
    REQUIRE(properlyInitialized(), "setIsWaitingAtBusStop moet eindigen in een geldige toestand.");
    return isWaitingAtStop;
}

/**
 * @brief Update de positie en snelheid van het voertuig op basis van de huidige versnelling
 * @param tijdstap De tijdstap voor de update in seconden
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
 * @brief Bereken de versnelling van het voertuig op basis van voorliggend voertuig en andere factoren
 * @param voorgaandVoertuig Het voorliggende voertuig, nullptr als er geen is
 * @param isEersteVoertuig Of dit het eerste voertuig op de weg is
 * @param verkeersLichtVertraagFactor Vertragingsfactor voor een verkeerslicht (0.4 standaard)
 * @param doelSnelheid Doelsnelheid, gebruikt voertuig's maximumsnelheid als standaard
 */
void Voertuig::berekenVersnelling(const Voertuig* voorgaandVoertuig, bool isEersteVoertuig,
                                 double verkeersLichtVertraagFactor, double doelSnelheid) {
    REQUIRE(properlyInitialized(), "updatePositieEnSnelheid moet eindigen in een geldige toestand.");
    //REQUIRE(doelSnelheid >= 0.0, "doelSnelheid mag niet negatief zijn.");

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

/**
 * @brief Voer een noodstop uit (voertuig komt tot stilstand)
 */
void Voertuig::noodStop() {
    REQUIRE(properlyInitialized(), "noodStop moet starten in een geldige toestand.");
    // Formule uit B.5
    versnelling = -getMaxRemFactor() * snelheid / getMaxSnelheid();
    ENSURE(versnelling <= 0, "Noodstop moet negatieve versnelling geven.");
}

/**
 * @brief Converteer een stringtype naar enum
 * @param typeStr Stringrepresentatie van het type
 * @return De overeenkomstige enum waarde
 */
Voertuig::VoertuigType Voertuig::stringToType(const std::string& typeStr) {
    if (typeStr == "auto") return VoertuigType::AUTO;
    if (typeStr == "bus") return VoertuigType::BUS;
    if (typeStr == "brandweerwagen") return VoertuigType::BRANDWEERWAGEN;
    if (typeStr == "ziekenwagen") return VoertuigType::ZIEKENWAGEN;
    if (typeStr == "politiecombi") return VoertuigType::POLITIECOMBI;

    // Standaard naar AUTO als type niet herkend wordt
    return VoertuigType::AUTO;
}

/**
 * @brief Converteer een enum type naar string
 * @param type De enum waarde
 * @return Stringrepresentatie van het type
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
 * @brief Haal de parameters op voor een specifiek voertuigtype
 * @param type Het voertuigtype
 * @return De parameters voor het gegeven type
 */
Voertuig::VoertuigParams Voertuig::getVoertuigParams(VoertuigType type) {
    return typeParameters.at(type);
}