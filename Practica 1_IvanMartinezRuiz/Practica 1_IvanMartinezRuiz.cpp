#include <iostream>
#include <random>
#include <limits>

using namespace std;

enum class Difficulty {
    Easy,
    Normal,
    Hard
};

enum class Element {
    Fire,
    Ice,
    Poison
};

int randomInt(int minValue, int maxValue) {
    static random_device rd;
    static mt19937 generator(rd());

    uniform_int_distribution<int> distribution(minValue, maxValue);

    return distribution(generator);
}

int rollDie(int faces) {
    return randomInt(1, faces);
}

Difficulty chooseDifficulty() {
    int option;

    cout << "=== ROGUE DUNGEON ===" << endl;
    cout << "Selecciona la dificultad:" << endl;
    cout << "1. Facil" << endl;
    cout << "2. Normal" << endl;
    cout << "3. Dificil" << endl;

    while (true) {
        cout << "Opcion: ";

        if (cin >> option && option >= 1 && option <= 3) {
            break;
        }

        cout << "Opcion no valida." << endl;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (option == 1) return Difficulty::Easy;
    if (option == 2) return Difficulty::Normal;

    return Difficulty::Hard;
}

int getMinRooms(Difficulty difficulty) {
    if (difficulty == Difficulty::Easy) return 3;
    if (difficulty == Difficulty::Normal) return 4;

    return 6;
}

int getMaxRooms(Difficulty difficulty) {
    if (difficulty == Difficulty::Easy) return 5;
    if (difficulty == Difficulty::Normal) return 6;

    return 8;
}

int main() {

    Difficulty difficulty = chooseDifficulty();

    cout << "\nDificultad seleccionada." << endl;

    cout << "Numero de salas: "
        << randomInt(getMinRooms(difficulty),
            getMaxRooms(difficulty)) << endl;

    cout << "Prueba de dado D20: " << rollDie(20) << endl;

    return 0;
}