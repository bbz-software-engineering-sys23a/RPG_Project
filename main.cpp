#include <iostream>     // Include für std::cout etc. in main
#include "StartScreen.h" // !!! Wichtig: Binde den neuen Header ein !!!

int main() {
    // Rufe die ausgelagerte Funktion auf
    displayStartScreen();

    // --- Hier würde der Rest deines Spiels beginnen ---
    std::cout << "\nStartbildschirm beendet. Hier geht das Spiel weiter..." << std::endl;
    // (z.B. Charakterdaten laden, Hauptmenü anzeigen etc.)


    return 0; // Programm erfolgreich beenden
}