#ifndef EXTREMECOMBAT_KAMPF_H
#define EXTREMECOMBAT_KAMPF_H

#include "Datenstrukturen.h" // Benötigt Player-Klasse
#include "Dice.h"            // Benötigt Dice Basisklasse

// --- Enum für Kampfaktionen ---
// (Unverändert von vorheriger Refaktorierung)
enum class CombatAction {
    Attack1 = 1,
    Attack2 = 2,
    Defend = 3,
    Exit,
    Invalid
};


// Deklaration der Funktion, die den Kampf startet und durchführt
// Nimmt jetzt eine Referenz auf ein Dice-Objekt entgegen
void startCombat(Player& player1, Player& player2, const Dice& dice);

// Deklaration für die Siegesanimation (unverändert)
void displayVictoryFireworks();

#endif //EXTREMECOMBAT_KAMPF_H