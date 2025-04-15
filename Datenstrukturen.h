#ifndef EXTREMECOMBAT_DATENSTRUKTUREN_H
#define EXTREMECOMBAT_DATENSTRUKTUREN_H

#include <string>
#include <vector>
#include <utility> // Für std::pair
#include "Charaktere.h" // !!! Wichtig: Benötigt jetzt die Character-Definition !!!

// --- Datenstrukturen ---

// Struktur für einen Spieler
struct Player {
    std::string name;
    Character character_data; // Verwendet die Klasse aus Charaktere.h

    // Konstruktor Deklarationen
    Player();
    Player(std::string n, Character character);
};

// --- Funktionsdeklarationen ---

// Spiellogik
std::pair<Player, Player> selectCharacters(const std::vector<Character>& available_chars);
// void startCombat(Player& player1, Player& player2); // Kampf ist entfernt

// Hilfsfunktionen (Utils)
void initializeRandomSeed();
int rollD20();
int getIntegerInput(const std::string& prompt);
void sleepMilliseconds(int ms);


#endif //EXTREMECOMBAT_DATENSTRUKTUREN_H
