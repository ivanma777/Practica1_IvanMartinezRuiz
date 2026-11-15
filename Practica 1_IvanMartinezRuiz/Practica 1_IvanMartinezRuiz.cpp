#include <iostream>
#include <random>
#include <limits>
#include <string>
#include <vector>

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

class Enemy {
private:
    string name;
    int level;
    int treasurePercent;
    int ac;
    int hp;
    int maxHp;
    int damageDie;

public:
    Enemy(string name, int level, int treasure, int armorClass, int damage)
        : name(name), level(level), treasurePercent(treasure),
        ac(armorClass), damageDie(damage) {

        maxHp = 12 + level * 5;
        hp = maxHp;
    }

    virtual ~Enemy() = default;

    string getName() const { return name; }
    int getLevel() const { return level; }
    int getTreasure() const { return treasurePercent; }
    int getAC() const { return ac; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getDamageDie() const { return damageDie; }

    void takeDamage(int damage) {
        hp -= damage;

        if (hp < 0) {
            hp = 0;
        }
    }

    virtual char getSymbol() const {
        return 'E';
    }

    virtual int attacksPerTurn() const {
        return 1;
    }

    virtual int getExtraElementDice() const {
        return 0;
    }
};



class Slime : public Enemy {
private:
    vector<Element> elements;

public:
    Slime(int level, int treasure, int ac,
        const vector<Element>& elements)
        : Enemy("Slime", level, treasure, ac, 4),
        elements(elements) {
    }

    char getSymbol() const override {
        return 'S';
    }

    int getExtraElementDice() const override {
        return static_cast<int>(elements.size());
    }

    string getElementDescription() const {
        string result;

        for (size_t i = 0; i < elements.size(); i++) {
            if (i > 0) {
                result += ", ";
            }

            switch (elements[i]) {
            case Element::Fire:
                result += "Fuego";
                break;

            case Element::Ice:
                result += "Hielo";
                break;

            case Element::Poison:
                result += "Veneno";
                break;
            }
        }

        return result;
    }
};

class Orc : public Enemy {
private:
    int anger;

public:
    Orc(int level, int treasure, int ac, int anger)
        : Enemy("Orco", level, treasure, ac, 6),
        anger(anger) {
    }

    char getSymbol() const override {
        return 'O';
    }

    int getAnger() const {
        return anger;
    }

    int attacksPerTurn() const override {
        return anger >= 2 ? 2 : 1;
    }
};

class Dragon : public Enemy {
public:
    Dragon(int level, int treasure, int ac)
        : Enemy("Dragon", level, treasure, ac, 10) {
    }

    char getSymbol() const override {
        return 'D';
    }
};


void showEnemy(const Enemy& enemy) {
    cout << "[" << enemy.getSymbol() << "] "
        << enemy.getName()
        << " | Nivel: " << enemy.getLevel()
        << " | PS: " << enemy.getHp()
        << "/" << enemy.getMaxHp()
        << " | Tesoro: " << enemy.getTreasure() << "%"
        << " | AC: " << enemy.getAC()
        << " | Dano: 1d" << enemy.getDamageDie()
        << " + " << enemy.getExtraElementDice() << "d4"
        << " | Ataques: " << enemy.attacksPerTurn()
        << endl;
}

int main() {

    Difficulty difficulty = chooseDifficulty();

    cout << "\nDificultad seleccionada." << endl;

    cout << "Numero de salas: "
        << randomInt(getMinRooms(difficulty),
            getMaxRooms(difficulty)) << endl;

    cout << "Prueba de dado D20: " << rollDie(20) << endl;
    cout << "\n=== PRUEBA DE ENEMIGOS ===" << endl;

    Slime slime(1, 30, 10,
        { Element::Fire, Element::Ice });

   

    showEnemy(slime);
    cout << "Elementos: "
        << slime.getElementDescription() << endl;

  

    return 0;
}