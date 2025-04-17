#ifndef EXTREMECOMBAT_DATENSTRUKTUREN_H
#define EXTREMECOMBAT_DATENSTRUKTUREN_H

#include <string>
#include <vector>
#include <utility> // Für std::pair
#include "Charaktere.h" // Benötigt die Character-Definition

// --- Enum für Hauptmenü ---
enum class MainMenuOption {
    Start = 1,
    Info = 2,
    Exit = 3,
    Invalid // Für ungültige Eingaben oder Fehler
};


// --- Datenstrukturen ---

// Klasse für einen Spieler (vorher struct)
class Player {
private:
    std::string name;
    Character character_data; // Verwendet die Klasse aus Charaktere.h
    int defense_bonus = 0;    // Bonus für nächsten Verteidigungswurf

public:
    // Konstruktor Deklarationen
    Player(); // Default
    Player(std::string n, Character character);

    // Getter
    std::string getName() const;
    const Character& getCharacterData() const; // Gibt konstante Referenz zurück
    Character& getCharacterData();             // Gibt modifizierbare Referenz zurück (vorsichtig verwenden!)
    int getDefenseBonus() const;

    // Setter/Modifier (nur für defense_bonus benötigt)
    void setDefenseBonus(int bonus);
    void resetDefenseBonus(); // Setzt Bonus auf 0 zurück
};

// --- Funktionsdeklarationen ---

// Spiellogik
std::pair<Player, Player> selectCharacters(const std::vector<Character>& available_chars);

// Hilfsfunktionen (Utils)
void initializeRandomSeed();
int rollD20();
// Gibt jetzt MainMenuOption zurück, interne Logik in .cpp
MainMenuOption getMainMenuInput(const std::string& prompt);
// Einfache Integer-Eingabe für Charakterauswahl etc.
int getIntegerInputSimple(const std::string& prompt);
void sleepMilliseconds(int ms);


#endif //EXTREMECOMBAT_DATENSTRUKTUREN_H
