/**
 * @file situatie.cpp
 * @brief Implementatie van de VerkeersSituatie klasse (Herzien met bushaltes en kruispunten en polymorfische voertuigen)
 */

#include "../Situation/situatie.h"
#include <algorithm>
#include <limits>
#include "DesignByContract.h"

/**
 * @brief Constructor
 */
VerkeersSituatie::VerkeersSituatie()
{
    _initCheck = this;
}

/**
 * @brief Destructor
 */
VerkeersSituatie::~VerkeersSituatie()
{
    // Resources worden automatisch opgeruimd door unique_ptrs
}

bool VerkeersSituatie::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Voeg een baan toe aan de verkeerssituatie
 * @param baan De toe te voegen baan
 * @return true als de baan succesvol werd toegevoegd, false indien niet
 */
bool VerkeersSituatie::voegBaanToe(const Baan& baan) {
    REQUIRE(properlyInitialized(), "voegBaanToe moet eindigen in een geldige toestand.");
    REQUIRE(!baan.getNaam().empty(), "Naam van toe te voegen baan mag niet leeg zijn.");

    std::string baanNaam = baan.getNaam();

    // Controleer of de baan al bestaat
    if (banen.find(baanNaam) != banen.end()) {
        return false;
    }

    banen[baanNaam] = baan;

    ENSURE(banen.find(baanNaam) != banen.end(), "Baan werd niet correct toegevoed.");
    return true;
}

/**
 * @brief Voeg een voertuig toe aan de verkeerssituatie
 * @param voertuig Het toe te voegen voertuig
 * @return true als het voertuig succesvol werd toegevoegd, false indien niet
 */
bool VerkeersSituatie::voegVoertuigToe(const Voertuig& voertuig) {
    REQUIRE(properlyInitialized(), "voegVoertuigToe moet eindigen in een geldige toestand.");
    std::string baanNaam = voertuig.getBaanNaam();

    // Controleer of de baan bestaat
    auto it = banen.find(baanNaam);

    if (it == banen.end()) {
        return false;
    }

    // Controleer of de positie geldig is
    if (voertuig.getPositie() < 0 || voertuig.getPositie() > it->second.getLengte()) {
        return false;
    }

    // Gebruik de clone-methode om een kopie van het voertuig toe te voegen
    try {
        auto nieuwVoertuig = voertuig.clone();
        voertuigen.push_back(std::move(nieuwVoertuig));
    }
    catch (const std::exception& e) {
        return false;
    }

    ENSURE(!voertuigen.empty(), "Voertuig werd niet correct toegevoed.");
    return true;
}

/**
 * @brief Voeg een voertuig toe aan de verkeerssituatie
 * @param voertuig Unieke pointer naar het toe te voegen voertuig
 * @return true als het voertuig succesvol werd toegevoegd, false indien niet
 */
bool VerkeersSituatie::voegVoertuigToe(std::unique_ptr<Voertuig> voertuig) {
    REQUIRE(properlyInitialized(), "voegVoertuigToe moet eindigen in een geldige toestand.");

    // Controleer of de baan bestaat
    std::string baanNaam = voertuig->getBaanNaam();
    auto it = banen.find(baanNaam);

    if (it == banen.end()) {
        return false;
    }

    // Controleer of de positie geldig is
    if (voertuig->getPositie() < 0 || voertuig->getPositie() > it->second.getLengte()) {
        return false;
    }

    // Voeg het voertuig toe aan de vector
    voertuigen.push_back(std::move(voertuig));

    ENSURE(!voertuigen.empty(), "Voertuig werd niet correct toegevoed.");
    return true;
}

/**
 * @brief Voeg een verkeerslicht toe aan de verkeerssituatie
 * @param licht Het toe te voegen verkeerslicht
 * @return true als het verkeerslicht succesvol werd toegevoegd, false indien niet
 */
bool VerkeersSituatie::voegVerkeerslichtToe(const Verkeerslicht& licht) {
    REQUIRE(properlyInitialized(), "voegVoertuigToe moet eindigen in een geldige toestand.");
    REQUIRE(licht.properlyInitialized(), "Verkeerslicht is niet correct ingesteld.");
    std::string baanNaam = licht.getBaan();

    // Controleer of de baan bestaat
    auto it = banen.find(baanNaam);
    if (it == banen.end()) {
        return false;
    }

    // Controleer of de positie geldig is
    if (licht.getPositie() < 0 || licht.getPositie() > it->second.getLengte()) {
        return false;
    }

    // Controleer of de cyclustijd geldig is
    if (licht.getCyclus() <= 0) {
        return false;
    }

    // Controleer of er geen andere verkeerslichten in de vertragingsafstand zijn
    for (const auto& existingLight : verkeerslichten) {
        if (existingLight.getBaan() == baanNaam) {
            // Verkeerslichten mogen niet in elkaars vertragingsafstand staan (50m volgens specificatie)
            const double distance = std::abs(existingLight.getPositie() - licht.getPositie());
            if (distance < 50) {
                return false;
            }
        }
    }

    verkeerslichten.push_back(licht);
    ENSURE(!verkeerslichten.empty(), "Verkeerslicht werdt niet correct toegevoed aan situatie.");
    return true;
}

/**
 * @brief Voeg een voertuiggenerator toe aan de verkeerssituatie
 * @param generator De toe te voegen voertuiggenerator
 * @return true als de generator succesvol werd toegevoegd, false indien niet
 */
bool VerkeersSituatie::voegVoertuigGeneratorToe(const VoertuigGenerator& generator) {
    REQUIRE(properlyInitialized(), "voegVoertuigGeneratorToe moet eindigen in een geldige toestand.");
    REQUIRE(generator.properlyInitialized(), "Generator is niet correct ingesteld");
    std::string baanNaam = generator.getBaanNaam();

    // Controleer of de baan bestaat
    auto it = banen.find(baanNaam);
    if (it == banen.end()) {
        return false;
    }

    generators.push_back(generator);
    ENSURE(!generators.empty(), "Generator werd niet correct toegevoed.");
    return true;
}

/**
 * @brief Voeg een bushalte toe aan de verkeerssituatie
 * @param bushalte De toe te voegen bushalte
 * @return true als de bushalte succesvol werd toegevoegd, false indien niet
 */
bool VerkeersSituatie::voegBushalteToe(const Bushalte& bushalte) {
    REQUIRE(properlyInitialized(), "voegBushalteToe moet eindigen in een geldige toestand.");
    REQUIRE(bushalte.properlyInitialized(), "Bushalte is niet correct geïnitialiseerd");

    std::string baanNaam = bushalte.getBaan();

    // Controleer of de baan bestaat
    auto it = banen.find(baanNaam);
    if (it == banen.end()) {
        return false;
    }

    // Controleer of de positie geldig is
    if (bushalte.getPositie() < 0 || bushalte.getPositie() > it->second.getLengte()) {
        return false;
    }

    // Controleer of de wachttijd geldig is
    if (bushalte.getWachttijd() <= 0) {
        return false;
    }

    bushaltes.push_back(bushalte);
    ENSURE(!bushaltes.empty(), "Bushalte werd niet correct toegevoed.");
    return true;
}

/**
 * @brief Voeg een kruispunt toe aan de verkeerssituatie
 * @param kruispunt Het toe te voegen kruispunt
 * @return true als het kruispunt succesvol werd toegevoegd, false indien niet
 */
bool VerkeersSituatie::voegKruispuntToe(const Kruispunt& kruispunt) {
    REQUIRE(properlyInitialized(), "voegKruispuntToe moet eindigen in een geldige toestand.");
    REQUIRE(kruispunt.properlyInitialized(), "Kruispunt is niet correct geïnitialiseerd");

    // Haal alle banen in het kruispunt op
    auto baanParen = kruispunt.getBanen();

    // Controleer of er tenminste één baan in het kruispunt is
    if (baanParen.empty()) {
        return false;
    }

    // Controleer of alle banen bestaan en posities geldig zijn
    for (const auto& paar : baanParen) {
        const std::string& baanNaam = paar.first;
        double positie = paar.second;

        // Controleer of de baan bestaat
        auto it = banen.find(baanNaam);
        if (it == banen.end()) {
            return false;
        }

        // Controleer of de positie geldig is
        if (positie < 0 || positie > it->second.getLengte()) {
            return false;
        }
    }

    // Alle banen zijn geldig, voeg het kruispunt toe
    kruispunten.push_back(kruispunt);
    ENSURE(!kruispunten.empty(), "Kruispunt werd niet correct toegevoegd.");
    return true;
}

/**
 * @brief Verifieer de consistentie van de verkeerssituatie
 * @return true als de verkeerssituatie consistent is, false indien niet
 */
bool VerkeersSituatie::verificeerConsistentie() const {
    REQUIRE(properlyInitialized(), "verificeerConsistentie moet eindigen in een geldige toestand.");

    // Controleer of er minstens één baan is
    if (banen.empty()) {
        return false;
    }

    // Controleer of alle voertuigen op geldige banen zitten met geldige posities
    for (const auto& voertuig : voertuigen) {
        std::string baanNaam = voertuig->getBaanNaam();
        auto it = banen.find(baanNaam);
        if (it == banen.end()) {
            return false;
        }

        if (voertuig->getPositie() < 0 || voertuig->getPositie() > it->second.getLengte()) {
            return false;
        }
    }

    // Controleer of alle verkeerslichten op geldige banen zitten met geldige posities
    for (const auto& licht : verkeerslichten) {
        std::string baanNaam = licht.getBaan();
        auto it = banen.find(baanNaam);
        if (it == banen.end()) {
            return false;
        }

        if (licht.getPositie() < 0 || licht.getPositie() > it->second.getLengte()) {
            return false;
        }
    }

    // Controleer of alle generators op geldige banen zitten
    for (const auto& generator : generators) {
        std::string baanNaam = generator.getBaanNaam();
        if (banen.find(baanNaam) == banen.end()) {
            return false;
        }
    }

    // Controleer of alle bushaltes op geldige banen zitten met geldige posities
    for (const auto& bushalte : bushaltes) {
        std::string baanNaam = bushalte.getBaan();
        auto it = banen.find(baanNaam);
        if (it == banen.end()) {
            return false;
        }

        if (bushalte.getPositie() < 0 || bushalte.getPositie() > it->second.getLengte()) {
            return false;
        }
    }

    // Controleer of alle kruispunten geldige banen en posities hebben
    for (const auto& kruispunt : kruispunten) {
        auto baanParen = kruispunt.getBanen();
        for (const auto& paar : baanParen) {
            const std::string& baanNaam = paar.first;
            double positie = paar.second;

            auto it = banen.find(baanNaam);
            if (it == banen.end()) {
                return false;
            }

            if (positie < 0 || positie > it->second.getLengte()) {
                return false;
            }
        }
    }
    ENSURE(true, "Constistentie is goed beëindigd.");
    return true;
}

/**
 * @brief Verwijder een voertuig uit de simulatie
 * @param index Index van het te verwijderen voertuig
 * @return true als het voertuig succesvol werd verwijderd, false indien niet
 */
bool VerkeersSituatie::verwijderVoertuig(int index) {
    REQUIRE(properlyInitialized(), "verwijderVoertuig moet eindigen in een geldige toestand.");
    if (index < 0 || index >= static_cast<int>(voertuigen.size())) {
        return false;
    }

    voertuigen.erase(voertuigen.begin() + index);
    ENSURE(static_cast<int>(voertuigen.size()) >= 0, "Aantal Voertuig out of range.");
    return true;
}

/**
 * @brief Krijg een map van alle banen
 * @return Map met baannamen als sleutels en baanobjecten als waarden
 */
const std::map<std::string, Baan>& VerkeersSituatie::getBanen() const {
    REQUIRE(properlyInitialized(), "getBanen moet eindigen in een geldige toestand.");
    return banen;
}

/**
 * @brief Maak kopieën van alle voertuigen
 * @return Vector met kopieën van alle voertuigen
 */
std::vector<std::unique_ptr<Voertuig>> VerkeersSituatie::kopieVoertuigen() const {
    REQUIRE(properlyInitialized(), "kopieVoertuigen moet eindigen in een geldige toestand.");

    std::vector<std::unique_ptr<Voertuig>> result;
    result.reserve(voertuigen.size());

    for (const auto& voertuig : voertuigen) {
        result.push_back(voertuig->clone());
    }

    return result;
}

/**
 * @brief Krijg wijzigbare referentie naar alle voertuigen
 * @return Vector met alle voertuigen
 */
std::vector<std::unique_ptr<Voertuig>>& VerkeersSituatie::getVoertuigen() {
    REQUIRE(properlyInitialized(), "getVoertuigen moet eindigen in een geldige toestand.");
    return voertuigen;
}

/**
 * @brief Krijg constante referentie naar alle voertuigen
 * @return Vector met alle voertuigen
 */
const std::vector<std::unique_ptr<Voertuig>>& VerkeersSituatie::getVoertuigen() const {
    REQUIRE(properlyInitialized(), "getVoertuigen moet eindigen in een geldige toestand.");
    return voertuigen;
}

/**
 * @brief Geef het aantal voertuigen in de situatie
 * @return Aantal voertuigen
 */
size_t VerkeersSituatie::getAantalVoertuigen() const {
    REQUIRE(properlyInitialized(), "getAantalVoertuigen moet eindigen in een geldige toestand.");
    return voertuigen.size();
}

/**
 * @brief Krijg alle verkeerslichten
 * @return Vector met alle verkeerslichten
 */
const std::vector<Verkeerslicht>& VerkeersSituatie::getVerkeerslichten() const {
    REQUIRE(properlyInitialized(), "getVerkeerslichten moet eindigen in een geldige toestand.");
    return verkeerslichten;
}

/**
 * @brief Krijg wijzigbare referentie naar alle verkeerslichten
 * @return Vector met alle verkeerslichten
 */
std::vector<Verkeerslicht>& VerkeersSituatie::getVerkeerslichten() {
    REQUIRE(properlyInitialized(), "getVerkeerslichten moet eindigen in een geldige toestand.");
    return verkeerslichten;
}

/**
 * @brief Krijg alle voertuiggenerators
 * @return Vector met alle voertuiggenerators
 */
const std::vector<VoertuigGenerator>& VerkeersSituatie::getVoertuigGenerators() const {
    REQUIRE(properlyInitialized(), "getVoertuigGenerators moet eindigen in een geldige toestand.");
    return generators;
}

/**
 * @brief Krijg alle bushaltes
 * @return Vector met alle bushaltes
 */
const std::vector<Bushalte>& VerkeersSituatie::getBushaltes() const {
    REQUIRE(properlyInitialized(), "getBushaltes moet eindigen in een geldige toestand.");
    return bushaltes;
}

/**
 * @brief Krijg wijzigbare referentie naar alle bushaltes
 * @return Vector met alle bushaltes
 */
std::vector<Bushalte>& VerkeersSituatie::getBushaltes() {
    REQUIRE(properlyInitialized(), "getBushaltes moet eindigen in een geldige toestand.");
    return bushaltes;
}

/**
 * @brief Krijg alle kruispunten
 * @return Vector met alle kruispunten
 */
const std::vector<Kruispunt>& VerkeersSituatie::getKruispunten() const {
    REQUIRE(properlyInitialized(), "getKruispunten moet eindigen in een geldige toestand.");
    return kruispunten;
}

/**
 * @brief Zoek bushaltes op een specifieke baan
 * @param baanNaam Naam van de baan
 * @return Vector met pointers naar bushaltes op de baan
 */
std::vector<Bushalte*> VerkeersSituatie::zoekBushaltesOpBaan(const std::string& baanNaam) {
    REQUIRE(properlyInitialized(), "zoekBushaltesOpBaan moet eindigen in een geldige toestand.");
    REQUIRE(!baanNaam.empty(), "Baannaam mag niet leeg zijn.");
    std::vector<Bushalte*> result;

    for (auto& bushalte : bushaltes) {
        if (bushalte.getBaan() == baanNaam) {
            result.push_back(&bushalte);
        }
    }

    // Sorteer op positie
    std::sort(result.begin(), result.end(), [](const Bushalte* a, const Bushalte* b) {
        return a->getPositie() < b->getPositie();
    });

    return result;
}

/**
 * @brief Zoek de volgende bushalte voor een voertuig
 * @param voertuig Het voertuig waarvoor de volgende bushalte wordt gezocht
 * @return Pointer naar de volgende bushalte, nullptr indien geen
 */
Bushalte* VerkeersSituatie::zoekEerstvolgendeHalte(const Voertuig& voertuig) {
    REQUIRE(properlyInitialized(), "zoekEerstvolgendeHalte moet eindigen in een geldige toestand.");
    Bushalte* eerstvolgend = nullptr;
    double kortsteAfstand = std::numeric_limits<double>::max();

    // Zoek alleen naar bushaltes als het voertuig een bus is
    if (!voertuig.isBus()) {
        return nullptr;
    }

    std::string baanNaam = voertuig.getBaanNaam();
    double voertuigPositie = voertuig.getPositie();

    for (auto& bushalte : bushaltes) {
        if (bushalte.getBaan() == baanNaam && bushalte.getPositie() > voertuigPositie) {
            double afstand = bushalte.getPositie() - voertuigPositie;

            if (afstand < kortsteAfstand) {
                kortsteAfstand = afstand;
                eerstvolgend = &bushalte;
            }
        }
    }

    return eerstvolgend;
}

/**
 * @brief Zoek het volgende verkeerslicht voor een voertuig
 * @param voertuig Het voertuig waarvoor het volgende verkeerslicht wordt gezocht
 * @return Pointer naar het volgende verkeerslicht, nullptr indien geen
 */
Verkeerslicht* VerkeersSituatie::zoekEerstvolgendeVerkeerslicht(const Voertuig& voertuig) {
    REQUIRE(properlyInitialized(), "zoekEerstvolgendeVerkeerslicht moet eindigen in een geldige toestand.");
    Verkeerslicht* eerstvolgend = nullptr;
    double kortsteAfstand = std::numeric_limits<double>::max();

    std::string baanNaam = voertuig.getBaanNaam();
    double voertuigPositie = voertuig.getPositie();

    for (auto& verkeerslicht : verkeerslichten) {
        if (verkeerslicht.getBaan() == baanNaam && verkeerslicht.getPositie() > voertuigPositie) {
            double afstand = verkeerslicht.getPositie() - voertuigPositie;

            if (afstand < kortsteAfstand) {
                kortsteAfstand = afstand;
                eerstvolgend = &verkeerslicht;
            }
        }
    }

    return eerstvolgend;
}


/**
 * @brief Zoek kruispunten die een specifieke baan omvatten
 * @param baanNaam Naam van de baan
 * @return Vector met pointers naar kruispunten die de baan omvatten
 */
std::vector<Kruispunt*> VerkeersSituatie::zoekKruispuntenOpBaan(const std::string& baanNaam) {
    REQUIRE(properlyInitialized(), "zoekKruispuntenOpBaan moet eindigen in een geldige toestand.");
    REQUIRE(!baanNaam.empty(), "BaanNaam mag niet leeg zijn.");
    std::vector<Kruispunt*> result;

    for (auto& kruispunt : kruispunten) {
        if (kruispunt.bevatBaan(baanNaam)) {
            result.push_back(&kruispunt);
        }
    }

    return result;
}

/**
 * @brief Zoek het volgende kruispunt voor een voertuig
 * @param voertuig Het voertuig waarvoor het volgende kruispunt wordt gezocht
 * @return Pointer naar het volgende kruispunt, nullptr indien geen
 */
Kruispunt* VerkeersSituatie::zoekEerstvolgendeKruispunt(const Voertuig& voertuig) {
    REQUIRE(properlyInitialized(), "zoekEerstvolgendeKruispunt moet eindigen in een geldige toestand.");
    Kruispunt* eerstvolgend = nullptr;
    double kortsteAfstand = std::numeric_limits<double>::max();

    std::string baanNaam = voertuig.getBaanNaam();
    double voertuigPositie = voertuig.getPositie();

    for (auto& kruispunt : kruispunten) {
        if (kruispunt.bevatBaan(baanNaam)) {
            double kruispuntPositie = kruispunt.getPositieOpBaan(baanNaam);

            if (kruispuntPositie > voertuigPositie) {
                double afstand = kruispuntPositie - voertuigPositie;

                if (afstand < kortsteAfstand) {
                    kortsteAfstand = afstand;
                    eerstvolgend = &kruispunt;
                }
            }
        }
    }

    return eerstvolgend;
}