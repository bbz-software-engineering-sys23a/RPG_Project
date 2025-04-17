#ifndef EXTREMECOMBAT_DICE_H
#define EXTREMECOMBAT_DICE_H

#include <random> // Für Zufallszahlen benötigt

// Abstrakte Basisklasse für Würfel
class Dice {
public:
    // Pure virtual functions - jede abgeleitete Klasse MUSS diese implementieren
    virtual int roll() const = 0;
    virtual int getMaxValue() const = 0; // <-- NEU: Maximalwert des Würfels

    // Virtual destructor - wichtig für Basisklassen mit virtuellen Funktionen
    virtual ~Dice() = default;

protected:
    // Statischer Random Number Generator für alle Würfeltypen
    // Wird nur einmal initialisiert
    static std::mt19937 rng;
    static bool rng_initialized;
    static void initializeRNG(); // Statische Methode zur Initialisierung
};

// Abgeleitete Klasse für einen 10-seitigen Würfel
class D10Dice : public Dice {
public:
    int roll() const override;
    int getMaxValue() const override; // <-- NEU
};

// Abgeleitete Klasse für einen 20-seitigen Würfel
class D20Dice : public Dice {
public:
    int roll() const override;
    int getMaxValue() const override; // <-- NEU
};

// Abgeleitete Klasse für einen 40-seitigen Würfel
class D40Dice : public Dice {
public:
    int roll() const override;
    int getMaxValue() const override; // <-- NEU
};

#endif // EXTREMECOMBAT_DICE_H