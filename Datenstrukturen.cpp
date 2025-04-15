//
// Created by felix on 15.04.2025.
//
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>  // Für rand() und srand()
#include <ctime>    // Für time()

// Enum für die Bewegungen
enum class Move {
    STANDARD_ATTACK
};

// Klasse für die Charaktere
class Character {
private:
    std::string name;  // Name des Charakters
    int hp;            // Hit Points
    int attack;        // Basis-Angriffswert
    int defense;       // Basis-Verteidigungswert

public:
    // Konstruktor
    Character(std::string name, int hp, int attack, int defense)
        : name(name), hp(hp), attack(attack), defense(defense) {}

    // Getter für den Namen
    std::string getName() const {
        return name;
    }

    // Getter für HP
    int getHP() const {
        return hp;
    }

    // Schadensberechnung
    void takeDamage(int damage) {
        hp = std::max(0, hp - damage);  // HP kann nicht negativ werden
    }

    // Angriffsmethode
    int attackEnemy(Character& enemy) {
        int attackRoll = rand() % 20 + 1;  // D20-Wurf
        int damage = attack + attackRoll - enemy.defenseRoll();
        if (damage > 0) {
            enemy.takeDamage(damage);
            return damage;
        }
        return 0;
    }

    // Verteidigungswurf
    int defenseRoll() {
        return rand() % 20 + 1;  // D20-Wurf für Verteidigung
    }
};

// Funktion zur Bestimmung des ersten Spielers
int determineFirstPlayer() {
    return rand() % 2 + 1;  // Zufällig 1 oder 2 zurückgeben
}

int main() {
    srand(static_cast<unsigned int>(time(0)));  // Zufallszahlengenerator initialisieren

    // Beispiel für die Charaktere
    std::vector<Character> characters = {
        Character("Charakter 1", 100, 10, 5),
        Character("Charakter 2", 90, 12, 4),
        Character("Charakter 3", 95, 8, 6),
        Character("Charakter 4", 80, 14, 3),
        Character("Charakter 5", 85, 11, 5),
        Character("Charakter 6", 75, 15, 2)
    };

    int firstPlayer = determineFirstPlayer();
    std::cout << "Spieler " << firstPlayer << " beginnt den Kampf!" << std::endl;

    return 0;
}