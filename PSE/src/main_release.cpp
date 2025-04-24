/**
 * @file main.cpp
 * @brief Main program for the traffic simulation (Revised with all features)
 */

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "situatie.h"
#include "bestandslezer.h"
#include "simulatie.h"
#include "output.h"

using namespace std;

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

int main(int argc, char* argv[]) {
    std::cout << "Verkeerssimulatie v2.0 - Opstart\n";

    if (argc != 2) {
        std::cerr << "Gebruik: " << argv[0] << " <XML-bestand>\n";
        return 1;
    }

    const string bestandsnaam = argv[1];
    std::cout << "Bestandsnaam: " << bestandsnaam << "\n";

    // Create a traffic situation and a file reader
    VerkeersSituatie situatie;
    BestandsLezer lezer;

    // Try to read the file
    if (!lezer.leesXmlBestand(bestandsnaam, situatie)) {
        cerr << "FOUT: " << lezer.getLastFoutmelding() << endl;
        return 1;
    }

    cout << "Verkeerssituatie succesvol ingelezen.\n";

    // Create a simulation with the loaded traffic situation
    simulatie sim(situatie, 0.1); // 0.1 second timestep
    output uitvoer;

    bool running = false;
    bool autoGenerate = false;

    // Display help at startup
    displayHelp();

    // Main program loop
    char cmd;
    do {
        // Run continuous simulation if enabled
        if (running) {
            sim.stap();

            // Display minimal info during continuous run
            cout << "Tijd: " << sim.getHuidigeSimulatieTijd()
                 << "s, Voertuigen: " << sim.getAantalVoertuigen()
                 << ", Gem. snelheid: " << sim.getGemiddeldeSnelheid() << " m/s\r" << flush;

            // Add a small delay to make the simulation visible
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Check for user input (non-blocking)
            if (cin.peek() != EOF) {
                cin >> cmd;
                if (cmd == 'c') {
                    running = false;
                    cout << "\nContinue modus gestopt.                                       \n";
                }
            }

            continue; // Skip the normal command input
        }

        // Prompt for command
        cout << "\nVoer commando in (h voor help): ";
        cin >> cmd;

        // Process command
        switch (cmd) {
            case 'h': // Help
                displayHelp();
                break;

            case 's': // Single step
                sim.stap();
                cout << "Simulatiestap uitgevoerd. Nieuwe tijd: " << sim.getHuidigeSimulatieTijd() << "s\n";
                cout << "Aantal voertuigen: " << sim.getAantalVoertuigen() << "\n";
                cout << "Gemiddelde snelheid: " << sim.getGemiddeldeSnelheid() << " m/s\n";
                break;

            case 'r': // Run 10 steps
                cout << "Uitvoeren van 10 simulatiestappen...\n";
                for (int i = 0; i < 10; i++) {
                    sim.stap();
                    cout << "Stap " << i+1 << " - Tijd: " << sim.getHuidigeSimulatieTijd()
                         << "s, Voertuigen: " << sim.getAantalVoertuigen() << "\n";
                }
                cout << "10 stappen voltooid. Gemiddelde snelheid: "
                     << sim.getGemiddeldeSnelheid() << " m/s\n";
                break;

            case 'c': // Continuous simulation
                running = true;
                cout << "Continue simulatie gestart. Druk 'c' om te stoppen.\n";
                break;

            case 't': // Text report
                cout << "Tekstrapport:\n";
                cout << "=============\n";
                cout << output::genereerTekstRapport(situatie) << "\n";
                break;

            case 'g': // Graphical impression
                cout << "Grafische impressie:\n";
                cout << "===================\n";
                cout << output::genereerGrafischeImpressie(situatie) << "\n";
                break;

            case 'x': // Write to XML
                {
                    string xmlBestand = "simulatie_uitvoer.xml";
                    if (uitvoer.schrijfNaarXml(situatie, xmlBestand)) {
                        cout << "Verkeerssituatie succesvol geschreven naar " << xmlBestand << "\n";
                    } else {
                        cout << "Fout bij schrijven naar XML: " << uitvoer.getLastFoutmelding() << "\n";
                    }
                }
                break;

            case 'w': // Write to HTML
                {
                    string htmlBestand = "simulatie_uitvoer.html";
                    if (uitvoer.schrijfNaarHtml(situatie, htmlBestand)) {
                        cout << "Verkeerssituatie succesvol geschreven naar " << htmlBestand << "\n";
                    } else {
                        cout << "Fout bij schrijven naar HTML: " << uitvoer.getLastFoutmelding() << "\n";
                    }
                }
                break;

            case 'a': // Toggle auto-generate vehicles
                autoGenerate = !autoGenerate;
                sim.setAutoGenereerVoertuigen(autoGenerate);
                cout << "Automatisch voertuigen genereren: " << (autoGenerate ? "AAN" : "UIT") << "\n";
                break;

            case 'q': // Quit
                cout << "Programma wordt afgesloten.\n";
                break;

            default:
                cout << "Onbekend commando. Gebruik 'h' voor help.\n";
        }
    } while (cmd != 'q');

    // Display final statistics
    cout << "\nSimulatie statistieken:\n";
    cout << "Totale simulatietijd: " << sim.getTotaleTijd() << "s\n";
    cout << "Totaal aantal voertuigen verwerkt: " << sim.getTotaalVerwijderdeVoertuigen() + sim.getAantalVoertuigen() << "\n";
    cout << "Aantal voertuigen verwijderd: " << sim.getTotaalVerwijderdeVoertuigen() << "\n";
    cout << "Gemiddelde snelheid: " << sim.getGemiddeldeSnelheid() << " m/s\n";

    return 0;
}