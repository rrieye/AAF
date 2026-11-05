# База даних: Реляційна модель із підтримкою FULL_JOIN (Варіант 6)

Цей проєкт є консольною програмою, яка надає інтерфейс для взаємодії із колекціями даних. Усі стовпці у створюваних таблицях підтримують виключно рядковий тип даних.

## Команда розробників
- Гінкул Максим ФБ-45 (@miles-lll)
- Рєпко Єлизавета ФІ-43 (@rrieye)

## Структура проєкту
- `src/main.cpp` — цикл читання команд і виклик виконання.
- `src/cli.*` — читання однієї команди до `;`.
- `src/tokenizer.*` — лексичний аналіз: рядок у токени.
- `src/parser.*` — синтаксичний аналіз: токени у структури `Create`, `Insert`, `Select`.
- `src/command.hpp` — спільні структури команд і `ParseError`.
- `src/db.*` — `Table`, `Database`, виконання `CREATE`, `INSERT`, `SELECT`.
- `src/query.*` — `WHERE` і `FULL_JOIN`.
- `src/format.*` — виведення результату у вигляді ASCII-таблиці.
- `tests/` — `test_queries.txt` (приклади команд).

## Збірка та запуск
Усі команди в програмі мають завершуватися символом `;`.

### Вимоги
* Встановлений компілятор C++ з підтримкою стандарту C++17.

### Компіляція та запуск
Відкрийте термінал у кореневій папці проєкту та виконайте команду:

#### Linux / macOS:
```bash
g++ src/*.cpp -o db_engine -std=c++17
./db_engine
```

#### Windows
```powershell
g++ src\*.cpp -o db_engine.exe -std=c++17
.\db_engine.exe
```

#### Через CMake
```bash
mkdir build && cd build
cmake ..
make
cd ..
./build/db_engine
```

Вийти з програми можна командою `EXIT;` (регістр неважливий).

### Перевірка
Команди з прикладів можна подати з файлу:
```bash
./db_engine < tests/test_queries.txt
```

## Інтерфейс

### Загальні правила

- Команда закінчується символом `;`. Усе, що йде після `;` у тому самому рядку, ігнорується.
- Команда може займати кілька рядків. Ключові слова, ідентифікатори та рядки не можна розривати між рядками.
- Пробіли, табуляції та переноси рядків між елементами команди ігноруються. Усередині лапок вони зберігаються.
- Ключові слова нечутливі до регістру (`select` = `SELECT`). Імена таблиць і стовпців та значення в лапках чутливі до регістру.
- Ідентифікатор відповідає виразу `[a-zA-Z][a-zA-Z0-9_]*`. Ключові слова (`CREATE`, `INSERT`, `INTO`, `SELECT`, `FROM`, `FULL_JOIN`, `ON`, `WHERE`, `INDEXED`) не можна використовувати як імена.
- Рядкові значення беруться в лапки: підтримуються і `"..."`, і `“...”`. Усередині рядка лапка заборонена.

### Команди

**Створення таблиці**

```
CREATE table_name (column_name [INDEXED] [, ...]);
```

Результат: `Table table_name has been created`.

**Додавання рядка**

```
INSERT [INTO] table_name ("value" [, ...]);
```

Кількість значень має дорівнювати кількості стовпців таблиці, порядок значень відповідає порядку стовпців у `CREATE`. Результат: `1 row has been inserted into table_name`.

**Вибірка**

```
SELECT FROM table_name_1
  [FULL_JOIN table_name_2 ON t1_column = t2_column]
  [WHERE column_name_1 = (column_name_2 | "value")];
```

- Без `FULL_JOIN` виводяться всі рядки таблиці з усіма стовпцями в порядку з `CREATE`.
- З `FULL_JOIN` виводяться стовпці першої таблиці, потім другої. У результат входять пари рядків зі збігом значень стовпців з `ON`, рядки першої таблиці без пари (стовпці другої заповнюються порожніми значеннями) і рядки другої таблиці без пари (стовпці першої порожні).
- `FULL_JOIN` іде перед `WHERE`. Умова `WHERE` застосовується до результату з'єднання, а не до окремих таблиць, і може посилатися на стовпці обох таблиць. Якщо назва стовпця є в обох таблицях, програма повідомляє про неоднозначність.

### Приклад сеансу

```
> CREATE owners (owner_id INDEXED, owner_name, owner_age);
Table owners has been created
> CREATE cats (
...   cat_id INDEXED,
...   cat_owner_id INDEXED,
...   cat_name
... ); // цей текст ігнорується
Table cats has been created
> INSERT INTO owners("1", "Vasya", "30");
1 row has been inserted into owners
> INSERT INTO owners("2", "Kolya", "28");
1 row has been inserted into owners
> INSERT cats ("10", "1", "Murzik");
1 row has been inserted into cats
> INSERT cats ("20", "1", "Pushok");
1 row has been inserted into cats
> INSERT cats ("30", "3", "Tom");
1 row has been inserted into cats
> SELECT FROM owners
...   FULL_JOIN cats ON owner_id = cat_owner_id;
+----------+------------+-----------+--------+--------------+----------+
| owner_id | owner_name | owner_age | cat_id | cat_owner_id | cat_name |
+----------+------------+-----------+--------+--------------+----------+
|    1     |   Vasya    |    30     |   10   |      1       |  Murzik  |
|    1     |   Vasya    |    30     |   20   |      1       |  Pushok  |
|    2     |   Kolya    |    28     |        |              |          |
|          |            |           |   30   |      3       |   Tom    |
+----------+------------+-----------+--------+--------------+----------+
> select from cats where cat_name = "Murzik";
+--------+--------------+----------+
| cat_id | cat_owner_id | cat_name |
+--------+--------------+----------+
|   10   |      1       |  Murzik  |
+--------+--------------+----------+
> DROP cats;
Parse error: Syntax error at position 0: unknown command 'DROP'
> exit;
Goodbye (: !
```

Символи `...` у прикладі лише показують, що команда продовжується на наступному рядку. Програма їх не друкує.

### Повідомлення про помилки

Помилки не завершують програму, після них можна вводити наступну команду.

**Синтаксичні помилки** (`Parse error: ...`):

| Ситуація | Повідомлення (приклад) |
|---|---|
| невідома команда | `Syntax error at position 0: unknown command 'DROP'` |
| недопустимий символ | `Syntax error at position 7: unexpected character '_'` |
| відсутня потрібна частина | `Syntax error at position 24: expected column name or value in quotes after '=', got end of command` |
| відсутня кома чи дужка | `Syntax error at position 14: expected ',' or ')' after value, got string "2"` |
| зайвий текст після команди | `Syntax error at position 14: expected end of command, got 'x'` |
| ключове слово як ім'я | `Syntax error at position 7: 'select' is a reserved word, expected table name` |
| однакові стовпці в `CREATE` | `Syntax error at position 13: duplicate column 'a'` |
| `WHERE` перед `FULL_JOIN` | `Syntax error at position 36: expected end of command, got 'FULL_JOIN'` |

**Помилки введення** (виводяться без префікса):

| Ситуація | Повідомлення |
|---|---|
| незакритий рядок | `Syntax error: Unclosed quote detected.` |
| немає `;` в кінці вводу | `Syntax error: Missing ';' at the end of command.` |

**Помилки виконання** (`Database error: ...`):

| Ситуація | Повідомлення (приклад) |
|---|---|
| таблиця вже існує | `Table cats already exists` |
| таблиці немає | `Table dogs does not exist` |
| не та кількість значень | `Insert error: expected 3 values, but got 1` |
| стовпця немає (`WHERE`) | `Column nope does not exist` |
| стовпця немає (`ON`) | `Column nope does not exist in table owners` |
| назва стовпця є в обох таблицях | `Column id is ambiguous, it exists in both tables` |

Позиція в повідомленні про синтаксичну помилку рахується в тексті команди після нормалізації пробілів (послідовності пробільних символів стискаються в один пробіл), тому для багаторядкових команд вона наближена.
