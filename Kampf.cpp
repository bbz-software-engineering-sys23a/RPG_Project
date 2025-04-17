#include "Kampf.h"           // Eigenen Header einbinden (enthält jetzt CombatAction Enum)
#include "Datenstrukturen.h" // Für Player, Character-Methoden (via Charaktere.h), Utils
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm> // Für std::max, std::swap, std::transform, std::min
#include <limits>
#include <cctype>    // Für std::tolower
#include <stdexcept> // Für std::stoi Fehler
#include <cstdlib>   // Für exit()
#include <utility>   // Für std::swap


// Hilfsfunktion zur Konvertierung von int zu CombatAction (intern)
namespace {
    CombatAction intToCombatAction(int choice) {
        switch (choice) {
            case 1: return CombatAction::Attack1;
            case 2: return CombatAction::Attack2;
            case 3: return CombatAction::Defend;
            default: return CombatAction::Invalid;
        }
    }
}

// --- Implementierung Kampflogik (angepasst an Getter und Enum) ---
void startCombat(Player& player1, Player& player2) {
    std::cout << "\n==================== KAMPFBEGINN ====================" << std::endl;
    // Verwende Getter, um auf Character-Daten zuzugreifen
    player1.getCharacterData().displayInfo();
    std::cout << "                    VS" << std::endl;
    player2.getCharacterData().displayInfo();
    std::cout << "=====================================================" << std::endl;
    sleepMilliseconds(2500);

    Player* currentPlayer = (rollD20() <= 10) ? &player1 : &player2;
    Player* opponentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

    // Sicherstellen, dass der Verteidigungsbonus zu Beginn 0 ist (über Setter)
    player1.resetDefenseBonus();
    player2.resetDefenseBonus();

    std::cout << "\n" << currentPlayer->getName() << " (" << currentPlayer->getCharacterData().getName() << ") beginnt!" << std::endl;
    sleepMilliseconds(1500);

    int round = 1;
    // Verwende Getter für Siegbedingung
    while (!player1.getCharacterData().isDefeated() && !player2.getCharacterData().isDefeated()) {
        std::cout << "\n---------- Runde " << round << ": " << currentPlayer->getName() << " (" << currentPlayer->getCharacterData().getName() << ") ist am Zug ----------" << std::endl;

        // OWP Regeneration zu Beginn des Zuges (über Referenz und Methode)
        std::cout << currentPlayer->getName() << " regeneriert OWP." << std::endl;
        // getCharacterData() gibt Referenz zurück, dann Methode aufrufen
        currentPlayer->getCharacterData().addOwp(10);
        sleepMilliseconds(800);

        // Zeige HP und OWP beider Spieler an (über Getter)
        // Hole Referenzen auf Character-Objekte für Lesbarkeit
        const Character& currentChara = currentPlayer->getCharacterData();
        const Character& opponentChara = opponentPlayer->getCharacterData();
        std::cout << currentPlayer->getName() << " HP: " << currentChara.getHP() << "/" << currentChara.getMaxHP()
                  << " OWP: " << currentChara.getOwp() << "/" << currentChara.getMaxOwp()
                  << "  |  " << opponentPlayer->getName() << " HP: " << opponentChara.getHP() << "/" << opponentChara.getMaxHP()
                  << " OWP: " << opponentChara.getOwp() << "/" << opponentChara.getMaxOwp()
                  << std::endl;
        sleepMilliseconds(1000);


        // --- Aktion auswählen (Angriff 1/2, Verteidigen, Exit) ---
        std::cout << "\nWähle deine Aktion:" << std::endl;
        currentChara.displayAttackOptions(); // Zeigt Optionen 1 und 2 mit Kosten an
        std::cout << "  3: Verteidigen (Verbessert Verteidigung nächste Runde, +5 OWP)" << std::endl;

        CombatAction chosenAction = CombatAction::Invalid;
        std::string inputLine;
        bool affordable = false;

        while (chosenAction == CombatAction::Invalid || !affordable) { // Schleife bis gültige UND bezahlbare Wahl
            chosenAction = CombatAction::Invalid; // Reset für jeden Versuch
            affordable = false;

            std::cout << "Deine Wahl (1, 2, 3 oder 'Exit' zum Beenden): ";
            // Verwende getline, um Leerzeichenprobleme zu vermeiden und 'Exit' zu lesen
            if (!std::getline(std::cin, inputLine)) {
                std::cerr << "Fehler bei der Eingabe. Breche ab." << std::endl;
                exit(1); // Beendet das Programm bei Eingabefehler
            }

            // Eingabe in Kleinbuchstaben umwandeln für 'Exit'-Vergleich
            std::string lowerInput = inputLine;
            std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            if (lowerInput == "exit") {
                chosenAction = CombatAction::Exit;
                affordable = true; // Exit ist immer "bezahlbar"
            } else {
                // Versuche, die Eingabe als Zahl zu interpretieren
                try {
                    size_t processedChars = 0;
                    int choiceNum = std::stoi(inputLine, &processedChars);

                    // Prüfen, ob die gesamte Eingabe eine Zahl war
                    if (processedChars == inputLine.length()) {
                        chosenAction = intToCombatAction(choiceNum); // Konvertiere int zu Enum

                        if (chosenAction != CombatAction::Invalid) {
                            // Kostenprüfung für Angriffe (verwende Getter)
                            if (chosenAction == CombatAction::Attack1) {
                                const int ATTACK1_COST = 5;
                                if (currentChara.canAfford(ATTACK1_COST)) {
                                    affordable = true;
                                } else {
                                    std::cout << "Nicht genug Ominöses weißes Pulver für '" << currentChara.getAttack1().name << "'! (Benötigt: "
                                              << ATTACK1_COST << ", Vorhanden: " << currentChara.getOwp() << ")" << std::endl;
                                    sleepMilliseconds(1500);
                                }
                            } else if (chosenAction == CombatAction::Attack2) {
                                const AttackData& attack2_data = currentChara.getAttack2();
                                if (currentChara.canAfford(attack2_data.base_damage)) {
                                    affordable = true;
                                } else {
                                    std::cout << "Nicht genug Ominöses weißes Pulver für '" << attack2_data.name << "'! (Benötigt: "
                                              << attack2_data.base_damage << ", Vorhanden: " << currentChara.getOwp() << ")" << std::endl;
                                    sleepMilliseconds(1500);
                                }
                            } else { // chosenAction == CombatAction::Defend
                                affordable = true; // Verteidigen kostet nichts
                            }
                        } else {
                            std::cout << "Ungültige Zahl. Bitte 1, 2, 3 eingeben (oder 'Exit')." << std::endl;
                        }
                    } else {
                         // Eingabe war keine reine Zahl (und nicht 'exit')
                         std::cout << "Ungültige Eingabe. Bitte 1, 2, 3 oder 'Exit' eingeben." << std::endl;
                    }
                } catch (const std::invalid_argument& e) {
                    // std::stoi konnte die Eingabe nicht als Zahl interpretieren
                    std::cout << "Ungültige Eingabe. Bitte 1, 2, 3 oder 'Exit' eingeben." << std::endl;
                } catch (const std::out_of_range& e) {
                     // Zahl war zu gross/klein für int
                     std::cout << "Zahl außerhalb des gültigen Bereichs." << std::endl;
                }
            } // end else (not "exit")
        } // Ende der Aktionsauswahl-Schleife

        // --- Aktion ausführen ---
        if (chosenAction == CombatAction::Exit) {
            std::cout << "\nSpiel wird auf Wunsch beendet..." << std::endl;
            exit(0);
        }

        // Verwende modifizierbare Referenz für Aktionen, die den Charakter ändern
        Character& currentCharaMod = currentPlayer->getCharacterData();
        Character& opponentCharaMod = opponentPlayer->getCharacterData();

        if (chosenAction == CombatAction::Attack1 || chosenAction == CombatAction::Attack2) { // Angriff gewählt
            // Holen der Angriffsdaten über Getter (von const Referenz)
            const AttackData& chosenAttackData = (chosenAction == CombatAction::Attack1) ? currentChara.getAttack1() : currentChara.getAttack2();
            int owpCost = (chosenAction == CombatAction::Attack1) ? 5 : chosenAttackData.base_damage;

            // Verwende OWP-Methode auf modifizierbarer Referenz
            bool paid = currentCharaMod.useOwp(owpCost);
            if (paid) {
                 std::cout << currentPlayer->getName() << " verbraucht " << owpCost << " OWP. Verbleibend: " << currentCharaMod.getOwp() << std::endl;
                 sleepMilliseconds(1000);
            } else {
                 // Sollte wegen der Prüfung oben nicht passieren
                 std::cerr << "[FEHLER] Konnte OWP nicht verbrauchen trotz canAfford Check!" << std::endl;
                 continue; // Zur nächsten Runde springen
            }

            std::cout << "\n" << currentPlayer->getName() << " setzt '" << chosenAttackData.name << "' ein!" << std::endl;
            sleepMilliseconds(1200);

            int attackRoll = rollD20();
            int defenseRoll = rollD20();
            bool is_crit = (attackRoll == 20);
            bool is_fumble = (attackRoll == 1);

            std::cout << currentPlayer->getName() << " würfelt eine " << attackRoll << " (Angriff)." << std::endl;
            sleepMilliseconds(600);
            std::cout << opponentPlayer->getName() << " würfelt eine " << defenseRoll << " (Verteidigung)." << std::endl;
            sleepMilliseconds(1200);

            // Hole aktuellen Verteidigungsbonus über Getter und setze ihn zurück über Setter
            int current_defense_bonus = opponentPlayer->getDefenseBonus();
            opponentPlayer->resetDefenseBonus(); // Bonus wird verbraucht/zurückgesetzt

            int potentialDamage = 0;
            if (is_fumble) {
                std::cout << ">>> FEHLSCHLAG! <<< Der Angriff geht daneben!" << std::endl;
                potentialDamage = 0;
                sleepMilliseconds(800);
            } else {
                 // Berechne potenziellen Schaden (verwende Getter für Basis-Schaden)
                 potentialDamage = std::max(0, chosenAttackData.base_damage + attackRoll - defenseRoll - current_defense_bonus);
                 if (current_defense_bonus > 0) {
                      std::cout << opponentPlayer->getName() << " verteidigt sich verbessert! (Bonus: " << current_defense_bonus << ")" << std::endl;
                      sleepMilliseconds(800);
                 }
                 if (is_crit) {
                     std::cout << ">>> KRITISCHER TREFFER! <<<" << std::endl;
                     potentialDamage = static_cast<int>(potentialDamage * 1.5); // Einfacher Bonus für Crit
                     sleepMilliseconds(800);
                 }
            }

            // Verwende Getter für Verteidigungstext (von const Referenz)
            std::cout << opponentPlayer->getName() << " versucht: \"" << opponentChara.getDefenseActionText() << "\"" << std::endl;
            sleepMilliseconds(1200);

            // Schaden zufügen über Methode (auf modifizierbarer Referenz)
            if (potentialDamage > 0) {
                 opponentCharaMod.takeDamage(potentialDamage);
            } else {
                 std::cout << opponentPlayer->getName() << " weicht aus oder der Angriff war zu schwach! (0 Schaden)" << std::endl;
            }

        } else if (chosenAction == CombatAction::Defend) { // Verteidigen gewählt
            int defense_bonus_amount = 5;
            int owp_gain_defense = 5;
            // Setze Bonus über Setter
            currentPlayer->setDefenseBonus(defense_bonus_amount);
            // Füge OWP hinzu über Methode (auf modifizierbarer Referenz)
            currentCharaMod.addOwp(owp_gain_defense);
            std::cout << "\n" << currentPlayer->getName() << " geht in Verteidigungshaltung! (Nächster Verteidigungswurf +" << defense_bonus_amount << ")" << std::endl;
            std::cout << currentPlayer->getName() << " sammelt " << owp_gain_defense << " Ominöses weißes Pulver! Aktuell: " << currentCharaMod.getOwp() << std::endl;
            sleepMilliseconds(1500);
        }
        // --- Ende Aktion ausführen ---


        std::cout << opponentPlayer->getName() << " hat noch " << opponentChara.getHP() << " HP." << std::endl;
        sleepMilliseconds(2500);

        // Siegbedingung prüfen (über Getter von const Referenz)
        if (opponentChara.isDefeated()) {
            std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << opponentPlayer->getName() << " (" << opponentChara.getName() << ") wurde besiegt!" << std::endl;
            std::cout << currentPlayer->getName() << " (" << currentChara.getName() << ") hat gewonnen!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            displayVictoryFireworks(); // Zeigt jetzt den Pokal an
            break; // Kampfschleife verlassen
        }

        // Spieler wechseln für die nächste Runde
        std::swap(currentPlayer, opponentPlayer);
        round++;
    } // Ende while Kampfschleife

     std::cout << "\n==================== KAMPFENDE ======================" << std::endl;
}

// --- Implementierung Siegesanimation (unverändert) ---
void displayVictoryFireworks() {
    // ASCII-Art: Pokal
    std::cout << R"(

          ___________
         '._==_==_.'
         .-\:      /-.
        | (|:.     |) |
         '-|:.     |-'
           \::.    /
            '::. .'
              ) (
            _.' '._
           `-------`
         * * S I E G * *

    )" << std::endl;
    sleepMilliseconds(2500); // Pause, um die Grafik anzusehen
}
