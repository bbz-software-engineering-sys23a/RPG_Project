#ifndef EXTREMECOMBAT_KAMPF_H
#define EXTREMECOMBAT_KAMPF_H

#include "Datenstrukturen.h" // Benötigt Player-Klasse (und indirekt Character)

// --- Enum für Kampfaktionen ---
enum class CombatAction {
    Attack1 = 1,
    Attack2 = 2,
    Defend = 3,
    Exit,    // Für den 'Exit' Befehl
    Invalid  // Für ungültige Eingaben oder Fehler
};


// Deklaration der Funktion, die den Kampf startet und durchführt
void startCombat(Player& player1, Player& player2);

// Deklaration für die Siegesanimation (Feuerwerk)
void displayVictoryFireworks();

#endif //EXTREMECOMBAT_KAMPF_H

