#include "Dice.h"
#include <chrono>
#include <stdexcept>

// Initialisierung der statischen Member.
std::mt19937 Dice::rng;
bool Dice::rng_initialized = false;

// Initialisiert den statischen RNG einmalig.
void Dice::initializeRNG() {
    if (!rng_initialized) {
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        rng.seed(seed);
        rng_initialized = true;
    }
}

// Würfelt eine Zahl von 1-10.
int D10Dice::roll() const {
    if (!rng_initialized) {
        initializeRNG();
    }
    std::uniform_int_distribution<int> dist(1, 10);
    return dist(rng);
}

// Gibt den Maximalwert 10 zurück.
int D10Dice::getMaxValue() const {
    return 10;
}

// Würfelt eine Zahl von 1-20.
int D20Dice::roll() const {
    if (!rng_initialized) {
        initializeRNG();
    }
    std::uniform_int_distribution<int> dist(1, 20);
    return dist(rng);
}

// Gibt den Maximalwert 20 zurück.
int D20Dice::getMaxValue() const {
    return 20;
}

// Würfelt eine Zahl von 1-40.
int D40Dice::roll() const {
    if (!rng_initialized) {
        initializeRNG();
    }
    std::uniform_int_distribution<int> dist(1, 40);
    return dist(rng);
}

// Gibt den Maximalwert 40 zurück.
int D40Dice::getMaxValue() const {
    return 40;
}
