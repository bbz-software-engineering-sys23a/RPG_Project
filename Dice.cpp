#include "Dice.h"
#include <chrono> // Für Seed-Generierung
#include <stdexcept> // Für Fehler

// Initialisierung der statischen Member der Basisklasse Dice
std::mt19937 Dice::rng;
bool Dice::rng_initialized = false;

void Dice::initializeRNG() {
    if (!rng_initialized) {
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        rng.seed(seed);
        rng_initialized = true;
    }
}

// Implementierung für D10Dice::roll()
int D10Dice::roll() const {
    if (!rng_initialized) {
        initializeRNG(); // Sicherstellen, dass RNG initialisiert ist
    }
    std::uniform_int_distribution<int> dist(1, 10);
    return dist(rng);
}

// NEU: Implementierung für D10Dice::getMaxValue()
int D10Dice::getMaxValue() const {
    return 10;
}

// Implementierung für D20Dice::roll()
int D20Dice::roll() const {
    if (!rng_initialized) {
        initializeRNG(); // Sicherstellen, dass RNG initialisiert ist
    }
    std::uniform_int_distribution<int> dist(1, 20);
    return dist(rng);
}

// NEU: Implementierung für D20Dice::getMaxValue()
int D20Dice::getMaxValue() const {
    return 20;
}


// Implementierung für D40Dice::roll()
int D40Dice::roll() const {
    if (!rng_initialized) {
        initializeRNG(); // Sicherstellen, dass RNG initialisiert ist
    }
    std::uniform_int_distribution<int> dist(1, 40);
    return dist(rng);
}

// NEU: Implementierung für D40Dice::getMaxValue()
int D40Dice::getMaxValue() const {
    return 40;
}
