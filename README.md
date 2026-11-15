# Explicación uso de patrones

Durante el diseño de la solución se barajaron cuatro patrones: builder, factory, bridge y strategy. Cada uno de ellos se ha seleccionado con un objetivo y funciones concretas. Despés, tras analizar las necesidades reales, se descarto la implementación de strategy y bridge por añadir un exceso de complejidad para el alcance actual del proyecto.

**Builder:**
Problema del enunciado:  un nivel se compone de varias salas y su estructura depende de la dificultad. También hay que definir dimensiones, conexiones y distribución de enemigos y tesoro.

Aplicación:el constructor RandomLevelBuilder organiza la generación en fases: preparar el nivel, crear las salas, conectarlas y poblarlas con enemigos y oro. Al final entrega el objeto Level construido.
Justificación: utilizó builder porque la creación de un nivel requiere varios pasos relacionados y depende de diversos parámetros como la dificultad. Este patrón me permite encapsular dicho proceso y separar las distintas lógicas. Dichas lógicas son la generación de salas, las conexiones y el contenido en sí, propio de cada nivel de dificultad.

Ventaja: la clase Level representa el resultado, pero no necesita contener toda la lógica que decide cuántas salas crear, cómo conectarlas o cómo distribuir los elementos. Esto facilita modificar el procedimiento de generación sin cambiar la representación del nivel.

**Factory:**
Problema del enunciado:  hay distintos tipos de enemigos, ya sean slimes, orcos…, y cada familia tiene variantes y datos propios, como elemento, enfado , nivel, tesoro dropeable y clase armadura.

Aplicación: se define una abstracción EnemyCreator y creadores concretos como SlimeCreator, OrcCreator. Cada creador implementa create() y devuelve un enemigo del tipo correspondiente. EnemyFactory selecciona qué creador utilizar durante la generación aleatoria.

Justificación: utilizó factory porque existen varias familias de enemigos con procesos de inicialización diferentes. Al emplear el patrón se delega la creación a creadores especializados evitando que el generador del nivel tenga que encargarse de ir creando cada tipo concreto de enemigo.

Ventaja: el generador de niveles no tiene que conocer los detalles internos de cada constructor de enemigo. Para incorporar otra familia, se puede crear un nuevo EnemyCreator y su producto concreto, manteniendo separada la lógica de generación del nivel.

**Bridge (no implementado):**
Problema del enunciado:   las armas pueden producir distintos tipos de daño y los ataques elementales añaden dados d4. Si cada combinación de arma y daño se representara con una clase distinta, el número de clases crecería rápidamente.

Aplicación: Se había planteado utilizar Weapon como abstracción y DamageRoll como implementación, con distintas variantes para el daño físico y elemental.

Justificación: Aunque Bridge permitiría separar ambas dimensiones y combinarlas libremente, lo he considerado excesivo para el alcance actual de la práctica. Solamente existen dos formas sencillas de calcular el daño: una tirada física y una tirada elemental que añade un d4 por cada elemento. Por ello, he decidido mantener una clase Weapon más sencilla, que almacena el nombre del arma, el dado de daño y sus posibles elementos.

Conclusión: Bridge sería útil si el sistema de armas y daños adquiriese más variantes y complejidad, pero actualmente su introducción no compensa el aumento de código y abstracciones.

**Strategy(no implementado):**
Problema del enunciado:   los enemigos no siempre atacan de la misma forma. En particular, los Orcos enfadados pueden realizar más de un ataque por turno.

Aplicación: la interfaz AttackStrategy define el comportamiento basado en el número de ataques por turno. SingleAttack representa un ataque y OrcFrenzy adapta el número de ataques según el nivel de enfado del Orco.


Justificación: Aunque Strategy permitiría encapsular comportamientos intercambiables y asignarlos independientemente a los enemigos, en esta prueba solamente necesitamos una regla especial: los Orcos con un nivel de enfado elevado realizan dos ataques. Para resolverlo se ha utilizado el polimorfismo tradicional, definiendo el método virtual attacksPerTurn() en Enemy y sobrescribiéndolo en Orc. De esta manera, cada enemigo puede determinar su número de ataques sin introducir una jerarquía adicional de estrategias.

Ventaja de la solución elegida: Se mantiene la posibilidad de ampliar el comportamiento de los enemigos sobrescribiendo sus métodos, evitando abstracciones adicionales que no aportan suficiente valor en el proyecto actual.

**¿Cómo colaboran los cuatro patrones?**
Los patrones no compiten entre sí: intervienen en momentos diferentes del funcionamiento del juego.
-Builder construye el nivel y solicita que sus salas se conecten y se rellenen.
-Durante el proceso de población, Factory Method crea los enemigos que se colocarán en las salas.

El combate utiliza el polimorfismo de las clases de enemigos y un cálculo de daño sencillo basado en los dados del arma. Los ataques enemigos también aprovechan la herencia para determinar su número de golpes.

De esta forma, ambos patrones tienen responsabilidades diferenciadas y contribuyen a mantener separadas la generación del nivel y la creación de sus enemigos.

