#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "../Situation/situatie.h"
#include "../FileReader/bestandslezer.h"
#include "../Simulation/simulatie.h"
#include "../Output/output.h"

using namespace std;

/**
 * @brief Toon help-informatie voor gebruikerscommando's
 * @post Help-tekst is weergegeven op stdout
 *
 * Toont een overzicht van alle beschikbare commando's en hun functionaliteit.
 */
void displayHelp() {
    cout << "Verkeerssimulatie - Help\n";
    cout << "------------------------\n";
    cout << "Commando's:\n";
    cout << "  h - Help weergeven\n";
    cout << "  s - Enkele simulatiestap uitvoeren\n";
    cout << "  r - Simulatie runnen (10 stappen)\n";
    cout << "  c - Continue simulatie starten/stoppen\n";
    cout << "  t - Tekstrapport weergeven\n";
    cout << "  g - Grafische impressie weergeven\n";
    cout << "  x - Situatie naar XML bestand schrijven\n";
    cout << "  w - Situatie naar HTML bestand schrijven\n";
    cout << "  a - Automatisch voertuigen genereren aan/uit\n";
    cout << "  q - Afsluiten\n";
}

/**
 * @brief Hoofdprogramma voor de verkeerssimulatie
 * @param argc Aantal command-line argumenten
 * @param argv Array van command-line argumenten
 * @return 0 bij succesvolle uitvoering, 1 bij fout
 * @pre argc == 2 (programmanaam + XML-bestandsnaam)
 * @pre argv[1] moet een geldig pad naar een XML-bestand zijn
 * @post Simulatie is uitgevoerd volgens gebruikersinput
 * @post Eindstatistieken zijn weergegeven
 *
 * Het programma:
 * 1. Controleert command-line argumenten
 * 2. Laadt de verkeerssituatie uit het XML-bestand
 * 3. Initialiseert de simulatie met 0.1s tijdstap
 * 4. Biedt een interactieve command-line interface
 * 5. Verwerkt gebruikerscommando's tot 'q' (quit)
 * 6. Toont eindstatistieken
 */
int main(int argc, char* argv[]) {
    std::cout << "Verkeerssimulatie v2.0 - Opstart\n";

    // Controleer command-line argumenten
    if (argc != 2) {
        std::cerr << "Gebruik: " << argv[0] << " <XML-bestand>\n";
        return 1;
    }

    const string bestandsnaam = argv[1];
    std::cout << "Bestandsnaam: " << bestandsnaam << "\n";

    // Maak een verkeerssituatie en een bestandslezer
    VerkeersSituatie situatie;
    BestandsLezer lezer;

    // Probeer het bestand te lezen
    if (!lezer.leesXmlBestand(bestandsnaam, situatie)) {
        cerr << "FOUT: " << lezer.getLastFoutmelding() << endl;
        return 1;
    }

    cout << "Verkeerssituatie succesvol ingelezen.\n";

    // Maak een simulatie met de geladen verkeerssituatie
    simulatie sim(situatie, 0.1); // 0.1 seconde tijdstap
    output uitvoer;

    bool running = false;      ///< Status van continue simulatie modus
    bool autoGenerate = false; ///< Status van automatische voertuiggeneratie

    // Toon help bij opstarten
    displayHelp();

    // Hoofdprogrammalus
    char cmd;
    do {
        // Draai continue simulatie indien ingeschakeld
        if (running) {
            sim.stap();

            // Toon minimale info tijdens continue run
            cout << "Tijd: " << sim.getHuidigeSimulatieTijd()
                 << "s, Voertuigen: " << sim.getAantalVoertuigen()
                 << ", Gem. snelheid: " << sim.getGemiddeldeSnelheid() << " m/s\r" << flush;

            // Voeg een kleine vertraging toe om de simulatie zichtbaar te maken
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Controleer op gebruikersinvoer (niet-blokkerend)
            if (cin.peek() != EOF) {
                cin >> cmd;
                if (cmd == 'c') {
                    running = false;
                    cout << "\nContinue modus gestopt.                                       \n";
                }
            }

            continue; // Sla de normale commandoinvoer over
        }

        // Vraag om commando
        cout << "\nVoer commando in (h voor help): ";
        cin >> cmd;

        // Verwerk commando
        switch (cmd) {
            case 'h': // Help
                /**
                 * @brief Toon help-informatie
                 * @post Help-tekst is weergegeven
                 */
                displayHelp();
                break;

            case 's': // Enkele stap
                /**
                 * @brief Voer één simulatiestap uit
                 * @post Simulatie is één tijdstap vooruitgegaan
                 * @post Statistieken zijn weergegeven
                 */
                sim.stap();
                cout << "Simulatiestap uitgevoerd. Nieuwe tijd: " << sim.getHuidigeSimulatieTijd() << "s\n";
                cout << "Aantal voertuigen: " << sim.getAantalVoertuigen() << "\n";
                cout << "Gemiddelde snelheid: " << sim.getGemiddeldeSnelheid() << " m/s\n";
                break;

            case 'r': // Draai 10 stappen
                /**
                 * @brief Voer 10 simulatiestappen uit
                 * @post Simulatie is 10 tijdstappen vooruitgegaan
                 * @post Progress van elke stap is weergegeven
                 * @post Eindstatistieken zijn weergegeven
                 */
                cout << "Uitvoeren van 10 simulatiestappen...\n";
                for (int i = 0; i < 10; i++) {
                    sim.stap();
                    cout << "Stap " << i+1 << " - Tijd: " << sim.getHuidigeSimulatieTijd()
                         << "s, Voertuigen: " << sim.getAantalVoertuigen() << "\n";
                }
                cout << "10 stappen voltooid. Gemiddelde snelheid: "
                     << sim.getGemiddeldeSnelheid() << " m/s\n";
                break;

            case 'c': // Continue simulatie
                /**
                 * @brief Start continue simulatie modus
                 * @post running == true
                 * @post Simulatie draait automatisch tot gebruiker 'c' indrukt
                 */
                running = true;
                cout << "Continue simulatie gestart. Druk 'c' om te stoppen.\n";
                break;

            case 't': // Tekstrapport
                /**
                 * @brief Genereer en toon tekstrapport
                 * @post Tekstuele weergave van verkeerssituatie is getoond
                 */
                cout << "Tekstrapport:\n";
                cout << "=============\n";
                cout << uitvoer.genereerTekstRapport(situatie) << "\n";
                break;

            case 'g': // Grafische impressie
                /**
                 * @brief Genereer en toon grafische impressie (ASCII-art)
                 * @post ASCII-art weergave van verkeerssituatie is getoond
                 */
                cout << "Grafische impressie:\n";
                cout << "===================\n";
                cout << uitvoer.genereerGrafischeImpressie(situatie) << "\n";
                break;

            case 'x': // Schrijf naar XML
                /**
                 * @brief Exporteer verkeerssituatie naar XML-bestand
                 * @post XML-bestand "simulatie_uitvoer.xml" is aangemaakt
                 * @post Succes/faal bericht is weergegeven
                 */
                {
                    string xmlBestand = "simulatie_uitvoer.xml";
                    if (uitvoer.schrijfNaarXml(situatie, xmlBestand)) {
                        cout << "Verkeerssituatie succesvol geschreven naar " << xmlBestand << "\n";
                    } else {
                        cout << "Fout bij schrijven naar XML: " << uitvoer.getLastFoutmelding() << "\n";
                    }
                }
                break;

            case 'w': // Schrijf naar HTML
                /**
                 * @brief Exporteer verkeerssituatie naar HTML-bestand
                 * @post HTML-bestand "simulatie_uitvoer.html" is aangemaakt
                 * @post Succes/faal bericht is weergegeven
                 */
                {
                    string htmlBestand = "simulatie_uitvoer.html";
                    if (uitvoer.schrijfNaarHtml(situatie, htmlBestand)) {
                        cout << "Verkeerssituatie succesvol geschreven naar " << htmlBestand << "\n";
                    } else {
                        cout << "Fout bij schrijven naar HTML: " << uitvoer.getLastFoutmelding() << "\n";
                    }
                }
                break;

            case 'a': // Schakel automatisch genereren van voertuigen in/uit
                /**
                 * @brief Toggle automatische voertuiggeneratie
                 * @post autoGenerate status is omgekeerd
                 * @post Simulatie gebruikt nieuwe autoGenerate setting
                 * @post Status is weergegeven aan gebruiker
                 */
                autoGenerate = !autoGenerate;
                sim.setAutoGenereerVoertuigen(autoGenerate);
                cout << "Automatisch voertuigen genereren: " << (autoGenerate ? "AAN" : "UIT") << "\n";
                break;

            case 'q': // Afsluiten
                /**
                 * @brief Beëindig het programma
                 * @post Afsluitbericht is weergegeven
                 */
                cout << "Programma wordt afgesloten.\n";
                break;

            default:
                /**
                 * @brief Behandel onbekende commando's
                 * @post Foutbericht en help-verwijzing zijn weergegeven
                 */
                cout << "Onbekend commando. Gebruik 'h' voor help.\n";
        }
    } while (cmd != 'q');

    // Toon eindstatistieken
    cout << "\nSimulatie statistieken:\n";
    cout << "Totale simulatietijd: " << sim.getTotaleTijd() << "s\n";
    cout << "Totaal aantal voertuigen verwerkt: " << sim.getTotaalVerwijderdeVoertuigen() + sim.getAantalVoertuigen() << "\n";
    cout << "Aantal voertuigen verwijderd: " << sim.getTotaalVerwijderdeVoertuigen() << "\n";
    cout << "Gemiddelde snelheid: " << sim.getGemiddeldeSnelheid() << " m/s\n";

    return 0;
}