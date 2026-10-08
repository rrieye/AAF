# База даних: Реляційна модель із підтримкою FULL_JOIN (Варіант 6)

Цей проєкт є консольною програмою, яка надає інтерфейс для взаємодії із колекціями даних. Усі стовпці у створюваних таблицях підтримують виключно рядковий тип даних.

## Команда розробників
- Гінкул Максим ФБ-45 (@miles-lll)
- Рєпко Єлизавета ФІ-43 (@rrieye)

## Структура проєкту
- `src/` — вихідний код програми (CLI, парсер, логіка БД).
- `tests/` — тестові файли для перевірки коректності синтаксичного аналізу.

## Збірка та запуск
Проєкт компілюється безпосередньо через компілятор g++. Усі команди в програмі мають завершуватися символом `;`.

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

**Додавання рядка**

```
INSERT [INTO] table_name ("value" [, ...]);
```

**Вибірка**

```
SELECT FROM table_name_1
  [FULL_JOIN table_name_2 ON t1_column = t2_column]
  [WHERE column_name_1 = (column_name_2 | "value")];
```

`FULL_JOIN` іде перед `WHERE`. Умова `WHERE` застосовується до результату з'єднання, а не до окремих таблиць.

### Приклад сеансу (етап 1)

```
> CREATE cats (
...   cat_id INDEXED,
...   cat_name
... ); // цей текст ігнорується
CREATE: table 'cats', columns: cat_id (indexed) cat_name
> INSERT cats (“10”, “Murzik”);
INSERT: table 'cats', values: "10" "Murzik"
> select from cats where cat_name = "Murzik";
SELECT: table 'cats', WHERE cat_name = "Murzik"
> DROP cats;
Error: Syntax error at position 0: unknown command 'DROP'
> exit;
Goodbye (: !
```

### Повідомлення про помилки

| Ситуація | Повідомлення (приклад) |
|---|---|
| невідома команда | `Syntax error at position 0: unknown command 'DROP'` |
| недопустимий символ | `Syntax error at position 7: unexpected character '_'` |
| відсутня потрібна частина | `Syntax error at position 29: expected column name or value in quotes after '=', got end of command` |
| відсутня кома чи дужка | `Syntax error at position 17: expected ',' or ')' after value, got string "2"` |
| зайвий текст після команди | `Syntax error at position 17: expected end of command, got 'x'` |
| ключове слово як ім'я | `Syntax error at position 7: 'select' is a reserved word, expected table name` |
| однакові стовпці в `CREATE` | `Syntax error at position 16: duplicate column 'a'` |
| незакритий рядок | `Syntax error: Unclosed quote detected.` |
| немає `;` в кінці вводу | `Syntax error: Missing ';' at the end of command.` |

Позиція в повідомленні рахується в тексті команди після нормалізації пробілів (послідовності пробільних символів стискаються в один пробіл), тому для багаторядкових команд вона наближена.
