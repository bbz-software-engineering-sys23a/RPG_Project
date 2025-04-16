#include "Datenstrukturen.h" // Eigenen Header einbinden
#include <iostream>
#include <limits>
#include <chrono>
#include <thread>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <vector>   // Nötig für selectCharacters
#include <utility>  // Nötig für selectCharacters
#include <windows.h> // Für UTF-8 Darstellung

// --- Implementierung der Player Konstruktoren ---
// Benötigt die volle Definition von Character (aus Charaktere.h, via Datenstrukturen.h)
Player::Player() : name(""), character_data("Dummy", 0, 0, AttackData{"",0}, AttackData{"",0}, "") {}
Player::Player(std::string n, Character character) : name(std::move(n)), character_data(std::move(character)) {}


// --- Implementierung der Hilfsfunktionen (Utils) ---
namespace {
    std::mt19937 rng_ds; // Eigener Generator für diese Datei
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
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
    int value;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
             std::cout << "Fehler bei der Eingabe. Programm wird beendet." << std::endl;
             throw std::runtime_error("Fehler beim Lesen der Eingabe.");
        }
        if (std::cin.good()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cout << "Ungültige Eingabe. Bitte eine Zahl eingeben." << std::endl;
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
// Benötigt die Methoden von Character (aus Charaktere.h, via Datenstrukturen.h)
Player selectSinglePlayer(int playerNumber, const std::vector<Character>& available_chars) {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\n--- Spieler " << playerNumber << ": Charakterauswahl ---" << std::endl;
    for (size_t i = 0; i < available_chars.size(); ++i) {
        std::cout << i + 1 << ": " << available_chars[i].getName() << " (HP:" << available_chars[i].getHP() << ")" << std::endl;
    }
    int choice = -1;
    Character selected_char_copy("Dummy", 0, 0, AttackData{"",0}, AttackData{"",0}, "");
    bool confirmed = false;
    while (!confirmed) {
        choice = getIntegerInput("Wähle einen Charakter (Nummer): ");
        if (choice >= 1 && choice <= static_cast<int>(available_chars.size())) {
            selected_char_copy = available_chars[choice - 1];
            std::cout << "\nDu hast gewählt:" << std::endl;
            selected_char_copy.displayInfo(); // Ruft Methode aus Character auf
            char confirmation_input = ' ';
            while (confirmation_input != 'j' && confirmation_input != 'n') {
                 std::cout << "Bist du zufrieden? (j/n): ";
                 std::cin >> confirmation_input;
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 confirmation_input = std::tolower(confirmation_input);
                 if (confirmation_input != 'j' && confirmation_input != 'n') {
                    std::cout << "Bitte nur 'j' oder 'n' eingeben." << std::endl;
                 }
            }
            if (confirmation_input == 'j') confirmed = true;
            else std::cout << "Wähle erneut." << std::endl;
        } else {
            std::cout << "Ungültige Auswahl." << std::endl;
        }
    }
    std::string player_name;
    std::cout << "Gib den Namen für Spieler " << playerNumber << " ein: ";
    std::getline(std::cin, player_name);
    if (player_name.empty()) player_name = "Spieler " + std::to_string(playerNumber);
    return Player(player_name, selected_char_copy);
}

// Implementierung Charakterauswahl
std::pair<Player, Player> selectCharacters(const std::vector<Character>& available_chars) {
    if (available_chars.empty()) {
        throw std::runtime_error("Keine Charaktere zum Auswählen vorhanden!");
    }
    Player player1 = selectSinglePlayer(1, available_chars);
    Player player2 = selectSinglePlayer(2, available_chars);
    return std::make_pair(player1, player2);
}

// void startCombat(Player& player1, Player& player2) { ... } // Kampf ist entfernt

