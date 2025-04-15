cmake_minimum_required(VERSION 3.10) # Oder eine neuere Version
project(RPG_Project) # Name deines Projekts

set(CMAKE_CXX_STANDARD 17) # C++ Standard verwenden
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Stelle sicher, dass hier main.cpp und StartScreen.cpp aufgelistet sind!
add_executable(ExtremeCombat
        main.cpp
        StartScreen.cpp
        Datenstrukturen.cpp
        Charaktere.cpp
        # Füge hier später weitere .cpp Dateien hinzu
)
