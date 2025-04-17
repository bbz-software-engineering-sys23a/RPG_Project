#ifndef EXTREMECOMBAT_CHARAKTERINFORMATIONEN_H
#define EXTREMECOMBAT_CHARAKTERINFORMATIONEN_H

#include <vector>       // Benötigt für std::vector als Parameter
#include "Charaktere.h" // Benötigt die Character-Klassendefinition

/**
 * @brief Zeigt die Detailinformationen für alle Charaktere in einem Vektor an.
 * Ruft für jeden Charakter die `displayInfo()` Methode auf.
 * Wartet am Ende auf eine Benutzereingabe (Enter), bevor die Funktion zurückkehrt.
 * @param available_chars Ein konstanter Referenz-Vektor mit den anzuzeigenden Charakteren.
 */
void displayAllCharacterInfo(const std::vector<Character>& available_chars);

#endif //EXTREMECOMBAT_CHARAKTERINFORMATIONEN_H