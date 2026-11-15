Explicación uso de patrones

Se han seleccionado cuatro patrones: builder, factory, bridge y strategy. Cada uno de ellos se ha seleccionado con un objetivo y funciones concretas.

Builder:
Problema del enunciado:  un nivel se compone de varias salas y su estructura depende de la dificultad. También hay que definir dimensiones, conexiones y distribución de enemigos y tesoro.

Aplicación:el constructor RandomLevelBuilder organiza la generación en fases: preparar el nivel, crear las salas, conectarlas y poblarlas con enemigos y oro. Al final entrega el objeto Level construido.
Justificación: utilizó builder porque la creación de un nivel requiere varios pasos relacionados y depende de diversos parámetros como la dificultad. Este patrón me permite encapsular dicho proceso y separar las distintas lógicas. Dichas lógicas son la generación de salas, las conexiones y el contenido en sí, propio de cada nivel de dificultad.

Ventaja: la clase Level representa el resultado, pero no necesita contener toda la lógica que decide cuántas salas crear, cómo conectarlas o cómo distribuir los elementos. Esto facilita modificar el procedimiento de generación sin cambiar la representación del nivel.

Factory:
Problema del enunciado:  hay distintos tipos de enemigos, ya sean slimes, orcos…, y cada familia tiene variantes y datos propios, como elemento, enfado , nivel, tesoro dropeable y clase armadura.

Aplicación: se define una abstracción EnemyCreator y creadores concretos como SlimeCreator, OrcCreator. Cada creador implementa create() y devuelve un enemigo del tipo correspondiente. EnemyFactory selecciona qué creador utilizar durante la generación aleatoria.

Justificación: utilizó factory porque existen varias familias de enemigos con procesos de inicialización diferentes. Al emplear el patrón se delega la creación a creadores especializados evitando que el generador del nivel tenga que encargarse de ir creando cada tipo concreto de enemigo.

Ventaja: el generador de niveles no tiene que conocer los detalles internos de cada constructor de enemigo. Para incorporar otra familia, se puede crear un nuevo EnemyCreator y su producto concreto, manteniendo separada la lógica de generación del nivel.

Bridge:
Problema del enunciado:   las armas pueden producir distintos tipos de daño y los ataques elementales añaden dados d4. Si cada combinación de arma y daño se representara con una clase distinta, el número de clases crecería rápidamente.

Aplicación: Weapon actúa como abstracción y mantiene una referencia a DamageRoll, que define cómo se calcula el daño. MeleeWeapon y RangedWeapon representan tipos de arma; PhysicalDamage y ElementalDamage implementan formas distintas de calcular la tirada.

Justificación: se iba a utilizar Bridge porque el tipo de arma y la forma de calcular el daño son dos dimensiones que pueden variar de forma independiente. Al separarlas evitaría tener que crear una clase de arma para cada combinación y me permite añadir armas o tipos de daño sin modificar jerarquía. El caso es que su uso me parecio excesivo en este momento ya que solo hay daño fisico o elemental y, en el caso de elemntal solo se va a añadir un dado extra, nada más.

Ventaja:se puede combinar una espada o un arco con una implementación de daño físico o elemental sin crear una clase específica para cada combinación. Además, como extra,  ElementalDamage puede reutilizarse para los ataques de Slimes elementales, que también añaden un d4 por elemento. Sin embargo, he decidido no llegar a implementarlo debido a que no justifica del todo el aumento en complejidad.

Strategy:
Problema del enunciado:   los enemigos no siempre atacan de la misma forma. En particular, los Orcos enfadados pueden realizar más de un ataque por turno.

Aplicación: la interfaz AttackStrategy define el comportamiento basado en el número de ataques por turno. SingleAttack representa un ataque y OrcFrenzy adapta el número de ataques según el nivel de enfado del Orco.


Justificación: utilizó strategy ya que el comportamiento de ataque varía según que enemigos. Es cierto que quizá cabe la posibilidad que su empleo sea exagerado, pero permite una mayor modularidad en caso de querer expandir el número de enemigos. Por otro lado al encapsular dichos comportamientos estas se pueden asignar de forma independiente  y cambiar comportamientos sin concentrar todas las reglas en una cadena de condiciones dentro de la lógica general de combate

Ventaja: la lógica de combate consulta la estrategia asignada al enemigo sin necesitar conocer sus condiciones internas. En el futuro se pueden añadir estrategias para ataques especiales, múltiples golpes u otros patrones sin reescribir el turno general de los enemigos.
¿Cómo colaboran los cuatro patrones?
Los patrones no compiten entre sí: intervienen en momentos diferentes del funcionamiento del juego.
Builder construye el nivel y solicita que sus salas se conecten y se rellenen.
Durante el proceso de población, Factory Method crea los enemigos que se colocarán en las salas.
Cuando el jugador ataca, el arma utiliza la implementación DamageRoll asociada para calcular el daño; Bridge mantiene separadas ambas jerarquías.
Durante el turno enemigo, cada enemigo consulta su AttackStrategy para determinar cuántos ataques realiza.

