#ifndef CHARACTER_H
#define CHARACTER_H

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

    void showAsciiArt() const;

public:
    Character(std::string n, Attack a1, Attack a2, Defense def);
    void showInfo() const;
    std::string getName() const;
};

Character chooseCharacter(const std::vector<Character>& characters, int playerNumber);
std::vector<Character> loadCharacters();
void showCharacterInfos(const std::vector<Character>& characters);
void startCharacterSelection(const std::vector<Character>& characters, Character& player1, Character& player2);

#endif
