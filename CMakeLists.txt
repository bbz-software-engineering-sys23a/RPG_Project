cmake_minimum_required(VERSION 3.10)
# Projektname (kannst du ändern, z.B. in ExtremeCombat)
project(ExtremeCombat) # Name konsistent gemacht

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Name der ausführbaren Datei
# Liste hier ALLE .cpp Dateien auf!
add_executable(ExtremeCombat
        main.cpp
        StartScreen.cpp
        GameData.cpp
        Utils.cpp
)

