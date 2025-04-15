#include <iostream> // Für Konsolenausgabe (cout)
#include <string>   // Für Text (string)
#include <limits>   // Für die "Enter drücken"-Pause (numeric_limits)

// Funktion zum Anzeigen des Startbildschirms
void displayStartScreen() {
    // Extrem einfache ASCII-Art für "Extreme Combat" für maximale Kompatibilität
    std::cout << R"(
###########################################################################
#                                                                         #
#                          EXTREME COMBAT                                 #
#                                                                         #
###########################################################################
    )" << std::endl; // R"(...)" ist ein Raw String Literal

    std::cout << std::endl; // Leerzeile für Abstand
    std::cout << std::endl; // Noch eine Leerzeile

    // Namen der Ersteller anzeigen
    // (Leerzeichen am Anfang angepasst für die neue Breite)
    std::cout << "                 Ein Spiel von: Gabriel, Felix und Florin" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // Kurze Pause, damit der Spieler den Bildschirm sehen kann
    std::cout << "                 Druecke Enter um fortzufahren...";
    // Ignoriert vorherige Eingaben im Buffer und wartet dann auf Enter
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // Alternative: std::cin.get();

    // Optional: Bildschirm löschen nach Enter (systemabhängig)
    // system("cls"); // Für Windows
    // system("clear"); // Für Linux/macOS
}

int main() {
    // Zeige den Startbildschirm als Erstes an
    displayStartScreen();

    // --- Hier würde der Rest deines Spiels beginnen ---
    std::cout << "\nStartbildschirm beendet. Hier geht das Spiel weiter..." << std::endl;


    return 0; // Programm erfolgreich beenden
}
