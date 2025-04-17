#ifndef EXTREMECOMBAT_DICE_H
#define EXTREMECOMBAT_DICE_H

#include <random> // Für Zufallszahlen benötigt

// Abstrakte Basisklasse für alle Würfeltypen.
class Dice {
public:
    // Führt einen Würfelwurf durch (muss von Ableitung implementiert werden).
    virtual int roll() const = 0;
    // Gibt den Maximalwert des Würfels zurück (muss von Ableitung implementiert werden).
    virtual int getMaxValue() const = 0;
    // Virtueller Destruktor.
    virtual ~Dice() = default;

protected:
    // Statischer RNG für alle Würfel.
    static std::mt19937 rng;
    static bool rng_initialized;
    static void initializeRNG(); // Methode zur Initialisierung.
};

// Repräsentiert einen 10-seitigen Würfel (D10).
class D10Dice : public Dice {
public:
    int roll() const override;
    int getMaxValue() const override;
};

// Repräsentiert einen 20-seitigen Würfel (D20).
class D20Dice : public Dice {
public:
    int roll() const override;
    int getMaxValue() const override;
};

// Repräsentiert einen 40-seitigen Würfel (D40).
class D40Dice : public Dice {
public:
    int roll() const override;
    int getMaxValue() const override;
};

#endif // EXTREMECOMBAT_DICE_H
// EXTREMECOMBAT_DICE_H