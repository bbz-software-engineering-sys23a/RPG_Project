#include "Charaktere.h"
#include <iostream>

Character::Character(std::string n, Attack a1, Attack a2, Defense def)
    : name(n), attack1(a1), attack2(a2), defense(def) {}

void Character::showInfo() const {
    std::cout << "Name: " << name << std::endl;
    showAsciiArt();
    std::cout << "Angriff 1: " << attack1.name << std::endl;
    std::cout << "Angriff 2: " << attack2.name << std::endl;
    std::cout << "Verteidigung: " << defense.name << std::endl;
}

std::string Character::getName() const { return name; }

void Character::showAsciiArt() const {
    if (name == "Goblin") {
        std::cout << R"(
     ,      ,
    /(.-""-.)\
    |\  \/  /|
    \ )    ( /
    /        \
    \_/\__/\
    _|    |_
        )" << std::endl;
    } else if (name == "MG-Opa") {
        std::cout << R"(
    (o_o)
    <)   )  ALTE SCHULE!
     /    \
        )" << std::endl;
    } else if (name == "Schweinebärmann") {
        std::cout << R"(
   /\___/\
  ( o   o )
  (  =^=  )
  (        )
  (         )
  (          ))))))))
        )" << std::endl;
    } else if (name == "Redneck") {
        std::cout << R"(
     ___
    / _ \
   | | | |
   | |_| |
    \___/  *spits*
        )" << std::endl;
    } else if (name == "Maggus Rühl") {
        std::cout << R"(
     (o_o)
    | GAINS |
    |______|
        )" << std::endl;
    } else if (name == "Rainer Winkler") {
        std::cout << R"(
     [_] MEINUNG
     |  ===>
        )" << std::endl;
    } else if (name == "Chuck Norris") {
        std::cout << R"(
    (o_o)
     |||||||||
    /========\
     Roundhouse Kick
        )" << std::endl;
    } else if (name == "Zinedine Zidane") {
        std::cout << R"(
     O==(o_o)==O
       //     \\
     Kopfstoß aktiviert!
        )" << std::endl;
    } else if (name == "Drachen Haider") {
        std::cout << R"(
    (o_o)
   /|\  Anzeige ist raus
    |
   / \
        )" << std::endl;
    }
}

Character chooseCharacter(const std::vector<Character>& characters, int playerNumber) {
    int choice = -1;
    char confirm = 'n';

    do {
        std::cout << "Spieler " << playerNumber << ", wähle deinen Charakter:\n";
        for (size_t i = 0; i < characters.size(); ++i) {
            std::cout << i + 1 << ". " << characters[i].getName() << std::endl;
        }

        std::cout << "Gib eine Zahl ein: ";
        std::cin >> choice;

        if (choice >= 1 && choice <= static_cast<int>(characters.size())) {
            characters[choice - 1].showInfo();
            std::cout << "Willst du diesen Charakter wählen? (j/n): ";
            std::cin >> confirm;
        }
    } while (confirm != 'j' && confirm != 'J');

    return characters[choice - 1];
}

void showCharacterInfos(const std::vector<Character>& characters) {
    std::cout << "\n==== Charaktere Infos ====" << std::endl;
    for (const auto& character : characters) {
        character.showInfo();
        std::cout << "-----------------------------\n";
    }
}

void startCharacterSelection(const std::vector<Character>& characters, Character& player1, Character& player2) {
    bool exit = false;
    while (!exit) {
        std::cout << "\n===== HAUPTMENÜ =====\n";
        std::cout << "1. Charaktere Infos anzeigen\n";
        std::cout << "2. Charaktere auswählen und Spiel starten\n";
        std::cout << "Wähle eine Option (1 oder 2): ";

        int input;
        std::cin >> input;

        switch (input) {
            case 1:
                showCharacterInfos(characters);
                break;
            case 2:
                player1 = chooseCharacter(characters, 1);
                player2 = chooseCharacter(characters, 2);
                exit = true;
                break;
            default:
                std::cout << "Ungültige Eingabe. Bitte erneut versuchen.\n";
        }
    }
}

std::vector<Character> loadCharacters() {
    return {
        Character("Goblin", Attack{"Dolchstich von hinten"}, Attack{"Brunnen Vergiften"}, Defense{"Stellt sich als Opfer dar"}),
        Character("MG-Opa", Attack{"Schlag mit Gehstock"}, Attack{"Ballert mit MG"}, Defense{"Rückzug in Schützenbunker"}),
        Character("Schweinebärmann", Attack{"Al Gore sagt, es ist voll ernte"}, Attack{"Prankenhieb"}, Defense{"Vortrag über Umweltschutz"}),
        Character("Redneck", Attack{"Sie klauen unsere Jobs"}, Attack{"AR-15"}, Defense{"Cousin/Bruder kommt"}),
        Character("Maggus Rühl", Attack{"335kg Benchpress"}, Attack{"Protein Furz"}, Defense{"De Bob Tschigarillo hilft"}),
        Character("Rainer Winkler", Attack{"Prügel raus Hauen"}, Attack{"Der Schwarze Luan kommt"}, Defense{"Lügenlord"}),
        Character("Chuck Norris", Attack{"Schmeisst Parties 500 Meter weit"}, Attack{"Roundhouse Kick"}, Defense{"Hatt als Kind Katholische Priester missbracht"}),
        Character("Zinedine Zidane", Attack{"Faust in die Fresse"}, Attack{"Kopfnuss"}, Defense{"Doping"}),
        Character("Drachen Haider", Attack{"Hausbesuch"}, Attack{"Doxing"}, Defense{"Anzeige ist raus"})
    };
}