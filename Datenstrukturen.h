#ifndef EXTREMECOMBAT_DATENSTRUKTUREN_H
#define EXTREMECOMBAT_DATENSTRUKTUREN_H

#include <string>
#include <vector>
#include <utility> // Für std::pair
#include "Charaktere.h" // Benötigt die Character-Definition

// --- Enum für Hauptmenü ---
// Hinzugefügt: Option für Würfelauswahl
enum class MainMenuOption {
    Start = 1,
    Info = 2,
    SelectDice = 3, // Neue Option
    Exit = 4,       // Exit ist jetzt 4
    Invalid         // Für ungültige Eingaben oder Fehler
};


// --- Datenstrukturen ---

// Klasse für einen Spieler (unverändert)
class Player {
private:
    std::string name;
    Character character_data;
    int defense_bonus = 0;

public:
    Player();
    Player(std::string n, Character character);
    std::string getName() const;
    const Character& getCharacterData() const;
    Character& getCharacterData();
    int getDefenseBonus() const;
    void setDefenseBonus(int bonus);
    void resetDefenseBonus();
};

// --- Funktionsdeklarationen ---

// Spiellogik
std::pair<Player, Player> selectCharacters(const std::vector<Character>& available_chars);

// Hilfsfunktionen (Utils)
// void initializeRandomSeed(); // Entfernt
// int rollD20(); // Entfernt
MainMenuOption getMainMenuInput(const std::string& prompt);
int getIntegerInputSimple(const std::string& prompt);
void sleepMilliseconds(int ms);


#endif //EXTREMECOMBAT_DATENSTRUKTUREN_H