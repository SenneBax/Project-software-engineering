//
// Created by senne on 27/02/2025.
//

#ifndef VOERTUIG_H
#define VOERTUIG_H

#include <string>
using namespace std;

class Voertuig {
private:
    string baan;
    int positie;

public:
    Voertuig(const string& baan, int positie);
    
    string getBaan() const;
    int getPositie() const;
};

#endif // VOERTUIG_H