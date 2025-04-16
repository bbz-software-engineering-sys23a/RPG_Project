#ifndef EXTREMECOMBAT_CHARAKTERE_H
#define EXTREMECOMBAT_CHARAKTERE_H

#include <string>
#include <vector>
#include <iostream> // Nur für Deklarationen benötigt, wenn Methoden inline wären

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

    // OWP (Mana) System
    int max_owp;
    int current_owp; // "Ominöses weisses Pulver"

    // Konstruktor Deklaration (angepasst für OWP)
    Character(std::string n, int hp, int def,
              const AttackData& a1, const AttackData& a2,
              std::string def_text, int max_owp_val = 100); // Standardwert für max OWP

    // Methoden Deklarationen
    void displayInfo() const;
    void displayAttackOptions() const;
    void takeDamage(int incoming_damage);
    std::string getName() const;
    int getHP() const;
    int getDefenseStat() const;
    bool isDefeated() const;

    // OWP Methoden Deklarationen
    void addOwp(int amount);             // Fügt OWP hinzu (bis max_owp)
    bool canAfford(int cost) const;      // Prüft, ob genug OWP vorhanden ist
    bool useOwp(int cost);               // Verbraucht OWP, gibt true zurück bei Erfolg
    int getOwp() const;                  // Gibt aktuellen OWP-Stand zurück

};

// Funktionsdeklaration für die Initialisierung
std::vector<Character> initializeCharacters();

#endif //EXTREMECOMBAT_CHARAKTERE_H
