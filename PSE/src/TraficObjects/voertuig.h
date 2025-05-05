/**
 * @file voertuig.h
 * @brief Header voor de Voertuig klasse (Herzien met voertuigtypes)
 */

#ifndef VOERTUIG_H
#define VOERTUIG_H

#include <string>
#include <map>

/**
 * @brief Klasse die een voertuig in een verkeerssituatie voorstelt
 */
class Voertuig {
public:
    /**
     * @brief Enumeratie voor verschillende voertuigtypes
     */
    enum class VoertuigType {
        AUTO,
        BUS,
        BRANDWEERWAGEN,
        ZIEKENWAGEN,
        POLITIECOMBI
    };

    /**
     * @brief Structuur met voertuigparameters op basis van type
     */
    struct VoertuigParams {
        double lengte;            ///< Voertuiglengte in meters
        double maxSnelheid;       ///< Max snelheid in m/s
        double maxVersnelling;    ///< Max versnelling in m/s²
        double maxRemFactor;      ///< Max remfactor in m/s²
        double minVolgafstand;    ///< Minimale volgafstand in meters
        bool isPrioriteitsvoertuig; ///< Of dit een prioriteitsvoertuig is

        VoertuigParams(double l, double v, double a, double b, double f, bool p = false)
            : lengte(l), maxSnelheid(v), maxVersnelling(a), maxRemFactor(b), minVolgafstand(f), isPrioriteitsvoertuig(p) {}
    };

    /**
     * @brief Constructor
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param type Het type van het voertuig (auto, bus, brandweerwagen, ziekenwagen, politiecombi)
     */
    Voertuig(const std::string& baan, double positie, const std::string& type = "auto");

    /**
     * @brief Constructor met snelheid en versnelling
     * @param baan De naam van de baan waar het voertuig zich bevindt
     * @param positie De positie van het voertuig op de baan
     * @param snelheid De snelheid van het voertuig
     * @param versnelling De versnelling van het voertuig
     * @param type Het type van het voertuig (auto, bus, brandweerwagen, ziekenwagen, politiecombi)
     */
    Voertuig(const std::string& baan, double positie, double snelheid, double versnelling, const std::string& type = "auto");

    /**
     * @brief Copy constructor
     * @param other Het te kopiëren voertuig
     */
    Voertuig(const Voertuig& other);

    /**
     * @brief Destructor
     */
    ~Voertuig();

    /**
     * @brief Assignment operator
     * @param other Het voertuig waarvan de waarden worden overgenomen
     * @return Referentie naar dit voertuig
     */
    Voertuig& operator=(const Voertuig& other);

    /**
     * @brief Geeft de naam van de baan terug
     * @return De naam van de baan
     */
    std::string getBaanNaam() const;

    /**
     * @brief Alias voor getBaanNaam voor compatibiliteit
     * @return De naam van de baan
     */
    std::string getBaan() const;

    /**
     * @brief Stelt de naam van de baan in
     * @param nieuweNaam De nieuwe baannaam
     */
    void setBaanNaam(const std::string& nieuweNaam);

    /**
     * @brief Geeft de positie van het voertuig terug
     * @return De positie op de baan
     */
    double getPositie() const;

    /**
     * @brief Zet de positie van het voertuig
     * @param nieuwePositie De nieuwe positie
     */
    void setPositie(double nieuwePositie);

    /**
     * @brief Geeft het type van het voertuig terug
     * @return Het type van het voertuig
     */
    std::string getType() const;

    /**
     * @brief Haalt het voertuigtype op als enum
     * @return Het voertuigtype als enum
     */
    VoertuigType getTypeEnum() const;

    /**
     * @brief Controleert of het voertuig een prioriteitsvoertuig is
     * @return True als het voertuig een prioriteitsvoertuig is (brandweerwagen, ziekenwagen, politie)
     */
    bool isPrioriteitsvoertuig() const;

    /**
     * @brief Controleert of dit voertuig een bus is
     * @return True als het voertuig een bus is
     */
    bool isBus() const;

    /**
     * @brief Geeft de snelheid van het voertuig terug
     * @return De snelheid in m/s
     */
    double getSnelheid() const;

    /**
     * @brief Zet de snelheid van het voertuig
     * @param nieuweSnelheid De nieuwe snelheid
     */
    void setSnelheid(double nieuweSnelheid);

    /**
     * @brief Geeft de versnelling van het voertuig terug
     * @return De versnelling in m/s²
     */
    double getVersnelling() const;

    /**
     * @brief Zet de versnelling van het voertuig
     * @param nieuweVersnelling De nieuwe versnelling
     */
    void setVersnelling(double nieuweVersnelling);

    /**
     * @brief Haalt de lengte van het voertuig op
     * @return De lengte in meters
     */
    double getLengte() const;

    /**
     * @brief Haalt de maximale snelheid van het voertuig op
     * @return De maximale snelheid in m/s
     */
    double getMaxSnelheid() const;

    /**
     * @brief Haalt de maximale versnelling van het voertuig op
     * @return De maximale versnelling in m/s²
     */
    double getMaxVersnelling() const;

    /**
     * @brief Haalt de maximale remfactor van het voertuig op
     * @return De maximale remfactor in m/s²
     */
    double getMaxRemFactor() const;

    /**
     * @brief Haalt de minimale volgafstand op
     * @return De minimale volgafstand in meters
     */
    double getMinVolgafstand() const;

    /**
     * @brief Stelt de bus wachtend-vlag in
     * @param isWaiting Of de bus wacht bij een halte
     */
    void setIsWaitingAtBusStop(bool isWaiting);

    /**
     * @brief Controleert of de bus wacht bij een halte
     * @return True als de bus wacht, false indien niet
     */
    bool isWaitingAtBusStop() const;

    /**
     * @brief Update de positie en snelheid van het voertuig op basis van de huidige versnelling
     * @param tijdstap De tijdstap voor de update in seconden
     */
    void updatePositieEnSnelheid(double tijdstap);

    /**
     * @brief Bereken de versnelling van het voertuig op basis van voorafgaand voertuig en andere factoren
     * @param voorgaandVoertuig Het voorafgaande voertuig, nullptr indien geen
     * @param isEersteVoertuig Of dit het eerste voertuig op de weg is
     * @param verkeersLichtVertraagFactor Vertraagfactor voor een verkeerslicht (0.4 standaard)
     * @param doelSnelheid Doelsnelheid, gebruikt voertuig's maximale snelheid standaard
     */
    void berekenVersnelling(const Voertuig* voorgaandVoertuig, bool isEersteVoertuig,
                            double verkeersLichtVertraagFactor = 0.4,
                            double doelSnelheid = -1.0);

    /**
     * @brief Pas noodstoppen toe (voertuig komt tot stilstand)
     */
    void noodStop();

    /**
     * @brief Converteer een string type naar enum
     * @param typeStr String-weergave van het type
     * @return De corresponderende enumwaarde
     */
    static VoertuigType stringToType(const std::string& typeStr);

    /**
     * @brief Converteer een enum type naar string
     * @param type De enumwaarde
     * @return String-weergave van het type
     */
    static std::string typeToString(VoertuigType type);

    /**
     * @brief Haal de parameters op voor een specifiek voertuigtype
     * @param type Het voertuigtype
     * @return De parameters voor het gegeven type
     */
    static VoertuigParams getVoertuigParams(VoertuigType type);

    bool properlyInitialized() const;



private:
    std::string baanNaam; ///< Naam van de baan waar het voertuig zich bevindt
    double positie;       ///< Positie van het voertuig op de baan
    double snelheid;      ///< Snelheid van het voertuig in m/s
    double versnelling;   ///< Versnelling van het voertuig in m/s²
    VoertuigType type;    ///< Type van het voertuig
    bool isWaitingAtStop; ///< Of de bus wacht bij een halte

    Voertuig* _initCheck;

    // Standaard parameters per voertuigtype (volgens bijlage C)
    static const std::map<VoertuigType, VoertuigParams> typeParameters;
};

#endif // VOERTUIG_H