// Charaktere.cpp – Sprint 1: Charakterdaten mit ASCII-Art (alle 9, ohne Sonderzeichen)
#include <iostream>
#include <string>
#include <vector>

struct Attack {
    std::string name;
};

struct Defense {
    std::string name;
};

class Character {
private:
    std::string name;
    Attack attack1;
    Attack attack2;
    Defense defense;

    void showAsciiArt() const {
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
    (•_•)
    <)   )╯ ALTE SCHULE!
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
     [¯\_(ツ)_/¯]
     |  "MEINUNG"
     |   o===|
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

public:
    Character(std::string n, Attack a1, Attack a2, Defense def)
        : name(n), attack1(a1), attack2(a2), defense(def) {}

    void showInfo() const {
        std::cout << "Name: " << name << std::endl;
        showAsciiArt();
        std::cout << "Angriff 1: " << attack1.name << std::endl;
        std::cout << "Angriff 2: " << attack2.name << std::endl;
        std::cout << "Verteidigung: " << defense.name << std::endl;
    }

    std::string getName() const { return name; }
};

int main() {
    std::vector<Character> characterList = {
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

    for (const auto& c : characterList) {
        c.showInfo();
        std::cout << "-----------------------" << std::endl;
    }

    return 0;
}
