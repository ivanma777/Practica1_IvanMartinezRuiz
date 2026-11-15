#include <iostream>
#include <random>
#include <limits>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

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

struct EnemyStats {
    int level;
    int treasure;
    int ac;
};

string elementName(Element element) {

    switch (element) {
    case Element::Fire:
        return "Fuego";

    case Element::Ice:
        return "Hielo";

    case Element::Poison:
        return "Veneno";
    }

    return "Desconocido";
}
#pragma region dificultad

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


EnemyStats generateEnemyStats(int dungeonLevel) {

    int level = dungeonLevel + randomInt(-1, 1);

    if (level < 1) {
        level = 1;
    }

    int treasure = randomInt(5 * level, 15 * level);
    int ac = 8 + level + randomInt(0, 3);

    return { level, treasure, ac };

}
#pragma endregion dificultad

#pragma region enemy y enemy creator



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

class EnemyCreator {
public:
    virtual ~EnemyCreator() = default;

    virtual unique_ptr<Enemy> createEnemy(int dungeonLevel) const = 0;
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

class SlimeCreator : public EnemyCreator {
public:
    unique_ptr<Enemy> createEnemy(int dungeonLevel) const override {

        EnemyStats stats = generateEnemyStats(dungeonLevel);

        vector<Element> elements;

   
        elements.push_back(
            static_cast<Element>(randomInt(0, 2))
        );

       
        if (randomInt(0, 1) == 1) {

            Element second = static_cast<Element>(randomInt(0, 2));

            while (second == elements[0]) {
                second = static_cast<Element>(randomInt(0, 2));
            }

            elements.push_back(second);
        }

        return make_unique<Slime>(
            stats.level,
            stats.treasure,
            stats.ac,
            elements
        );
    }
};

class OrcCreator : public EnemyCreator {
public:
    unique_ptr<Enemy> createEnemy(int dungeonLevel) const override {

        EnemyStats stats = generateEnemyStats(dungeonLevel);

        int anger = randomInt(0, 3);

        return make_unique<Orc>(
            stats.level,
            stats.treasure,
            stats.ac,
            anger
        );
    }
};

class DragonCreator : public EnemyCreator {
public:
    unique_ptr<Enemy> createEnemy(int dungeonLevel) const override {

        EnemyStats stats = generateEnemyStats(dungeonLevel);

        return make_unique<Dragon>(
            stats.level,
            stats.treasure,
            stats.ac
        );
    }
};

class EnemyFactory {
private:
    SlimeCreator slimeCreator;
    OrcCreator orcCreator;
    DragonCreator dragonCreator;

    bool dragonCreated = false;

public:

    
    void reset() {
        dragonCreated = false;
    }

    unique_ptr<Enemy> createRandomEnemy(int dungeonLevel) {

        int chance = randomInt(1, 100);

   
        if (!dragonCreated && chance <= 5) {
            dragonCreated = true;
            return dragonCreator.createEnemy(dungeonLevel);
        }

      
        if (chance <= 55) {
            return slimeCreator.createEnemy(dungeonLevel);
        }

       
        return orcCreator.createEnemy(dungeonLevel);
    }
};

#pragma endregion enemy y enemy creator (Factory)

#pragma region builder

struct Room {
    int id = 0;
    int width = 10;
    int height = 6;
    int gold = 0;

    vector<int> connections;
    vector<unique_ptr<Enemy>> enemies;
};

struct Level {
    vector<Room> rooms;
};


class RandomLevelBuilder {
private:
    Level level;
    EnemyFactory enemyFactory;

   
    void connect(int first, int second) {

        level.rooms[first].connections.push_back(second);
        level.rooms[second].connections.push_back(first);
    }

public:

    
    void reset() {
        level.rooms.clear();
        enemyFactory.reset();
    }

    
    void buildRooms(Difficulty difficulty) {

        int minRooms = getMinRooms(difficulty);
        int maxRooms = getMaxRooms(difficulty);

        int roomCount = randomInt(minRooms, maxRooms);

        for (int i = 0; i < roomCount; i++) {

            Room room;

            room.id = i;
            room.width = randomInt(8, 18);
            room.height = randomInt(5, 9);

            level.rooms.push_back(std::move(room));
        }
    }

    
    void connectRooms() {

        int count = static_cast<int>(level.rooms.size());

        
        for (int i = 0; i < count - 1; i++) {
            connect(i, i + 1);
        }

       
        for (int i = 0; i < count; i++) {

            for (int j = i + 2; j < count; j++) {

                if (randomInt(1, 100) <= 25) {
                    connect(i, j);
                }
            }
        }
    }

    
    void populateRooms(int dungeonLevel) {

        for (Room& room : level.rooms) {

            room.gold = randomInt(0, 15);

            int enemyCount = randomInt(0, 3);

            for (int i = 0; i < enemyCount; i++) {

                room.enemies.push_back(
                    enemyFactory.createRandomEnemy(dungeonLevel)
                );
            }
        }
    }

    
    Level getResult() {
        return std::move(level);
    }
};
#pragma endregion builder

#pragma region weapon y player

class Weapon {
private:
    string name;
    int damageDie;
    vector<Element> elements;

public:
    Weapon(string name, int damageDie,
        const vector<Element>& elements = {})
        : name(name), damageDie(damageDie),
        elements(elements) {
    }

    string getName() const {
        return name;
    }

    int rollDamage() const {

        int total = rollDie(damageDie);

       
        for (size_t i = 0; i < elements.size(); i++) {
            total += rollDie(4);
        }

        return total;
    }

    string getDamageDescription() const {

        string result = "1d" + to_string(damageDie);

        for (Element element : elements) {
            result += " + 1d4 " + elementName(element);
        }

        return result;
    }
};

class Player {
private:
    int level = 1;
    int hp = 100;
    int maxHp = 100;
    int hitBonus = 0;
    int gold = 0;

public:

    int getLevel() const {
        return level;
    }

    int getHp() const {
        return hp;
    }

    int getMaxHp() const {
        return maxHp;
    }

    int getHitBonus() const {
        return hitBonus;
    }

    int getGold() const {
        return gold;
    }

    bool isAlive() const {
        return hp > 0;
    }

    void receiveDamage(int damage) {
        hp -= damage;

        if (hp < 0) {
            hp = 0;
        }
    }

    void addGold(int amount) {
        gold += amount;
    }

  
    bool attack(Enemy& enemy, const Weapon& weapon) {

        if (!enemy.getHp()) {
            cout << "El enemigo ya ha sido derrotado." << endl;
            return false;
        }

        int d20 = rollDie(20);
        int result = d20 + hitBonus;

        cout << "\nAtacas con " << weapon.getName() << endl;

        cout << "D20: " << d20
            << " + " << hitBonus
            << " = " << result << endl;

        cout << "AC del enemigo: " << enemy.getAC() << endl;

      
        if (result > enemy.getAC()) {

            int damage = weapon.rollDamage();

            enemy.takeDamage(damage);

            cout << "Has acertado!" << endl;
            cout << "Dano causado: " << damage << endl;

            cout << "PS del enemigo: "
                << enemy.getHp() << "/"
                << enemy.getMaxHp() << endl;

            return true;
        }

        cout << "Has fallado el ataque." << endl;

        return false;
    }
};
#pragma endregion weapon y player

#pragma region ASCII

void drawRoom(const Room& room) {

   
    vector<string> grid(
        room.height,
        string(room.width, ' ')
    );

    
    for (int x = 0; x < room.width; x++) {
        grid[0][x] = '#';
        grid[room.height - 1][x] = '#';
    }

    
    for (int y = 0; y < room.height; y++) {
        grid[y][0] = '#';
        grid[y][room.width - 1] = '#';
    }

    
    int perWall = room.width - 2;

    for (size_t i = 0; i < room.connections.size(); i++) {

        int row = (i < perWall) ? 0 : room.height - 1;

        int column = 1 + (i % perWall);

        grid[row][column] =
            static_cast<char>('0' + room.connections[i]);
    }

    
    for (size_t i = 0; i < room.enemies.size(); i++) {

        int x = 2 + static_cast<int>(i) * 2;

        grid[1][x] = room.enemies[i]->getSymbol();
    }

    
    if (room.gold > 0) {
        grid[room.height - 2][room.width - 3] = '$';
    }

    
    grid[room.height / 2][room.width / 2] = '@';

    
    cout << "\nSala " << room.id << " ("
        << room.width << "x" << room.height << ")" << endl;

    for (const string& line : grid) {
        cout << line << endl;
    }

    cout << "@ Jugador | S Slime | O Orco | D Dragon"
        << " | $ Oro | 0-7 Puertas" << endl;
}

void showConnections(const Room& room) {

    cout << "\nSalas conectadas: ";

    for (int connection : room.connections) {
        cout << connection << " ";
    }

    cout << endl;
}

bool isConnected(const Room& room, int destination) {

    for (int connection : room.connections) {

        if (connection == destination) {
            return true;
        }
    }

    return false;
}

void exploreLevel(const Level& level) {

    if (level.rooms.empty()) {
        cout << "No hay salas disponibles." << endl;
        return;
    }

    int currentRoom = 0;
    int destination;

    bool exploring = true;

    while (exploring) {

        system("cls");

        cout << "\n========== ROGUE ==========" << endl;

        
        const Room& room = level.rooms[currentRoom];

        drawRoom(room);
        showConnections(room);

        cout << "\n1. Moverse a otra sala" << endl;
        cout << "0. Salir de la exploracion" << endl;

        int option;

        cout << "Opcion: ";

        if (!(cin >> option)) {

            cout << "Entrada no valida." << endl;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            continue;
        }

        if (option == 0) {
            exploring = false;
        }

        else if (option == 1) {

            cout << "Introduce el numero de la sala destino: ";

            if (!(cin >> destination)) {

                cout << "Entrada no valida." << endl;

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                continue;
            }

           
            if (destination >= 0 &&
                destination < static_cast<int>(level.rooms.size()) &&
                isConnected(room, destination)) {

                currentRoom = destination;

                cout << "Te desplazas a la sala "
                    << currentRoom << endl;
            }

            else {
                cout << "No puedes acceder a esa sala." << endl;
            }
        }

        else {
            cout << "Opcion no valida." << endl;
        }
    }
}



#pragma endregion ASCII

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

    cout << "\n=== GENERACION ALEATORIA ===" << endl;

    EnemyFactory factory;

    factory.reset();

    for (int i = 0; i < 15; i++) {

        unique_ptr<Enemy> enemy = factory.createRandomEnemy(1);

        showEnemy(*enemy);
    }


    cout << "\n=== PRUEBA DE COMBATE ===" << endl;

    Player player;

    Weapon sword("Espada", 8);

    Weapon fireSword("Espada de fuego", 8,
        { Element::Fire });

    EnemyFactory combatFactory;

    unique_ptr<Enemy> enemy1 =
        combatFactory.createRandomEnemy(1);

    unique_ptr<Enemy> enemy2 =
        combatFactory.createRandomEnemy(1);

    cout << "\nJugador: 100 PS" << endl;

    cout << "\nPrimer enemigo:" << endl;
    showEnemy(*enemy1);

    player.attack(*enemy1, sword);

    cout << "\nSegundo enemigo:" << endl;
    showEnemy(*enemy2);

    player.attack(*enemy2, fireSword);

    cout << "\n=== GENERACION DE MAZMORRA ===" << endl;

    RandomLevelBuilder builder;

    builder.reset();
    builder.buildRooms(difficulty);
    builder.connectRooms();
    builder.populateRooms(1);

    Level dungeon = builder.getResult();

    
    for (const Room& room : dungeon.rooms) {

        cout << "\n--- Sala " << room.id << " ---" << endl;

        cout << "Dimensiones: "
            << room.width << "x" << room.height << endl;

        cout << "Oro: " << room.gold << endl;

        cout << "Salas conectadas: ";

        for (int connection : room.connections) {
            cout << connection << " ";
        }

        cout << "\nEnemigos: "
            << room.enemies.size() << endl;

        for (const auto& enemy : room.enemies) {
            showEnemy(*enemy);
        }
    }

    cout << "\n=== EXPLORACION DE LA MAZMORRA ===" << endl;

    exploreLevel(dungeon);

    return 0;
}