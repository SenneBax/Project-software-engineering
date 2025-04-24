/**
 * @file kruispunt.h
 * @brief Definition of the intersection class
 */

#ifndef KRUISPUNT_H
#define KRUISPUNT_H

#include <string>
#include <vector>
#include <utility>

/**
 * @class Kruispunt
 * @brief Class representing an intersection in the traffic simulation
 */
class Kruispunt {
private:
    struct BaanPositie {
        std::string naam;   /**< Name of the road */
        double positie;     /**< Position on the road */

        BaanPositie(const std::string& naam, double positie) : naam(naam), positie(positie) {}
    };

    std::vector<BaanPositie> banen;  /**< List of roads connected to this intersection */

public:
    /**
     * @brief Constructor for the intersection
     */
    Kruispunt();

    /**
     * @brief Add a road to the intersection
     * @param baanNaam Name of the road
     * @param positie Position on the road
     * @return True if the road was added successfully, false otherwise
     */
    bool voegBaanToe(const std::string& baanNaam, double positie);

    /**
     * @brief Get all the roads connected to this intersection
     * @return Vector of pairs with road names and positions
     */
    std::vector<std::pair<std::string, double>> getBanen() const;

    /**
     * @brief Check if a road is part of this intersection
     * @param baanNaam Name of the road
     * @return True if the road is part of this intersection, false otherwise
     */
    bool bevatBaan(const std::string& baanNaam) const;

    /**
     * @brief Get position on a specific road
     * @param baanNaam Name of the road
     * @return Position on the road, -1 if road is not found
     */
    double getPositieOpBaan(const std::string& baanNaam) const;

    /**
     * @brief Choose a random road to continue on from the intersection
     * @param huidigeWeg Current road name (to exclude it from possibilities)
     * @return Name of the chosen road, empty string if no valid road exists
     */
    std::string kiesRandomBaan(const std::string& huidigeWeg) const;
};

#endif // KRUISPUNT_H