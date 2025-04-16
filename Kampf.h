#ifndef EXTREMECOMBAT_KAMPF_H
#define EXTREMECOMBAT_KAMPF_H

#include "Datenstrukturen.h" // Benötigt Player-Struct (und indirekt Character)

// Deklaration der Funktion, die den Kampf startet und durchführt
void startCombat(Player& player1, Player& player2);

// !!! NEU: Deklaration für die Siegesanimation !!!
void displayVictoryFireworks();

#endif //EXTREMECOMBAT_KAMPF_H

