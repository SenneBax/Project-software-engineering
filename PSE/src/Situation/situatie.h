#ifndef SITUATIE_H
#define SITUATIE_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "../TraficObjects/baan.h"
#include "../TraficObjects/voertuig.h"
#include "../TraficObjects/verkeerslicht.h"
#include "../TraficObjects/voertuiggenerator.h"
#include "../TraficObjects/bushalte.h"
#include "../TraficObjects/kruispunt.h"

/**
 * @brief Klasse die een verkeerssituatie voorstelt
 *
 * De VerkeersSituatie klasse is de centrale container voor alle elementen
 * van een verkeerssimulatie. Het beheert de relaties tussen verschillende
 * verkeerscomponenten en zorgt ervoor dat de gehele situatie consistent
 * blijft gedurende de simulatie.
 */
class VerkeersSituatie {
public:
    /**
     * @brief Constructor voor VerkeersSituatie
     * @pre Geen specifieke precondities
     * @post properlyInitialized() == true
     * @post Alle containers zijn leeg maar geldig
     * @post Object is klaar voor het toevoegen van verkeerselementen
     *
     * Initialiseert een lege verkeerssituatie in een geldige toestand.
     */
    VerkeersSituatie();

    /**
     * @brief Copy constructor - deleted omdat we unieke pointers gebruiken
     *
     * Copy constructor is uitgeschakeld omdat de klasse unique_ptr containers
     * bevat die niet gekopieerd kunnen worden.
     */
    VerkeersSituatie(const VerkeersSituatie&) = delete;

    /**
     * @brief Assignment operator - deleted omdat we unieke pointers gebruiken
     *
     * Assignment operator is uitgeschakeld vanwege unique_ptr containers.
     */
    VerkeersSituatie& operator=(const VerkeersSituatie&) = delete;

    /**
     * @brief Move constructor
     * @param other VerkeersSituatie object om van te verplaatsen
     * @pre other.properlyInitialized() == true
     * @post properlyInitialized() == true
     * @post other is in geldige maar lege toestand
     * @post Alle data is verplaatst van other naar this
     */
    VerkeersSituatie(VerkeersSituatie&&) = default;

    /**
     * @brief Move assignment operator
     * @param other VerkeersSituatie object om van te verplaatsen
     * @return Referentie naar this object
     * @pre properlyInitialized() == true
     * @pre other.properlyInitialized() == true
     * @post properlyInitialized() == true
     * @post other is in geldige maar lege toestand
     * @post Alle data is verplaatst van other naar this
     */
    VerkeersSituatie& operator=(VerkeersSituatie&&) = default;

    /**
     * @brief Destructor
     * @pre properlyInitialized() == true
     * @post Alle resources zijn opgeruimd
     *
     * Resources worden automatisch opgeruimd door unique_ptr containers.
     */
    ~VerkeersSituatie();

    /**
     * @brief Voeg een weg toe aan de verkeerssituatie
     * @param baan De toe te voegen weg
     * @return true als de weg succesvol is toegevoegd, false indien niet
     * @pre properlyInitialized() == true
     * @pre baan.properlyInitialized() == true
     * @pre !baan.getNaam().empty()
     * @pre baan.getLengte() > 0
     * @post Bij success: baan is toegevoegd en getBanen().count(baan.getNaam()) == 1
     * @post Bij success: baan is toegankelijk via getBanen()
     * @post Bij faal: situatie blijft ongewijzigd
     * @post Geen duplicate baannamen toegestaan
     *
     * Voegt een nieuwe baan toe als de naam uniek is en de baan geldig is.
     */
    bool voegBaanToe(const Baan& baan);

    /**
     * @brief Voeg een voertuig toe aan de verkeerssituatie (copy versie)
     * @param voertuig Het toe te voegen voertuig
     * @return true als het voertuig succesvol is toegevoegd, false indien niet
     * @pre properlyInitialized() == true
     * @pre voertuig.properlyInitialized() == true
     * @pre getBanen().count(voertuig.getBaanNaam()) > 0
     * @pre voertuig.getPositie() >= 0
     * @pre voertuig.getPositie() <= getBanen().at(voertuig.getBaanNaam()).getLengte()
     * @post Bij success: kopie van voertuig is toegevoegd aan getVoertuigen()
     * @post Bij success: getVoertuigen().size() is verhoogd met 1
     * @post Bij faal: situatie blijft ongewijzigd
     *
     * Maakt een kopie van het voertuig en voegt deze toe als de baan bestaat
     * en de positie geldig is.
     */
    bool voegVoertuigToe(const Voertuig& voertuig);

    /**
     * @brief Voeg een voertuig toe aan de verkeerssituatie (move versie)
     * @param voertuig Unieke pointer naar het toe te voegen voertuig
     * @return true als het voertuig succesvol is toegevoegd, false indien niet
     * @pre properlyInitialized() == true
     * @pre voertuig != nullptr
     * @pre voertuig->properlyInitialized() == true
     * @pre getBanen().count(voertuig->getBaanNaam()) > 0
     * @pre voertuig->getPositie() >= 0
     * @pre voertuig->getPositie() <= getBanen().at(voertuig->getBaanNaam()).getLengte()
     * @post Bij success: voertuig is toegevoegd aan getVoertuigen()
     * @post Bij success: getVoertuigen().size() is verhoogd met 1
     * @post Bij success: voertuig pointer is transferred naar container
     * @post Bij faal: voertuig pointer blijft geldig bij caller
     *
     * Verplaatst ownership van het voertuig naar de verkeerssituatie.
     */
    bool voegVoertuigToe(std::unique_ptr<Voertuig> voertuig);

    /**
     * @brief Voeg een verkeerslicht toe aan de verkeerssituatie
     * @param licht Het toe te voegen verkeerslicht
     * @return true als het verkeerslicht succesvol is toegevoegd, false indien niet
     * @pre properlyInitialized() == true
     * @pre licht.properlyInitialized() == true
     * @pre getBanen().count(licht.getBaan()) > 0
     * @pre licht.getPositie() >= 0
     * @pre licht.getPositie() <= getBanen().at(licht.getBaan()).getLengte()
     * @pre licht.getCyclus() > 0
     * @post Bij success: verkeerslicht is toegevoegd aan getVerkeerslichten()
     * @post Bij success: geen ander verkeerslicht binnen 50m afstand op zelfde baan
     * @post Bij faal: situatie blijft ongewijzigd
     *
     * Voegt verkeerslicht toe met validatie van positie en afstand tot andere lichten.
     */
    bool voegVerkeerslichtToe(const Verkeerslicht& licht);

    /**
     * @brief Voeg een voertuiggenerator toe aan de verkeerssituatie
     * @param generator De toe te voegen generator
     * @return true als de generator succesvol is toegevoegd, false indien niet
     * @pre properlyInitialized() == true
     * @pre generator.properlyInitialized() == true
     * @pre getBanen().count(generator.getBaanNaam()) > 0
     * @pre generator.getFrequentie() > 0
     * @post Bij success: generator is toegevoegd aan getVoertuigGenerators()
     * @post Bij faal: situatie blijft ongewijzigd
     *
     * Voegt een voertuiggenerator toe die tijdens simulatie nieuwe voertuigen
     * kan genereren op de opgegeven baan.
     */
    bool voegVoertuigGeneratorToe(const VoertuigGenerator& generator);

    /**
     * @brief Voeg een bushalte toe aan de verkeerssituatie
     * @param bushalte De toe te voegen bushalte
     * @return true als de bushalte succesvol is toegevoegd, false indien niet
     * @pre properlyInitialized() == true
     * @pre bushalte.properlyInitialized() == true
     * @pre getBanen().count(bushalte.getBaan()) > 0
     * @pre bushalte.getPositie() >= 0
     * @pre bushalte.getPositie() <= getBanen().at(bushalte.getBaan()).getLengte()
     * @pre bushalte.getWachttijd() > 0
     * @post Bij success: bushalte is toegevoegd aan getBushaltes()
     * @post Bij faal: situatie blijft ongewijzigd
     *
     * Voegt een bushalte toe waar bussen kunnen stoppen en wachten.
     */
    bool voegBushalteToe(const Bushalte& bushalte);

    /**
     * @brief Voeg een kruispunt toe aan de verkeerssituatie
     * @param kruispunt Het toe te voegen kruispunt
     * @return true als het kruispunt succesvol is toegevoegd, false indien niet
     * @pre properlyInitialized() == true
     * @pre kruispunt.properlyInitialized() == true
     * @pre kruispunt.getBanen().size() > 0
     * @pre Alle banen in kruispunt bestaan in getBanen()
     * @pre Alle posities in kruispunt zijn geldig voor hun respectievelijke banen
     * @post Bij success: kruispunt is toegevoegd aan getKruispunten()
     * @post Bij faal: situatie blijft ongewijzigd
     *
     * Voegt een kruispunt toe dat meerdere banen verbindt en voertuigverplaatsing
     * tussen banen mogelijk maakt.
     */
    bool voegKruispuntToe(const Kruispunt& kruispunt);

    /**
     * @brief Verifieer de consistentie van de verkeerssituatie
     * @return true als de verkeerssituatie consistent is, false indien niet
     * @pre properlyInitialized() == true
     * @post return waarde geeft aan of alle elementen consistent zijn
     * @post Bij true: alle voertuigen/lichten/etc zijn op geldige banen met geldige posities
     * @post Bij false: er zijn inconsistenties in de data
     *
     * Controleert of:
     * - Er minimaal één baan bestaat
     * - Alle voertuigen op bestaande banen staan binnen baanlengte
     * - Alle verkeerslichten op bestaande banen staan binnen baanlengte
     * - Alle generators verwijzen naar bestaande banen
     * - Alle bushaltes op bestaande banen staan binnen baanlengte
     * - Alle kruispunten verwijzen naar bestaande banen met geldige posities
     */
    bool verificeerConsistentie() const;

    /**
     * @brief Verwijder een voertuig uit de simulatie
     * @param index Index van het te verwijderen voertuig
     * @return true als het voertuig succesvol is verwijderd, false indien niet
     * @pre properlyInitialized() == true
     * @pre index >= 0
     * @pre index < static_cast<int>(getVoertuigen().size())
     * @post Bij success: voertuig op index is verwijderd
     * @post Bij success: getVoertuigen().size() is verminderd met 1
     * @post Bij success: indices van voertuigen na index zijn verminderd met 1
     * @post Bij faal: situatie blijft ongewijzigd
     *
     * Verwijdert het voertuig op de opgegeven index uit de voertuigen vector.
     */
    bool verwijderVoertuig(int index);

    /**
     * @brief Haal een map van alle wegen op
     * @return Map met wegnamen als sleutels en wegobjecten als waarden
     * @pre properlyInitialized() == true
     * @post return waarde bevat alle banen geïndexeerd op naam
     * @post return waarde is read-only referentie
     *
     * Geeft toegang tot alle banen in de verkeerssituatie via hun namen.
     */
    const std::map<std::string, Baan>& getBanen() const;

    /**
     * @brief Haal een vector met kopieën van alle voertuigen op
     * @return Vector met kopieën van alle voertuigen
     * @pre properlyInitialized() == true
     * @post return waarde bevat kopieën van alle voertuigen
     * @post wijzigingen aan return waarde beïnvloeden originele situatie niet
     *
     * Maakt deep copies van alle voertuigen voor veilige toegang zonder
     * de originele situatie te kunnen wijzigen.
     */
    std::vector<std::unique_ptr<Voertuig>> kopieVoertuigen() const;

    /**
     * @brief Haal een aanpasbare referentie naar alle voertuigen op
     * @return Vector met alle voertuigen
     * @pre properlyInitialized() == true
     * @post return waarde is aanpasbare referentie naar voertuigen container
     * @post wijzigingen aan return waarde beïnvloeden originele situatie
     *
     * Geeft directe toegang tot de voertuigen container voor simulatie updates.
     */
    std::vector<std::unique_ptr<Voertuig>>& getVoertuigen();

    /**
     * @brief Haal een constante referentie naar alle voertuigen op
     * @return Vector met alle voertuigen
     * @pre properlyInitialized() == true
     * @post return waarde is read-only referentie naar voertuigen container
     *
     * Geeft read-only toegang tot alle voertuigen voor inspectie.
     */
    const std::vector<std::unique_ptr<Voertuig>>& getVoertuigen() const;

    /**
     * @brief Geef het aantal voertuigen in de situatie
     * @return Aantal voertuigen
     * @pre properlyInitialized() == true
     * @post return waarde == getVoertuigen().size()
     * @post return waarde >= 0
     *
     * Hulpmethode voor het verkrijgen van het huidige aantal voertuigen.
     */
    size_t getAantalVoertuigen() const;

    /**
     * @brief Haal alle verkeerslichten op
     * @return Vector met alle verkeerslichten
     * @pre properlyInitialized() == true
     * @post return waarde is read-only referentie naar verkeerslichten container
     *
     * Geeft read-only toegang tot alle verkeerslichten.
     */
    const std::vector<Verkeerslicht>& getVerkeerslichten() const;

    /**
     * @brief Haal een aanpasbare referentie naar alle verkeerslichten op
     * @return Vector met alle verkeerslichten
     * @pre properlyInitialized() == true
     * @post return waarde is aanpasbare referentie naar verkeerslichten container
     *
     * Geeft directe toegang tot verkeerslichten voor simulatie updates.
     */
    std::vector<Verkeerslicht>& getVerkeerslichten();

    /**
     * @brief Haal alle voertuiggeneratoren op
     * @return Vector met alle voertuiggeneratoren
     * @pre properlyInitialized() == true
     * @post return waarde is read-only referentie naar generators container
     *
     * Geeft toegang tot alle voertuiggeneratoren voor simulatie gebruik.
     */
    const std::vector<VoertuigGenerator>& getVoertuigGenerators() const;

    /**
     * @brief Haal alle bushaltes op
     * @return Vector met alle bushaltes
     * @pre properlyInitialized() == true
     * @post return waarde is read-only referentie naar bushaltes container
     *
     * Geeft read-only toegang tot alle bushaltes.
     */
    const std::vector<Bushalte>& getBushaltes() const;

    /**
     * @brief Haal een aanpasbare referentie naar alle bushaltes op
     * @return Vector met alle bushaltes
     * @pre properlyInitialized() == true
     * @post return waarde is aanpasbare referentie naar bushaltes container
     *
     * Geeft directe toegang tot bushaltes voor simulatie updates.
     */
    std::vector<Bushalte>& getBushaltes();

    /**
     * @brief Haal alle kruispunten op
     * @return Vector met alle kruispunten
     * @pre properlyInitialized() == true
     * @post return waarde is read-only referentie naar kruispunten container
     *
     * Geeft toegang tot alle kruispunten voor simulatie gebruik.
     */
    const std::vector<Kruispunt>& getKruispunten() const;

    /**
     * @brief Zoek bushaltes op een specifieke weg
     * @param baanNaam Naam van de weg
     * @return Vector met pointers naar bushaltes op de weg
     * @pre properlyInitialized() == true
     * @pre !baanNaam.empty()
     * @post return waarde bevat alle bushaltes op opgegeven baan
     * @post return waarde is gesorteerd op positie (laag naar hoog)
     * @post alle pointers in return waarde zijn geldig
     *
     * Zoekt alle bushaltes die zich op de opgegeven baan bevinden
     * en retourneert ze gesorteerd op positie.
     */
    std::vector<Bushalte*> zoekBushaltesOpBaan(const std::string& baanNaam);

    /**
     * @brief Zoek de volgende bushalte voor een voertuig
     * @param voertuig Het voertuig waarvoor de volgende bushalte wordt gezocht
     * @return Pointer naar de volgende bushalte, nullptr indien geen
     * @pre properlyInitialized() == true
     * @pre voertuig.properlyInitialized() == true
     * @post Bij niet-nullptr: return waarde->getPositie() > voertuig.getPositie()
     * @post Bij niet-nullptr: return waarde->getBaan() == voertuig.getBaanNaam()
     * @post Bij nullptr: geen bushalte voor dit voertuig of voertuig is geen bus
     *
     * Zoekt de eerstvolgende bushalte voor een bus op dezelfde baan.
     * Retourneert nullptr als het voertuig geen bus is of geen bushalte volgt.
     */
    Bushalte* zoekEerstvolgendeHalte(const Voertuig& voertuig);

    /**
     * @brief Zoek het volgende verkeerslicht voor een voertuig
     * @param voertuig Het voertuig waarvoor het volgende verkeerslicht wordt gezocht
     * @return Pointer naar het volgende verkeerslicht, nullptr indien geen
     * @pre properlyInitialized() == true
     * @pre voertuig.properlyInitialized() == true
     * @post Bij niet-nullptr: return waarde->getPositie() > voertuig.getPositie()
     * @post Bij niet-nullptr: return waarde->getBaan() == voertuig.getBaanNaam()
     * @post Bij nullptr: geen verkeerslicht volgt op deze baan
     *
     * Zoekt het eerstvolgende verkeerslicht voor een voertuig op dezelfde baan.
     */
    Verkeerslicht* zoekEerstvolgendeVerkeerslicht(const Voertuig& voertuig);

    /**
     * @brief Zoek kruispunten die een specifieke weg bevatten
     * @param baanNaam Naam van de weg
     * @return Vector met pointers naar kruispunten die de weg bevatten
     * @pre properlyInitialized() == true
     * @pre !baanNaam.empty()
     * @post return waarde bevat alle kruispunten die opgegeven baan bevatten
     * @post alle pointers in return waarde zijn geldig
     *
     * Zoekt alle kruispunten waar de opgegeven baan deel van uitmaakt.
     */
    std::vector<Kruispunt*> zoekKruispuntenOpBaan(const std::string& baanNaam);

    /**
     * @brief Zoek het volgende kruispunt voor een voertuig
     * @param voertuig Het voertuig waarvoor het volgende kruispunt wordt gezocht
     * @return Pointer naar het volgende kruispunt, nullptr indien geen
     * @pre properlyInitialized() == true
     * @pre voertuig.properlyInitialized() == true
     * @post Bij niet-nullptr: kruispunt bevat voertuig.getBaanNaam()
     * @post Bij niet-nullptr: kruispunt positie > voertuig.getPositie()
     * @post Bij nullptr: geen kruispunt volgt op deze baan
     *
     * Zoekt het eerstvolgende kruispunt voor een voertuig op dezelfde baan.
     */
    Kruispunt* zoekEerstvolgendeKruispunt(const Voertuig& voertuig);

    /**
     * @brief Controleer of het VerkeersSituatie object correct is geïnitialiseerd
     * @return true als het object correct is geïnitialiseerd, false anders
     * @post return waarde geeft aan of object in geldige toestand verkeert
     *
     * Design by Contract verificatie methode om object integriteit
     * te controleren.
     */
    bool properlyInitialized() const;

private:
    std::map<std::string, Baan> banen;        ///< De wegen in de verkeerssituatie, geïndexeerd op naam
    std::vector<std::unique_ptr<Voertuig>> voertuigen;  ///< De voertuigen in de verkeerssituatie
    std::vector<Verkeerslicht> verkeerslichten; ///< De verkeerslichten in de verkeerssituatie
    std::vector<VoertuigGenerator> generators;  ///< De voertuiggeneratoren in de verkeerssituatie
    std::vector<Bushalte> bushaltes;          ///< De bushaltes in de verkeerssituatie
    std::vector<Kruispunt> kruispunten;       ///< De kruispunten in de verkeerssituatie
    VerkeersSituatie* _initCheck; ///< Pointer voor Design by Contract verificatie
};

#endif // SITUATIE_H