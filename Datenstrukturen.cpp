#include "Datenstrukturen.h" // Eigenen Header einbinden
#include <iostream>
#include <limits>
#include <chrono>
#include <thread>
#include <random>
#include <stdexcept>
#include <algorithm> // Für std::max
#include <cctype>    // Für std::tolower
#include <vector>
#include <utility>   // Für std::pair, std::move

// --- Implementierung der Player Methoden ---
// (Keine Änderungen hier)
Player::Player() : name(""), character_data("Dummy", 0, 0, AttackData{"",0}, AttackData{"",0}, ""), defense_bonus(0) {}
Player::Player(std::string n, Character character) : name(std::move(n)), character_data(std::move(character)), defense_bonus(0) {}
std::string Player::getName() const { return name; }
const Character& Player::getCharacterData() const { return character_data; }
Character& Player::getCharacterData() { return character_data; }
int Player::getDefenseBonus() const { return defense_bonus; }
void Player::setDefenseBonus(int bonus) { defense_bonus = std::max(0, bonus); }
void Player::resetDefenseBonus() { defense_bonus = 0; }


// --- Implementierung der Hilfsfunktionen (Utils) ---
namespace { // Anonyme Namespaces für interne Variablen/Funktionen
    // Interne Hilfsfunktion für sichere Zahleneingabe (unverändert)
    int getIntegerInputInternal(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;
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
} // Ende anonymer Namespace

// Einfache Integer-Eingabe für externe Nutzung (unverändert)
int getIntegerInputSimple(const std::string& prompt) {
    return getIntegerInputInternal(prompt);
}

// void initializeRandomSeed(); // Entfernt

// int rollD20(); // Entfernt

// Angepasste Funktion für Menü-Eingabe mit Enum-Rückgabe
MainMenuOption getMainMenuInput(const std::string& prompt) {
    int value = getIntegerInputInternal(prompt); // Nutzt interne Funktion
    switch (value) {
        case 1: return MainMenuOption::Start;
        case 2: return MainMenuOption::Info;
        case 3: return MainMenuOption::SelectDice; // Option 3 ist jetzt Würfelwahl
        case 4: return MainMenuOption::Exit;       // Option 4 ist jetzt Exit
        default: return MainMenuOption::Invalid;
    }
}

// sleepMilliseconds (unverändert)
void sleepMilliseconds(int ms) {
     if (ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
     }
}

// --- Implementierung der Spiellogik-Funktionen (ohne Kampf) ---
// selectSinglePlayer (unverändert)
Player selectSinglePlayer(int playerNumber, const std::vector<Character>& available_chars) {
    std::cout << "\n--- Spieler " << playerNumber << ": Charakterauswahl ---" << std::endl;
    for (size_t i = 0; i < available_chars.size(); ++i) {
        std::cout << i + 1 << ": " << available_chars[i].getName() << " (HP:" << available_chars[i].getHP() << ")" << std::endl;
    }
    int choice_idx = -1;
    Character selected_char_copy("Dummy", 0, 0, AttackData{"",0}, AttackData{"",0}, "");
    bool char_confirmed = false;

    while (!char_confirmed) {
        int choice_num = getIntegerInputSimple("Waehle einen Charakter (Nummer): ");
        choice_idx = choice_num - 1;

        if (choice_idx >= 0 && choice_idx < static_cast<int>(available_chars.size())) {
            selected_char_copy = available_chars[choice_idx];
            std::cout << "\nDu hast gewaehlt:" << std::endl;
            selected_char_copy.displayInfo();

            char confirmation_input = ' ';
            while (confirmation_input != 'j' && confirmation_input != 'n') {
                 std::cout << "Bist du mit dem Charakter zufrieden? (j/n): ";
                 std::cin >> confirmation_input;
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 confirmation_input = std::tolower(confirmation_input);
                 if (confirmation_input != 'j' && confirmation_input != 'n') {
                    std::cout << "Bitte nur 'j' oder 'n' eingeben." << std::endl;
                 }
            }
            if (confirmation_input == 'j') {
                 char_confirmed = true;
            } else {
                 std::cout << "Waehle erneut." << std::endl;
            }
        } else {
             std::cout << "Ungueltige Auswahl. Bitte eine Nummer zwischen 1 und "
                       << available_chars.size() << " waehlen." << std::endl;
        }
    }

    std::string player_name_input;
    bool name_confirmed = false;
    while (!name_confirmed) {
        std::cout << "Gib den Namen fuer Spieler " << playerNumber << " ein: ";
        if (std::cin.peek() == '\n') {
             std::cin.ignore();
        }
        std::getline(std::cin, player_name_input);
        if (player_name_input.empty()) {
            player_name_input = "Spieler " + std::to_string(playerNumber);
            std::cout << "Kein Name eingegeben, Standardname '" << player_name_input << "' wird verwendet." << std::endl;
        }

        std::cout << "Du hast den Namen '" << player_name_input << "' gewaehlt." << std::endl;
        char name_confirmation_input = ' ';
        while (name_confirmation_input != 'j' && name_confirmation_input != 'n') {
            std::cout << "Ist der Name korrekt? (j/n): ";
            std::cin >> name_confirmation_input;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            name_confirmation_input = std::tolower(name_confirmation_input);
            if (name_confirmation_input != 'j' && name_confirmation_input != 'n') {
                std::cout << "Bitte nur 'j' oder 'n' eingeben." << std::endl;
            }
        }

        if (name_confirmation_input == 'j') {
            name_confirmed = true;
        } else {
            std::cout << "Bitte gib den Namen erneut ein." << std::endl;
        }
    }
    return Player(player_name_input, selected_char_copy);
}

// selectCharacters (unverändert)
std::pair<Player, Player> selectCharacters(const std::vector<Character>& available_chars) {
    if (available_chars.empty()) {
        throw std::runtime_error("Keine Charaktere zum Auswaehlen vorhanden!");
    }
    Player player1 = selectSinglePlayer(1, available_chars);
    Player player2 = selectSinglePlayer(2, available_chars);
    return std::make_pair(player1, player2);
}
