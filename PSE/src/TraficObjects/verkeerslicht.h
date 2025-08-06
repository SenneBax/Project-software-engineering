#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H

#include <string>

/**
 * @class Verkeerslicht
 * @brief Klasse die een verkeerslicht in de verkeerssimulatie voorstelt
 *
 * Een Verkeerslicht regelt het verkeer op een specifieke positie op een baan.
 * Het kan verschillende kleuren hebben (groen, oranje, rood) en schakelt
 * automatisch volgens een cyclustijd. Slimme verkeerslichten kunnen hun
 * gedrag aanpassen op basis van wachtende voertuigen.
 *
 * Eigenschappen:
 * - Normale verkeerslichten: vaste cyclustijd
 * - Slimme verkeerslichten: adaptieve cyclustijd gebaseerd op verkeer
 * - Optionele oranje fase tussen groen en rood
 * - Registratie van wachtende voertuigen voor slimme lichten
 */
class Verkeerslicht {
public:
    /**
     * @brief Enum voor de verkeerslicht kleuren
     *
     * Definieert de drie mogelijke kleuren van een verkeerslicht:
     * - GROEN: Voertuigen mogen doorrijden
     * - ORANJE: Voertuigen moeten stoppen tenzij dit onveilig is
     * - ROOD: Voertuigen moeten stoppen
     */
    enum class Kleur {
        GROEN,   /**< Groen licht - doorrijden toegestaan */
        ORANJE,  /**< Oranje licht - voorzichtig stoppen */
        ROOD     /**< Rood licht - stoppen verplicht */
    };

private:
    std::string baan;         /**< Naam van de baan waar het verkeerslicht zich bevindt */
    double positie;           /**< Positie van het verkeerslicht op de baan in meters */
    int cyclus;               /**< Cyclustijd van het verkeerslicht in seconden */
    double tijdSindsLaatsteWissel; /**< Tijd sinds de laatste kleurverandering in seconden */
    Kleur huidigeKleur;       /**< Huidige kleur van het verkeerslicht */
    bool heeftOranje;         /**< Of dit verkeerslicht een oranje status heeft */
    bool isSlim;              /**< Of dit een slim verkeerslicht is */
    int voertuigenVoorLicht;  /**< Aantal voertuigen wachtend voor het licht (alleen slim) */
    int prioriteitsVoertuigenVoorLicht; /**< Aantal prioriteitsvoertuigen wachtend (alleen slim) */
    Verkeerslicht* _initCheck; /**< Pointer voor Design by Contract verificatie */

public:
    /**
     * @brief Constructor voor het verkeerslicht
     * @param baan Naam van de baan waar het licht staat
     * @param positie Positie op de baan in meters
     * @param cyclus Cyclustijd in seconden
     * @param heeftOranje Of dit verkeerslicht een oranje status heeft (default false)
     * @param isSlim Of dit een slim verkeerslicht is (default false)
     * @pre !baan.empty()
     * @pre positie >= 0.0
     * @pre cyclus > 0
     * @post properlyInitialized() == true
     * @post getBaan() == baan
     * @post getPositie() == positie
     * @post getCyclus() == cyclus
     * @post getHeeftOranje() == heeftOranje
     * @post getIsSlim() == isSlim
     * @post getKleur() == Kleur::ROOD (startkleur)
     * @post getVoertuigenVoorLicht() == 0
     *
     * Maakt een nieuw verkeerslicht aan op de opgegeven positie.
     * Het licht start altijd in rode toestand. Voor slimme lichten
     * kunnen voertuigen geregistreerd worden die voor het licht wachten.
     */
    Verkeerslicht(const std::string& baan, double positie, int cyclus, bool heeftOranje = false, bool isSlim = false);

    /**
     * @brief Copy constructor
     * @param other Het te kopiëren verkeerslicht
     * @pre other.properlyInitialized() == true
     * @post properlyInitialized() == true
     */
    Verkeerslicht(const Verkeerslicht& other);

    /**
     * @brief Assignment operator
     * @param other Het toe te wijzen verkeerslicht
     * @return Referentie naar dit object
     * @pre properlyInitialized() == true
     * @pre other.properlyInitialized() == true
     * @post properlyInitialized() == true
     */
    Verkeerslicht& operator=(const Verkeerslicht& other);

    /**
     * @brief Destructor
     */
    ~Verkeerslicht();

    /**
     * @brief Controleer of het object correct is geïnitialiseerd
     * @return true als het object correct is geïnitialiseerd, false anders
     * @post return waarde geeft aan of object in geldige toestand verkeert
     *
     * Design by Contract verificatie methode om object integriteit
     * te controleren.
     */
    bool properlyInitialized() const;

    /**
     * @brief Getter voor de baannaam van het verkeerslicht
     * @return De naam van de baan als string
     * @pre properlyInitialized() == true
     * @post !return waarde.empty()
     * @post return waarde is de baan waar dit licht staat
     *
     * Haalt de naam op van de baan waarop dit verkeerslicht staat.
     */
    std::string getBaan() const;

    /**
     * @brief Getter voor de positie van het verkeerslicht
     * @return De positie in meters vanaf begin van de baan
     * @pre properlyInitialized() == true
     * @post return waarde >= 0.0
     * @post return waarde is positie op de baan
     *
     * Haalt de positie op waar dit verkeerslicht staat op de baan.
     */
    double getPositie() const;

    /**
     * @brief Getter voor de cyclustijd van het verkeerslicht
     * @return De cyclustijd in seconden
     * @pre properlyInitialized() == true
     * @post return waarde > 0
     * @post return waarde is de ingestelde cyclustijd
     *
     * Haalt de cyclustijd op waarmee het verkeerslicht schakelt.
     * Voor normale lichten is dit een vaste tijd, voor slimme lichten
     * wordt deze gebruikt als basis maar kan variëren.
     */
    int getCyclus() const;

    /**
     * @brief Getter voor de huidige kleur van het verkeerslicht
     * @return De huidige kleur (GROEN, ORANJE, of ROOD)
     * @pre properlyInitialized() == true
     * @post return waarde is een geldige Kleur enum waarde
     *
     * Haalt de huidige kleur op van het verkeerslicht.
     */
    Kleur getKleur() const;

    /**
     * @brief Setter voor de huidige kleur van het verkeerslicht
     * @param kleur De nieuwe kleur om in te stellen
     * @pre properlyInitialized() == true
     * @post getKleur() == kleur
     * @post tijdSindsLaatsteWissel is gereset naar 0.0
     *
     * Stelt een nieuwe kleur in voor het verkeerslicht en reset
     * de timer voor de laatste kleurverandering.
     */
    void setKleur(Kleur kleur);

    /**
     * @brief Haal kleur op als string representatie
     * @return String-weergave van de huidige kleur
     * @pre properlyInitialized() == true
     * @post return waarde is "groen", "oranje", "rood", of "onbekend"
     * @post return waarde komt overeen met getKleur()
     *
     * Converteert de huidige kleur naar een leesbare string.
     */
    std::string getKleurAsString() const;

    /**
     * @brief Controleert of het verkeerslicht rood is
     * @return true als het licht rood is, false anders
     * @pre properlyInitialized() == true
     * @post return waarde == (getKleur() == Kleur::ROOD)
     *
     * Hulpmethode om te controleren of het licht rood staat.
     */
    bool isRood() const;

    /**
     * @brief Controleert of het verkeerslicht oranje is
     * @return true als het licht oranje is, false anders
     * @pre properlyInitialized() == true
     * @post return waarde == (getKleur() == Kleur::ORANJE)
     *
     * Hulpmethode om te controleren of het licht oranje staat.
     */
    bool isOranje() const;

    /**
     * @brief Controleert of het verkeerslicht groen is
     * @return true als het licht groen is, false anders
     * @pre properlyInitialized() == true
     * @post return waarde == (getKleur() == Kleur::GROEN)
     *
     * Hulpmethode om te controleren of het licht groen staat.
     */
    bool isGroen() const;

    /**
     * @brief Controleert of dit verkeerslicht een oranje status heeft
     * @return true als het licht een oranje status heeft, false anders
     * @pre properlyInitialized() == true
     * @post return waarde is configuratie van constructor
     *
     * Geeft aan of dit verkeerslicht een oranje fase heeft tussen
     * groen en rood. Zonder oranje schakelt het direct van groen naar rood.
     */
    bool getHeeftOranje() const;

    /**
     * @brief Controleert of dit een slim verkeerslicht is
     * @return true als het licht slim is, false anders
     * @pre properlyInitialized() == true
     * @post return waarde is configuratie van constructor
     *
     * Geeft aan of dit een slim verkeerslicht is dat kan reageren
     * op wachtende voertuigen door zijn cyclustijd aan te passen.
     */
    bool getIsSlim() const;

    /**
     * @brief Update het verkeerslicht op basis van verstreken tijd
     * @param tijdstap De verstreken tijd sinds laatste update in seconden
     * @pre properlyInitialized() == true
     * @pre tijdstap >= 0.0
     * @post tijdSindsLaatsteWissel is verhoogd met tijdstap
     * @post Kleur kan zijn veranderd volgens licht logica
     * @post Voor slimme lichten: kleur reageert op voertuigenVoorLicht
     *
     * Update de status van het verkeerslicht. Voor normale lichten
     * schakelt het volgens vaste cyclustijd. Voor slimme lichten
     * wordt ook rekening gehouden met wachtende voertuigen.
     *
     * Normale logica:
     * - Met oranje: Groen -> Oranje (10% cyclus) -> Rood -> Groen
     * - Zonder oranje: Groen -> Rood -> Groen
     *
     * Slimme logica:
     * - Minimaal 10s, maximaal 60s per fase
     * - Schakelt naar groen bij wachtende voertuigen
     * - Schakelt naar rood bij geen verkeer na 20s groen
     */
    void update(double tijdstap);

    /**
     * @brief Registreer een voertuig wachtend voor dit licht (voor slimme verkeerslichten)
     * @pre properlyInitialized() == true
     * @post Als isSlim(): getVoertuigenVoorLicht() is verhoogd met 1
     * @post Als !isSlim(): geen effect
     *
     * Registreert een voertuig dat voor dit slimme verkeerslicht wacht.
     * Alleen van toepassing op slimme verkeerslichten.
     */
    void registerVoertuigVoorLicht();

    /**
     * @brief Registreer een prioriteitsvoertuig wachtend voor dit licht (voor slimme verkeerslichten)
     * @param isPrioriteit true als het een prioriteitsvoertuig is (brandweer, ziekenwagen, politie)
     * @pre properlyInitialized() == true
     * @post Als isSlim() && isPrioriteit: prioriteitsVoertuigenVoorLicht is verhoogd met 1
     * @post Als isSlim() && !isPrioriteit: voertuigenVoorLicht is verhoogd met 1
     * @post Als !isSlim(): geen effect
     *
     * Registreert een voertuig dat voor dit slimme verkeerslicht wacht.
     * Prioriteitsvoertuigen worden apart geteld en krijgen voorrang.
     */
    void registerVoertuigVoorLicht(bool isPrioriteit);

    /**
     * @brief Deregistreer een voertuig dat aan dit licht wachtte (voor slimme verkeerslichten)
     * @pre properlyInitialized() == true
     * @post Als isSlim() && voertuigenVoorLicht > 0: getVoertuigenVoorLicht() is verminderd met 1
     * @post Als !isSlim(): geen effect
     * @post getVoertuigenVoorLicht() >= 0
     *
     * Deregistreert een voertuig dat niet meer wacht voor dit licht.
     * Voorkomt dat de teller negatief wordt.
     */
    void unregisterVoertuigVoorLicht();

    /**
     * @brief Deregistreer een prioriteitsvoertuig dat aan dit licht wachtte (voor slimme verkeerslichten)
     * @param isPrioriteit true als het een prioriteitsvoertuig is
     * @pre properlyInitialized() == true
     * @post Als isSlim() && isPrioriteit && prioriteitsVoertuigenVoorLicht > 0: prioriteitsVoertuigenVoorLicht verminderd met 1
     * @post Als isSlim() && !isPrioriteit && voertuigenVoorLicht > 0: voertuigenVoorLicht verminderd met 1
     * @post prioriteitsVoertuigenVoorLicht >= 0 && voertuigenVoorLicht >= 0
     *
     * Deregistreert een voertuig dat niet meer wacht voor dit licht.
     * Houdt rekening met type voertuig (prioriteit of normaal).
     */
    void unregisterVoertuigVoorLicht(bool isPrioriteit);

    /**
     * @brief Haal het aantal voertuigen op dat voor dit licht wacht
     * @return Het aantal geregistreerde wachtende voertuigen
     * @pre properlyInitialized() == true
     * @post return waarde >= 0
     * @post Voor slimme lichten: actueel aantal wachtende voertuigen
     * @post Voor normale lichten: altijd 0
     *
     * Geeft het aantal voertuigen dat geregistreerd staat als wachtend
     * voor dit verkeerslicht. Alleen relevant voor slimme lichten.
     */
    int getVoertuigenVoorLicht() const;

    /**
     * @brief Haal het aantal prioriteitsvoertuigen op dat voor dit licht wacht
     * @return Het aantal geregistreerde wachtende prioriteitsvoertuigen
     * @pre properlyInitialized() == true
     * @post return waarde >= 0
     * @post Voor slimme lichten: actueel aantal wachtende prioriteitsvoertuigen
     * @post Voor normale lichten: altijd 0
     *
     * Geeft het aantal prioriteitsvoertuigen dat geregistreerd staat als wachtend
     * voor dit verkeerslicht. Alleen relevant voor slimme lichten.
     */
    int getPrioriteitsVoertuigenVoorLicht() const;

    /**
     * @brief Haal totaal aantal voertuigen op dat voor dit licht wacht
     * @return Het totaal aantal (normale + prioriteits) wachtende voertuigen
     * @pre properlyInitialized() == true
     * @post return waarde >= 0
     * @post return waarde == getVoertuigenVoorLicht() + getPrioriteitsVoertuigenVoorLicht()
     *
     * Geeft het totaal aantal voertuigen (normale + prioriteits) dat wacht.
     */
    int getTotaalVoertuigenVoorLicht() const;

    /**
     * @brief Reset het aantal voertuigen dat voor dit licht wacht
     * @pre properlyInitialized() == true
     * @post getVoertuigenVoorLicht() == 0
     * @post getPrioriteitsVoertuigenVoorLicht() == 0
     *
     * Reset beide tellers van wachtende voertuigen naar nul.
     * Wordt typisch gebruikt wanneer het licht groen wordt.
     */
    void resetVoertuigenVoorLicht();
};

#endif // VERKEERSLICHT_H