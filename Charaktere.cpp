#include "Charaktere.h" // Eigenen Header einbinden
#include <vector>
#include <string>
#include <iostream>
#include <algorithm> // Für std::max, std::min

// --- Implementierung AttackData Konstruktor ---
AttackData::AttackData(std::string n, int dmg) : name(std::move(n)), base_damage(dmg) {}

// --- Implementierung der Character Methoden ---

// Konstruktor Implementierung (mit OWP Initialisierung)
Character::Character(std::string n, int hp, int def,
                     const AttackData& a1, const AttackData& a2,
                     std::string def_text, int max_owp_val /*=100*/) // Standardwert aus Header
    : name(std::move(n)), max_hp(hp), current_hp(hp),
      attack1(a1), attack2(a2),
      defense_action_text(std::move(def_text)), base_defense(def),
      max_owp(max_owp_val), current_owp(0) /* Startet mit 0 OWP */ {}

// Info-Anzeige Implementierung (zeigt jetzt OWP an)
void Character::displayInfo() const {
    std::cout << "Name: " << name
              << " (HP: " << current_hp << "/" << max_hp
              << ", DEF: " << base_defense
              << ", OWP: " << current_owp << "/" << max_owp << ")" // OWP Anzeige hinzugefügt
              << std::endl;
    std::cout << "  Angriff 1: " << attack1.name << " (Basis DMG: " << attack1.base_damage << ")" << std::endl;
    std::cout << "  Angriff 2: " << attack2.name << " (Basis DMG: " << attack2.base_damage << ")" << std::endl;
    std::cout << "  Verteidigungstext: \"" << defense_action_text << "\"" << std::endl;
}

// Angriffsoptionen Implementierung (zeigt jetzt OWP-Kosten an)
void Character::displayAttackOptions() const {
     std::cout << "  1: " << attack1.name << " (DMG: " << attack1.base_damage << ", Kosten: 0 OWP)" << std::endl; // Angriff 1 kostet nichts
     std::cout << "  2: " << attack2.name << " (DMG: " << attack2.base_damage << ", Kosten: " << attack2.base_damage << " OWP)" << std::endl; // Angriff 2 kostet Basis-Schaden
}

// Schaden nehmen Implementierung (unverändert)
void Character::takeDamage(int incoming_damage) {
    int actual_damage = std::max(0, incoming_damage - base_defense);
    current_hp = std::max(0, current_hp - actual_damage);
    if (actual_damage > 0) {
        std::cout << name << " erleidet " << actual_damage << " tatsaechlichen Schaden!" << std::endl;
    } else {
        std::cout << name << " blockt den Angriff komplett ab! (0 Schaden)" << std::endl;
    }
}

// Getter Implementierungen (unverändert)
std::string Character::getName() const { return name; }
int Character::getHP() const { return current_hp; }
int Character::getDefenseStat() const { return base_defense; }
bool Character::isDefeated() const { return current_hp <= 0; }

// --- Implementierung der OWP Methoden ---
int Character::getOwp() const {
    return current_owp;
}

bool Character::canAfford(int cost) const {
    return current_owp >= cost;
}

void Character::addOwp(int amount) {
    if (amount > 0) {
        current_owp += amount;
        // Stelle sicher, dass OWP nicht über das Maximum steigt
        current_owp = std::min(max_owp, current_owp);
    }
}

bool Character::useOwp(int cost) {
    if (canAfford(cost)) {
        current_owp -= cost;
        return true; // Erfolgreich verbraucht
    }
    return false; // Nicht genug OWP
}


// --- Implementierung der Initialisierungsfunktion ---
// (Hier muss der Konstruktoraufruf angepasst werden, falls der Standardwert für max_owp nicht passt)
std::vector<Character> initializeCharacters() {
    std::vector<Character> all_characters;
    // Beispielwerte - passe HP, DEF, DMG, OWP_MAX nach Bedarf an!
    const int HP_LOW = 80, HP_MID = 100, HP_HIGH = 120, HP_NORRIS = 200, HP_RUEHL = 150;
    const int DEF_LOW = 3, DEF_MID = 5, DEF_HIGH = 7, DEF_NORRIS = 10, DEF_RUEHL = 8;
    const int DMG1_LOW = 8, DMG1_MID = 10, DMG1_HIGH = 12, DMG1_NORRIS = 20;
    const int DMG2_LOW = 14, DMG2_MID = 18, DMG2_HIGH = 22, DMG2_NORRIS = 35;
    const int OWP_MAX_DEFAULT = 100; // Max OWP für die meisten

    // Format: Name, HP, Basis-DEF, Angriff1(Name, Schaden), Angriff2(Name, Schaden), Verteidigungstext, [Optional: Max OWP]
    all_characters.emplace_back("Goblin", HP_LOW, DEF_LOW, AttackData{"Dolchstich von hinten", DMG1_LOW}, AttackData{"Brunnen Vergiften", DMG2_LOW}, "Stellt sich als Opfer dar", OWP_MAX_DEFAULT);
    all_characters.emplace_back("MG-Opa", HP_MID, DEF_MID, AttackData{"Schlag mit Gehstock", DMG1_LOW}, AttackData{"Ballert mit MG", DMG2_HIGH}, "Rückzug in Schützenbunker", OWP_MAX_DEFAULT);
    all_characters.emplace_back("Schweinebärmann", HP_HIGH, DEF_MID, AttackData{"Al Gore sagt, es ist voll ernte", DMG1_MID}, AttackData{"Prankenhieb", DMG2_MID}, "Vortrag über Umweltschutz", OWP_MAX_DEFAULT);
    all_characters.emplace_back("Redneck", HP_MID, DEF_LOW, AttackData{"Sie klauen unsere Jobs", DMG1_MID}, AttackData{"AR-15", DMG2_HIGH}, "Cousin/Bruder kommt", OWP_MAX_DEFAULT);
    all_characters.emplace_back("Maggus Rühl", HP_RUEHL, DEF_RUEHL, AttackData{"335kg Benchpress", DMG1_HIGH}, AttackData{"Protein Furz", DMG2_LOW}, "De Bob Tschigarillo hilft", OWP_MAX_DEFAULT + 20); // Mehr OWP?
    all_characters.emplace_back("Rainer Winkler", HP_LOW, DEF_LOW, AttackData{"Prügel raus Hauen", DMG1_MID}, AttackData{"Der Schwarze Luan kommt", DMG2_MID}, "Lügenlord", OWP_MAX_DEFAULT - 20); // Weniger OWP?
    all_characters.emplace_back("Chuck Norris", HP_NORRIS, DEF_NORRIS, AttackData{"Schmeisst Parties 500 Meter weit", DMG1_NORRIS}, AttackData{"Roundhouse Kick", DMG2_NORRIS}, "Hatt als Kind Katholische Priester missbracht", OWP_MAX_DEFAULT * 2); // Unendlich OWP? ;)
    all_characters.emplace_back("Zinedine Zidane", HP_MID, DEF_MID, AttackData{"Faust in die Fresse", DMG1_MID}, AttackData{"Kopfstuss", DMG2_HIGH}, "Doping", OWP_MAX_DEFAULT);
    all_characters.emplace_back("Drachen Haider", HP_LOW, DEF_LOW, AttackData{"Hausbesuch", DMG1_MID}, AttackData{"Doxing", DMG2_MID}, "Anzeige ist raus", OWP_MAX_DEFAULT);

    return all_characters;
}
