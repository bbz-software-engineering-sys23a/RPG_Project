#ifndef EXTREMECOMBAT_DATENSTRUKTUREN_H
#define EXTREMECOMBAT_DATENSTRUKTUREN_H

#include <string>
#include <vector>
#include <utility>
#include "Charaktere.h"

// Definiert die Optionen im Hauptmenü.
enum class MainMenuOption {
    Start = 1,
    Info = 2,
    SelectDice = 3,
    Exit = 4,
    Invalid
};

// Repräsentiert einen Spieler.
class Player {
private:
    std::string name;
    Character character_data;
    int defense_bonus = 0;

public:
    // Konstruktoren.
    Player();
    Player(std::string n, Character character);

    // Getter.
    std::string getName() const;
    const Character& getCharacterData() const;
    Character& getCharacterData();
    int getDefenseBonus() const;

    // Setter/Modifier.
    void setDefenseBonus(int bonus);
    void resetDefenseBonus();
};

// --- Funktionsdeklarationen ---

// Startet die Charakterauswahl für beide Spieler.
std::pair<Player, Player> selectCharacters(const std::vector<Character>& available_chars);

// Liest die Hauptmenü-Auswahl ein.
MainMenuOption getMainMenuInput(const std::string& prompt);
// Liest eine einfache Ganzzahl ein.
int getIntegerInputSimple(const std::string& prompt);
// Pausiert das Programm.
void sleepMilliseconds(int ms);

#endif //EXTREMECOMBAT_DATENSTRUKTUREN_H
