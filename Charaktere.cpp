#include "Charaktere.h" // Eigenen Header einbinden
#include <vector>
#include <string>
#include <iostream>  // Für displayInfo etc.
#include <algorithm> // Für std::max in takeDamage
#include <windows.h> // Für UTF-8 Darstellung

// --- Implementierung AttackData Konstruktor ---
AttackData::AttackData(std::string n, int dmg) : name(std::move(n)), base_damage(dmg) {}

// --- Implementierung der Character Methoden ---
Character::Character(std::string n, int hp, int def,
                     const AttackData& a1, const AttackData& a2,
                     std::string def_text)
    : name(std::move(n)), max_hp(hp), current_hp(hp),
      attack1(a1), attack2(a2),
      defense_action_text(std::move(def_text)), base_defense(def) {}

void Character::displayInfo() const {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Name: " << name << " (HP: " << current_hp << "/" << max_hp << ", DEF: " << base_defense << ")" << std::endl;
    std::cout << "  Angriff 1: " << attack1.name << " (Basis DMG: " << attack1.base_damage << ")" << std::endl;
    std::cout << "  Angriff 2: " << attack2.name << " (Basis DMG: " << attack2.base_damage << ")" << std::endl;
    std::cout << "  Verteidigungstext: \"" << defense_action_text << "\"" << std::endl;
}

void Character::displayAttackOptions() const {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
     std::cout << "  1: " << attack1.name << " (Basis DMG: " << attack1.base_damage << ")" << std::endl;
     std::cout << "  2: " << attack2.name << " (Basis DMG: " << attack2.base_damage << ")" << std::endl;
}

void Character::takeDamage(int incoming_damage) {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
    int actual_damage = std::max(0, incoming_damage - base_defense);
    current_hp = std::max(0, current_hp - actual_damage);
    if (actual_damage > 0) {
        std::cout << name << " erleidet " << actual_damage << " tatsächlichen Schaden!" << std::endl;
    } else {
        std::cout << name << " blockt den Angriff komplett ab! (0 Schaden)" << std::endl;
    }
}

std::string Character::getName() const { return name; }
int Character::getHP() const { return current_hp; }
int Character::getDefenseStat() const { return base_defense; }
bool Character::isDefeated() const { return current_hp <= 0; }


// --- Implementierung der Initialisierungsfunktion ---
std::vector<Character> initializeCharacters() {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
    std::vector<Character> all_characters;
    // Beispielwerte - passe HP, DEF, DMG nach Bedarf an!
    const int HP_LOW = 80, HP_MID = 100, HP_HIGH = 120, HP_NORRIS = 200, HP_RUEHL = 150;
    const int DEF_LOW = 3, DEF_MID = 5, DEF_HIGH = 7, DEF_NORRIS = 10, DEF_RUEHL = 8;
    const int DMG1_LOW = 8, DMG1_MID = 10, DMG1_HIGH = 12, DMG1_NORRIS = 20;
    const int DMG2_LOW = 14, DMG2_MID = 18, DMG2_HIGH = 22, DMG2_NORRIS = 35;

    all_characters.emplace_back("Goblin", HP_LOW, DEF_LOW, AttackData{"Dolchstich von hinten", DMG1_LOW}, AttackData{"Brunnen Vergiften", DMG2_LOW}, "Stellt sich als Opfer dar");
    all_characters.emplace_back("MG-Opa", HP_MID, DEF_MID, AttackData{"Schlag mit Gehstock", DMG1_LOW}, AttackData{"Ballert mit MG", DMG2_HIGH}, "Rückzug in Schützenbunker");
    all_characters.emplace_back("Schweinebärmann", HP_HIGH, DEF_MID, AttackData{"Al Gore sagt, es ist voll ernte", DMG1_MID}, AttackData{"Prankenhieb", DMG2_MID}, "Vortrag über Umweltschutz");
    all_characters.emplace_back("Redneck", HP_MID, DEF_LOW, AttackData{"Sie klauen unsere Jobs", DMG1_MID}, AttackData{"AR-15", DMG2_HIGH}, "Cousin/Bruder kommt");
    all_characters.emplace_back("Maggus Rühl", HP_RUEHL, DEF_RUEHL, AttackData{"335kg Benchpress", DMG1_HIGH}, AttackData{"Protein Furz", DMG2_LOW}, "De Bob Tschigarillo hilft");
    all_characters.emplace_back("Rainer Winkler", HP_LOW, DEF_LOW, AttackData{"Prügel raus Hauen", DMG1_MID}, AttackData{"Der Schwarze Luan kommt", DMG2_MID}, "Lügenlord");
    all_characters.emplace_back("Chuck Norris", HP_NORRIS, DEF_NORRIS, AttackData{"Schmeisst Parties 500 Meter weit", DMG1_NORRIS}, AttackData{"Roundhouse Kick", DMG2_NORRIS}, "Hatt als Kind Katholische Priester missbracht");
    all_characters.emplace_back("Zinedine Zidane", HP_MID, DEF_MID, AttackData{"Faust in die Fresse", DMG1_MID}, AttackData{"Kopfstuss", DMG2_HIGH}, "Doping");
    all_characters.emplace_back("Drachen Haider", HP_LOW, DEF_LOW, AttackData{"Hausbesuch", DMG1_MID}, AttackData{"Doxing", DMG2_MID}, "Anzeige ist raus");

    return all_characters;
}
