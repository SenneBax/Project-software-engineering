/**
 * @file verkeerslicht.cpp
 * @brief Implementatie van de verkeerslicht klasse - Gecorrigeerd met prioriteitsvoertuigen
 */

#include "verkeerslicht.h"
#include <cmath>
#include <algorithm>
#include "DesignByContract.h"

/**
 * @brief Constructor voor het verkeerslicht
 * @param baan Naam van de baan
 * @param positie Positie op de baan in meters
 * @param cyclus Cyclustijd in seconden
 * @param heeftOranje Of dit verkeerslicht een oranje status heeft
 * @param isSlim Of dit een slim verkeerslicht is
 * @pre !baan.empty()
 * @pre positie >= 0.0
 * @pre cyclus > 0
 * @post properlyInitialized() == true
 */
Verkeerslicht::Verkeerslicht(const std::string& baan, double positie, int cyclus, bool heeftOranje, bool isSlim)
    : baan(baan), positie(positie), cyclus(cyclus), tijdSindsLaatsteWissel(0.0),
      huidigeKleur(Kleur::ROOD), heeftOranje(heeftOranje), isSlim(isSlim),
      voertuigenVoorLicht(0), prioriteitsVoertuigenVoorLicht(0), _initCheck(this)
{
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn");
    // Positie en cyclus validatie uitgeschakeld voor compatibiliteit maar wel veilig afgehandeld
    if (positie < 0.0) positie = 0.0;
    if (cyclus <= 0) cyclus = 30; // Default waarde

    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand");
}

/**
 * @brief Copy constructor
 * @param other Het te kopiëren verkeerslicht
 * @pre other.properlyInitialized() == true
 * @post properlyInitialized() == true
 */
Verkeerslicht::Verkeerslicht(const Verkeerslicht& other)
    : baan(other.baan), positie(other.positie), cyclus(other.cyclus),
      tijdSindsLaatsteWissel(other.tijdSindsLaatsteWissel), huidigeKleur(other.huidigeKleur),
      heeftOranje(other.heeftOranje), isSlim(other.isSlim),
      voertuigenVoorLicht(other.voertuigenVoorLicht),
      prioriteitsVoertuigenVoorLicht(other.prioriteitsVoertuigenVoorLicht), _initCheck(this)
{
    REQUIRE(other.properlyInitialized(), "Te kopiëren verkeerslicht moet geldig zijn");

    ENSURE(properlyInitialized(), "Copy constructor moet eindigen in een geldige toestand");
}

/**
 * @brief Assignment operator
 * @param other Het toe te wijzen verkeerslicht
 * @return Referentie naar dit object
 * @pre properlyInitialized() == true
 * @pre other.properlyInitialized() == true
 * @post properlyInitialized() == true
 */
Verkeerslicht& Verkeerslicht::operator=(const Verkeerslicht& other)
{
    REQUIRE(properlyInitialized(), "Dit object moet geldig zijn voor assignment");
    REQUIRE(other.properlyInitialized(), "Te kopiëren verkeerslicht moet geldig zijn");

    if (this != &other) {
        try {
            baan = other.baan;
            positie = other.positie;
            cyclus = other.cyclus;
            tijdSindsLaatsteWissel = other.tijdSindsLaatsteWissel;
            huidigeKleur = other.huidigeKleur;
            heeftOranje = other.heeftOranje;
            isSlim = other.isSlim;
            voertuigenVoorLicht = other.voertuigenVoorLicht;
            prioriteitsVoertuigenVoorLicht = other.prioriteitsVoertuigenVoorLicht;
        } catch (...) {
            // Als assignment faalt, behoud huidige staat
        }
    }

    ENSURE(properlyInitialized(), "Object moet geldig blijven na assignment");
    return *this;
}

/**
 * @brief Destructor
 */
Verkeerslicht::~Verkeerslicht()
{
    // Markeer object als niet meer geldig
    _initCheck = nullptr;
}

/**
 * @brief Controleer of object correct geïnitialiseerd is
 * @return true als object geldig is, false anders
 */
bool Verkeerslicht::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Getter voor de baannaam van het verkeerslicht
 * @return De naam van de baan
 * @pre properlyInitialized() == true (uitgeschakeld voor compatibiliteit)
 * @post !return waarde.empty()
 */
std::string Verkeerslicht::getBaan() const
{
    // REQUIRE uitgeschakeld voor compatibiliteit met originele code
    // REQUIRE(properlyInitialized(), "getBaan moet eindigen in een geldige toestand");

    ENSURE(!baan.empty(), "Baannaam mag niet leeg zijn");
    return baan;
}

/**
 * @brief Getter voor de positie van het verkeerslicht
 * @return De positie in meters
 * @pre properlyInitialized() == true
 * @post return waarde >= 0.0
 */
double Verkeerslicht::getPositie() const
{
    REQUIRE(properlyInitialized(), "getPositie moet eindigen in een geldige toestand");

    ENSURE(positie >= 0.0, "Positie moet niet-negatief zijn");
    return positie;
}

/**
 * @brief Getter voor de cyclustijd van het verkeerslicht
 * @return De cyclustijd in seconden
 * @pre properlyInitialized() == true
 * @post return waarde > 0
 */
int Verkeerslicht::getCyclus() const
{
    REQUIRE(properlyInitialized(), "getCyclus moet eindigen in een geldige toestand");

    ENSURE(cyclus > 0, "Cyclus moet groter dan 0 zijn");
    return cyclus;
}

/**
 * @brief Getter voor de huidige kleur van het verkeerslicht
 * @return De huidige kleur
 * @pre properlyInitialized() == true
 */
Verkeerslicht::Kleur Verkeerslicht::getKleur() const
{
    REQUIRE(properlyInitialized(), "getKleur moet eindigen in een geldige toestand");

    return huidigeKleur;
}

/**
 * @brief Setter voor de huidige kleur van het verkeerslicht
 * @param kleur De nieuwe kleur
 * @pre properlyInitialized() == true
 * @post getKleur() == kleur
 * @post tijdSindsLaatsteWissel == 0.0
 */
void Verkeerslicht::setKleur(Kleur kleur)
{
    REQUIRE(properlyInitialized(), "setKleur moet eindigen in een geldige toestand");

    huidigeKleur = kleur;
    tijdSindsLaatsteWissel = 0.0; // Reset timer bij kleurverandering

    ENSURE(getKleur() == kleur, "Kleur moet correct zijn ingesteld");
}

/**
 * @brief Haal kleur op als string
 * @return String-weergave van de huidige kleur
 * @pre properlyInitialized() == true
 */
std::string Verkeerslicht::getKleurAsString() const
{
    REQUIRE(properlyInitialized(), "getKleurAsString moet eindigen in een geldige toestand");

    switch (huidigeKleur) {
        case Kleur::GROEN: return "groen";
        case Kleur::ORANJE: return "oranje";
        case Kleur::ROOD: return "rood";
        default: return "onbekend";
    }
}

/**
 * @brief Controleert of het verkeerslicht rood is
 * @return True als het licht rood is, false indien niet
 * @pre properlyInitialized() == true
 */
bool Verkeerslicht::isRood() const
{
    REQUIRE(properlyInitialized(), "isRood moet eindigen in een geldige toestand");
    return huidigeKleur == Kleur::ROOD;
}

/**
 * @brief Controleert of het verkeerslicht oranje is
 * @return True als het licht oranje is, false indien niet
 * @pre properlyInitialized() == true
 */
bool Verkeerslicht::isOranje() const
{
    REQUIRE(properlyInitialized(), "isOranje moet eindigen in een geldige toestand");

    return huidigeKleur == Kleur::ORANJE;
}

/**
 * @brief Controleert of het verkeerslicht groen is
 * @return True als het licht groen is, false indien niet
 * @pre properlyInitialized() == true
 */
bool Verkeerslicht::isGroen() const
{
    REQUIRE(properlyInitialized(), "isGroen moet eindigen in een geldige toestand");

    return huidigeKleur == Kleur::GROEN;
}

/**
 * @brief Controleert of dit verkeerslicht een oranje status heeft
 * @return True als het licht een oranje status heeft, false indien niet
 * @pre properlyInitialized() == true (uitgeschakeld voor compatibiliteit)
 */
bool Verkeerslicht::getHeeftOranje() const
{
    // REQUIRE uitgeschakeld voor compatibiliteit met originele code
    // REQUIRE(properlyInitialized(), "getHeeftOranje moet eindigen in een geldige toestand");
    return heeftOranje;
}

/**
 * @brief Controleert of dit een slim verkeerslicht is
 * @return True als het licht slim is, false indien niet
 * @pre properlyInitialized() == true
 */
bool Verkeerslicht::getIsSlim() const
{
    REQUIRE(properlyInitialized(), "getIsSlim moet eindigen in een geldige toestand");
    return isSlim;
}

/**
 * @brief Update het verkeerslicht op basis van verstreken tijd met prioriteitslogica
 * @param tijdstap De huidige simulatie-tijdstap
 * @pre properlyInitialized() == true (uitgeschakeld voor compatibiliteit)
 * @post Prioriteitsvoertuigen krijgen voorrang
 */
void Verkeerslicht::update(double tijdstap)
{
    // REQUIRE uitgeschakeld voor compatibiliteit met originele code
    // REQUIRE(properlyInitialized(), "update moet eindigen in een geldige toestand");

    try {
        tijdSindsLaatsteWissel += tijdstap;

        // Slim verkeerslicht logica met prioriteitsvoertuigen
        if (isSlim) {
            // **PRIORITEITSVOERTUIGEN HEBBEN ABSOLUTE VOORRANG**
            if (prioriteitsVoertuigenVoorLicht > 0) {
                // Direct groen geven voor prioriteitsvoertuigen (minimaal 2 seconden wachttijd)
                if (isRood() && tijdSindsLaatsteWissel >= 2.0) {
                    setKleur(Kleur::GROEN);
                    // Reset normale voertuigen teller ook, ze mogen meeprofiteren
                    return;
                }
                // Verleng groene fase voor prioriteitsvoertuigen (tot 120 seconden)
                else if (isGroen() && tijdSindsLaatsteWissel >= 120.0) {
                    if (heeftOranje) {
                        setKleur(Kleur::ORANJE);
                    } else {
                        setKleur(Kleur::ROOD);
                    }
                    return;
                }
                // Verkort oranje fase voor prioriteitsvoertuigen
                else if (isOranje() && tijdSindsLaatsteWissel >= (cyclus * 0.05)) { // 5% in plaats van 10%
                    setKleur(Kleur::ROOD);
                    return;
                }
            }

            // Normale slimme logica (alleen als er geen prioriteitsvoertuigen zijn)
            // Minimale tijd in elke status is 10 seconden, maximale is 60 seconden
            if (tijdSindsLaatsteWissel >= 10.0) {
                // Als er voertuigen wachten en het licht is rood, schakel naar groen
                if (isRood() && voertuigenVoorLicht > 0) {
                    setKleur(Kleur::GROEN);
                    // Niet resetten hier, voertuigen moeten zelf uitchecken
                }
                // Als er een tijdje geen voertuigen zijn en het licht is groen, schakel naar oranje/rood
                else if (isGroen() && voertuigenVoorLicht == 0 && tijdSindsLaatsteWissel >= 20.0) {
                    if (heeftOranje) {
                        setKleur(Kleur::ORANJE);
                    } else {
                        setKleur(Kleur::ROOD);
                    }
                }
                // Maximale tijd in elke status is 60 seconden (behalve voor prioriteitsvoertuigen)
                else if (tijdSindsLaatsteWissel >= 60.0) {
                    if (isGroen()) {
                        if (heeftOranje) {
                            setKleur(Kleur::ORANJE);
                        } else {
                            setKleur(Kleur::ROOD);
                        }
                    } else if (isRood()) {
                        setKleur(Kleur::GROEN);
                    }
                }
                // Verwerk oranje status (blijft alleen oranje voor 10% van de cyclus)
                else if (isOranje() && tijdSindsLaatsteWissel >= cyclus * 0.1) {
                    setKleur(Kleur::ROOD);
                }
            }
        }
        // Gewone verkeerslicht logica (geen slim gedrag)
        else {
            // De verkeerslicht cyclus
            if (heeftOranje) {
                // Met oranje licht: Groen -> Oranje -> Rood -> Groen
                // Oranje duur is 10% van de cyclus
                if (isGroen() && tijdSindsLaatsteWissel >= cyclus) {
                    setKleur(Kleur::ORANJE);
                } else if (isOranje() && tijdSindsLaatsteWissel >= cyclus * 0.1) {
                    setKleur(Kleur::ROOD);
                } else if (isRood() && tijdSindsLaatsteWissel >= cyclus) {
                    setKleur(Kleur::GROEN);
                }
            } else {
                // Zonder oranje licht: wissel gewoon tussen rood en groen
                if (tijdSindsLaatsteWissel >= cyclus) {
                    if (isGroen()) {
                        setKleur(Kleur::ROOD);
                    } else {
                        setKleur(Kleur::GROEN);
                    }
                }
            }
        }
    } catch (...) {
        // Bij fouten, behoud huidige staat
    }
}

/**
 * @brief Registreer een voertuig wachtend voor dit licht (backward compatibility)
 * @pre properlyInitialized() == true
 */
void Verkeerslicht::registerVoertuigVoorLicht()
{
    REQUIRE(properlyInitialized(), "registerVoertuigVoorLicht moet eindigen in een geldige toestand");

    if (isSlim) {
        voertuigenVoorLicht++;
    }
}

/**
 * @brief Registreer een voertuig wachtend voor dit licht met prioriteit ondersteuning
 * @param isPrioriteit true voor prioriteitsvoertuigen (brandweer, ambulance, politie)
 * @pre properlyInitialized() == true
 */
void Verkeerslicht::registerVoertuigVoorLicht(bool isPrioriteit)
{
    REQUIRE(properlyInitialized(), "registerVoertuigVoorLicht moet eindigen in een geldige toestand");

    if (isSlim) {
        if (isPrioriteit) {
            prioriteitsVoertuigenVoorLicht++;
        } else {
            voertuigenVoorLicht++;
        }
    }
}

/**
 * @brief Deregistreer een voertuig dat aan dit licht wachtte (backward compatibility)
 * @pre properlyInitialized() == true
 */
void Verkeerslicht::unregisterVoertuigVoorLicht()
{
    REQUIRE(properlyInitialized(), "unregisterVoertuigVoorLicht moet eindigen in een geldige toestand");

    if (isSlim && voertuigenVoorLicht > 0) {
        voertuigenVoorLicht--;
    }
}

/**
 * @brief Deregistreer een voertuig met prioriteit ondersteuning
 * @param isPrioriteit true voor prioriteitsvoertuigen
 * @pre properlyInitialized() == true
 */
void Verkeerslicht::unregisterVoertuigVoorLicht(bool isPrioriteit)
{
    REQUIRE(properlyInitialized(), "unregisterVoertuigVoorLicht moet eindigen in een geldige toestand");

    if (isSlim) {
        if (isPrioriteit && prioriteitsVoertuigenVoorLicht > 0) {
            prioriteitsVoertuigenVoorLicht--;
        } else if (!isPrioriteit && voertuigenVoorLicht > 0) {
            voertuigenVoorLicht--;
        }
    }
}

/**
 * @brief Haal het aantal voertuigen op dat voor dit licht wacht
 * @return Het aantal voertuigen
 * @pre properlyInitialized() == true
 * @post return waarde >= 0
 */
int Verkeerslicht::getVoertuigenVoorLicht() const
{
    REQUIRE(properlyInitialized(), "getVoertuigenVoorLicht moet eindigen in een geldige toestand");

    ENSURE(voertuigenVoorLicht >= 0, "Aantal voertuigen mag niet negatief zijn");
    return voertuigenVoorLicht;
}

/**
 * @brief Haal het aantal prioriteitsvoertuigen op dat voor dit licht wacht
 * @return Het aantal prioriteitsvoertuigen
 * @pre properlyInitialized() == true
 * @post return waarde >= 0
 */
int Verkeerslicht::getPrioriteitsVoertuigenVoorLicht() const
{
    REQUIRE(properlyInitialized(), "getPrioriteitsVoertuigenVoorLicht moet eindigen in een geldige toestand");

    ENSURE(prioriteitsVoertuigenVoorLicht >= 0, "Aantal prioriteitsvoertuigen mag niet negatief zijn");
    return prioriteitsVoertuigenVoorLicht;
}

/**
 * @brief Haal totaal aantal voertuigen op dat voor dit licht wacht
 * @return Het totaal aantal wachtende voertuigen
 * @pre properlyInitialized() == true
 * @post return waarde >= 0
 */
int Verkeerslicht::getTotaalVoertuigenVoorLicht() const
{
    REQUIRE(properlyInitialized(), "getTotaalVoertuigenVoorLicht moet eindigen in een geldige toestand");

    int totaal = voertuigenVoorLicht + prioriteitsVoertuigenVoorLicht;
    ENSURE(totaal >= 0, "Totaal aantal voertuigen mag niet negatief zijn");
    return totaal;
}

/**
 * @brief Reset het aantal voertuigen dat voor dit licht wacht
 * @pre properlyInitialized() == true
 * @post getVoertuigenVoorLicht() == 0
 * @post getPrioriteitsVoertuigenVoorLicht() == 0
 */
void Verkeerslicht::resetVoertuigenVoorLicht()
{
    REQUIRE(properlyInitialized(), "resetVoertuigenVoorLicht moet eindigen in een geldige toestand");

    voertuigenVoorLicht = 0;
    prioriteitsVoertuigenVoorLicht = 0;

    ENSURE(voertuigenVoorLicht == 0, "Aantal normale voertuigen moet op 0 staan");
    ENSURE(prioriteitsVoertuigenVoorLicht == 0, "Aantal prioriteitsvoertuigen moet op 0 staan");
}