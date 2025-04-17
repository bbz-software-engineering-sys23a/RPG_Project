#ifndef EXTREMECOMBAT_CHARAKTERE_H
#define EXTREMECOMBAT_CHARAKTERE_H

#include <string>
#include <vector>
#include <iostream>

// Struktur für Angriffsdaten (Name + Schaden).
struct AttackData {
    std::string name;
    int base_damage;

    // Konstruktor für AttackData.
    AttackData(std::string n, int dmg);
};

// Repräsentiert einen Spielcharakter.
class Character {
private:
    std::string name;
    int max_hp;
    int current_hp;
    AttackData attack1;
    AttackData attack2;
    std::string defense_action_text;
    int base_defense;
    int max_owp;
    int current_owp;

public:
    // Konstruktor für Character.
    Character(std::string n, int hp, int def,
              const AttackData& a1, const AttackData& a2,
              std::string def_text, int max_owp_val = 100);

    // Zeigt Charakter-Infos an.
    void displayInfo() const;
    // Zeigt Angriffsoptionen an.
    void displayAttackOptions() const;
    // Verarbeitet eingehenden Schaden.
    void takeDamage(int incoming_damage);
    // Prüft, ob der Charakter besiegt ist.
    bool isDefeated() const;

    // Fügt OWP hinzu.
    void addOwp(int amount);
    // Prüft, ob OWP für Kosten ausreichen.
    bool canAfford(int cost) const;
    // Verbraucht OWP (falls möglich).
    bool useOwp(int cost);

    // Getter-Methoden.
    std::string getName() const;
    int getHP() const;
    int getMaxHP() const;
    int getDefenseStat() const;
    int getOwp() const;
    int getMaxOwp() const;
    const AttackData& getAttack1() const;
    const AttackData& getAttack2() const;
    std::string getDefenseActionText() const;
};

// Initialisiert die Liste der verfügbaren Charaktere.
std::vector<Character> initializeCharacters();

#endif //EXTREMECOMBAT_CHARAKTERE_H