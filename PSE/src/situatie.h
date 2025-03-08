//
// Created by senne on 27/02/2025.
//

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
};

bool leesVerkeersSituatie(const string& bestandsnaam, VerkeersSituatie& situatie);

#endif // SITUATIE_H
