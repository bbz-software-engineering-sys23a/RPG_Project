#ifndef EXTREMECOMBAT_CHARAKTERE_H // Angepasster Include Guard
#define EXTREMECOMBAT_CHARAKTERE_H

#include <string>
#include <vector>

// Struktur für Angriffsdaten (Name + Schaden)
struct AttackData {
    std::string name;
    int base_damage; // Basis-Schaden des Angriffs

    // Konstruktor Deklaration
    AttackData(std::string n, int dmg);
};

// Charakter-Klasse
class Character {
public:
    std::string name;
    int max_hp;
    int current_hp;
    AttackData attack1;
    AttackData attack2;
    std::string defense_action_text;
    int base_defense;

    // Konstruktor Deklaration
    Character(std::string n, int hp, int def,
              const AttackData& a1, const AttackData& a2,
              std::string def_text);

    // Methoden Deklarationen
    void displayInfo() const;
    void displayAttackOptions() const;
    void takeDamage(int incoming_damage);
    std::string getName() const;
    int getHP() const;
    int getDefenseStat() const;
    bool isDefeated() const;
};

// Funktionsdeklaration für die Initialisierung
std::vector<Character> initializeCharacters();

#endif //EXTREMECOMBAT_CHARAKTERE_H
