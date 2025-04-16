#include "Datenstrukturen.h" // Eigenen Header einbinden
#include <iostream>
#include <limits>
#include <chrono>
#include <thread>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <cctype>    // Für std::tolower
#include <vector>
#include <utility>

// --- Implementierung der Player Konstruktoren ---
Player::Player() : name(""), character_data("Dummy", 0, 0, AttackData{"",0}, AttackData{"",0}, "") {}
Player::Player(std::string n, Character character) : name(std::move(n)), character_data(std::move(character)) {}


// --- Implementierung der Hilfsfunktionen (Utils) ---
namespace {
    std::mt19937 rng_ds;
    bool seed_initialized_ds = false;
}

void initializeRandomSeed() {
    if (!seed_initialized_ds) {
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        rng_ds.seed(seed);
        seed_initialized_ds = true;
    }
}

int rollD20() {
    if (!seed_initialized_ds) {
       initializeRandomSeed();
    }
    std::uniform_int_distribution<int> dist(1, 20);
    return dist(rng_ds);
}

int getIntegerInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
             std::cout << "Fehler bei der Eingabe. Programm wird beendet." << std::endl;
             throw std::runtime_error("Fehler beim Lesen der Eingabe.");
        }
        if (std::cin.good() && std::cin.peek() == '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cout << "Ungueltige Eingabe. Bitte nur eine Zahl eingeben." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void sleepMilliseconds(int ms) {
     if (ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
     }
}

// --- Implementierung der Spiellogik-Funktionen (ohne Kampf) ---

// Hilfsfunktion für die Charakterauswahl eines Spielers
Player selectSinglePlayer(int playerNumber, const std::vector<Character>& available_chars) {
    std::cout << "\n--- Spieler " << playerNumber << ": Charakterauswahl ---" << std::endl;
    for (size_t i = 0; i < available_chars.size(); ++i) {
        std::cout << i + 1 << ": " << available_chars[i].getName() << " (HP:" << available_chars[i].getHP() << ")" << std::endl;
    }
    int choice = -1;
    Character selected_char_copy("Dummy", 0, 0, AttackData{"",0}, AttackData{"",0}, "");
    bool char_confirmed = false; // Umbenannt zur Klarheit

    // Schleife für Charakterwahl + Bestätigung
    while (!char_confirmed) {
        choice = getIntegerInput("Waehle einen Charakter (Nummer): ");
        if (choice >= 1 && choice <= static_cast<int>(available_chars.size())) {
            selected_char_copy = available_chars[choice - 1];
            std::cout << "\nDu hast gewaehlt:" << std::endl;
            selected_char_copy.displayInfo();
            char confirmation_input = ' ';
            while (confirmation_input != 'j' && confirmation_input != 'n') {
                 std::cout << "Bist du mit dem Charakter zufrieden? (j/n): "; // Text angepasst
                 std::cin >> confirmation_input;
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 confirmation_input = std::tolower(confirmation_input);
                 if (confirmation_input != 'j' && confirmation_input != 'n') {
                    std::cout << "Bitte nur 'j' oder 'n' eingeben." << std::endl;
                 }
            }
            if (confirmation_input == 'j') {
                 char_confirmed = true; // Charakter bestätigt
            } else {
                 std::cout << "Waehle erneut." << std::endl;
            }
        } else {
             std::cout << "Ungueltige Auswahl. Bitte eine Nummer zwischen 1 und "
<< available_chars.size() << " waehlen." << std::endl;
        }
    } // Ende Charakterwahl-Schleife

    // --- NEU: Schleife für Namenseingabe + Bestätigung ---
    std::string player_name;
    bool name_confirmed = false;
    while (!name_confirmed) {
        std::cout << "Gib den Namen fuer Spieler " << playerNumber << " ein: ";
        std::getline(std::cin, player_name);
        // Standardnamen, falls nichts eingegeben wurde
        if (player_name.empty()) {
            player_name = "Spieler " + std::to_string(playerNumber);
            std::cout << "Kein Name eingegeben, Standardname '" << player_name << "' wird verwendet." << std::endl;
            // Optional: Hier direkt bestätigen oder auch nachfragen
            // name_confirmed = true; // Einfachste Variante: Standardname wird akzeptiert
        }

        // Bestätigung für den Namen einholen (auch für Standardnamen, falls nicht oben direkt bestätigt)
        // if (!name_confirmed) { // Nur fragen, wenn nicht schon durch Standardnamen bestätigt
            std::cout << "Du hast den Namen '" << player_name << "' gewaehlt." << std::endl;
            char name_confirmation_input = ' ';
            while (name_confirmation_input != 'j' && name_confirmation_input != 'n') {
                std::cout << "Ist der Name korrekt? (j/n): ";
                std::cin >> name_confirmation_input;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wichtig! Buffer leeren
                name_confirmation_input = std::tolower(name_confirmation_input);
                if (name_confirmation_input != 'j' && name_confirmation_input != 'n') {
                    std::cout << "Bitte nur 'j' oder 'n' eingeben." << std::endl;
                }
            }

            if (name_confirmation_input == 'j') {
                name_confirmed = true; // Name bestätigt, Schleife verlassen
            } else {
                std::cout << "Bitte gib den Namen erneut ein." << std::endl;
                // Die Schleife beginnt von vorne mit der Namensabfrage
            }
        // } // Ende if (!name_confirmed)
    } // Ende der while(!name_confirmed) Schleife
    // --- Ende NEUE Schleife ---


    // Spieler-Objekt mit bestätigtem Charakter und Namen zurückgeben
    return Player(player_name, selected_char_copy);
}

// Implementierung Charakterauswahl
std::pair<Player, Player> selectCharacters(const std::vector<Character>& available_chars) {
    if (available_chars.empty()) {
        throw std::runtime_error("Keine Charaktere zum Auswaehlen vorhanden!");
    }
    Player player1 = selectSinglePlayer(1, available_chars);
    Player player2 = selectSinglePlayer(2, available_chars);
    return std::make_pair(player1, player2);
}

// void startCombat(Player& player1, Player& player2) { ... } // Kampf ist hier nicht implementiert