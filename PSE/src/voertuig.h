/**
 * @file voertuig.h
 * @brief Header for the Voertuig class (Revised with vehicle types)
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
     * @brief Enumeration for different vehicle types
     */
    enum class VoertuigType {
        AUTO,
        BUS,
        BRANDWEERWAGEN,
        ZIEKENWAGEN,
        POLITIECOMBI
    };

    /**
     * @brief Structure containing vehicle parameters based on type
     */
    struct VoertuigParams {
        double lengte;            ///< Vehicle length in meters
        double maxSnelheid;       ///< Max speed in m/s
        double maxVersnelling;    ///< Max acceleration in m/s²
        double maxRemFactor;      ///< Max braking factor in m/s²
        double minVolgafstand;    ///< Minimum following distance in meters
        bool isPrioriteitsvoertuig; ///< Whether this is a priority vehicle

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
     * @brief Sets the name of the road
     * @param nieuweNaam The new road name
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
     * @brief Gets the vehicle type as enum
     * @return The vehicle type enum
     */
    VoertuigType getTypeEnum() const;

    /**
     * @brief Checks if the vehicle is a priority vehicle
     * @return True if the vehicle is a priority vehicle (fire truck, ambulance, police)
     */
    bool isPrioriteitsvoertuig() const;

    /**
     * @brief Checks if this vehicle is a bus
     * @return True if the vehicle is a bus
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
     * @brief Gets the length of the vehicle
     * @return The length in meters
     */
    double getLengte() const;

    /**
     * @brief Gets the maximum speed of the vehicle
     * @return The maximum speed in m/s
     */
    double getMaxSnelheid() const;

    /**
     * @brief Gets the maximum acceleration of the vehicle
     * @return The maximum acceleration in m/s²
     */
    double getMaxVersnelling() const;

    /**
     * @brief Gets the maximum braking factor of the vehicle
     * @return The maximum braking factor in m/s²
     */
    double getMaxRemFactor() const;

    /**
     * @brief Gets the minimum following distance
     * @return The minimum following distance in meters
     */
    double getMinVolgafstand() const;

    /**
     * @brief Sets the bus waiting flag
     * @param isWaiting Whether the bus is waiting at a stop
     */
    void setIsWaitingAtBusStop(bool isWaiting);

    /**
     * @brief Checks if the bus is waiting at a stop
     * @return True if the bus is waiting, false otherwise
     */
    bool isWaitingAtBusStop() const;

    /**
     * @brief Update the vehicle's position and speed based on the current acceleration
     * @param tijdstap The time step for the update in seconds
     */
    void updatePositieEnSnelheid(double tijdstap);

    /**
     * @brief Calculate the vehicle's acceleration based on preceding vehicle and other factors
     * @param voorgaandVoertuig The preceding vehicle, nullptr if none
     * @param isEersteVoertuig Whether this is the first vehicle on the road
     * @param verkeersLichtVertraagFactor Slowdown factor for a traffic light (0.4 by default)
     * @param doelSnelheid Target speed, uses vehicle's maximum speed by default
     */
    void berekenVersnelling(const Voertuig* voorgaandVoertuig, bool isEersteVoertuig,
                            double verkeersLichtVertraagFactor = 0.4,
                            double doelSnelheid = -1.0);

    /**
     * @brief Apply emergency braking (vehicle comes to a stop)
     */
    void noodStop();

    /**
     * @brief Convert a string type to enum
     * @param typeStr String representation of the type
     * @return The corresponding enum value
     */
    static VoertuigType stringToType(const std::string& typeStr);

    /**
     * @brief Convert an enum type to string
     * @param type The enum value
     * @return String representation of the type
     */
    static std::string typeToString(VoertuigType type);

    /**
     * @brief Get the parameters for a specific vehicle type
     * @param type The vehicle type
     * @return The parameters for the given type
     */
    static VoertuigParams getVoertuigParams(VoertuigType type);

private:
    std::string baanNaam; ///< Naam van de baan waar het voertuig zich bevindt
    double positie;       ///< Positie van het voertuig op de baan
    double snelheid;      ///< Snelheid van het voertuig in m/s
    double versnelling;   ///< Versnelling van het voertuig in m/s²
    VoertuigType type;    ///< Type van het voertuig
    bool isWaitingAtStop; ///< Whether the bus is waiting at a stop

    // Standard parameters per vehicle type (according to appendix C)
    static const std::map<VoertuigType, VoertuigParams> typeParameters;
};

#endif // VOERTUIG_H