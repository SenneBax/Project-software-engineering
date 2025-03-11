#ifndef SITUATIE_H
#define SITUATIE_H

#include <map>
#include <vector>
#include <string>
#include "Baan.h"
#include "Voertuig.h"
#include "Verkeerslicht.h"
#include <iostream>

using namespace std;

class VerkeersSituatie {
private:
    map<string, Baan> banen;
    vector<Voertuig> voertuigen;
    vector<Verkeerslicht> verkeerslichten;

public:
    bool voegBaanToe(const Baan& baan);
    bool voegVoertuigToe(const Voertuig& voertuig);
    bool voegVerkeerslichtToe(const Verkeerslicht& verkeerslicht);
    bool verificeerConsistentie() const;
    void printInfo() const;
    
    // methodes om de simulatie te ondersteunen
    std::vector<Voertuig>& getVoertuigen() { return voertuigen; }
    const std::vector<Voertuig>& getVoertuigen() const { return voertuigen; }
    
    std::vector<Verkeerslicht>& getVerkeerslichten() { return verkeerslichten; }
    const std::vector<Verkeerslicht>& getVerkeerslichten() const { return verkeerslichten; }
    
    const std::map<string, Baan>& getBanen() const { return banen; }

    // Methode om een voertuig te verwijderen op een bepaalde plaats
    bool verwijderVoertuig(int index);
};

bool leesVerkeersSituatie(const string& bestandsnaam, VerkeersSituatie& situatie);

#endif // SITUATIE_H