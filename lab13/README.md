**ЛАБОРАТОРНАЯ РАБОТА 13**
MAKE, CMAKE
-

## Структура лабы

```
lab13/
├── include/
│   └── warehouse.h   — структуры и объявления функций
├── src/
│   ├── warehouse.c   — реализация функций
│   └── main.c        — основной файл
├── Makefile
└── CMakeLists.txt
```

---

### Оценка 3 — Makefile

Создаём динамическую библиотеку + команда `clean`

```bash
make        # сборка
make clean  # очистка
```

---

### Оценка 4 — CMake

> ⚠️ CMake запускать через WSL/Linux — конфликт `sys/time.h` на Windows

Выбор типа библиотеки через флаг `-DBUILD_SHARED_LIBS` (по умолчанию `ON`):

**Динамическая (.so / .dll):**
```bash
cmake -B build
cmake --build build
```

**Статическая (.a):**
```bash
cmake -B build -DBUILD_SHARED_LIBS=OFF
cmake --build build
```

Опция `-DCMAKE_BUILD_TYPE` управляет режимом компиляции:

| Режим | Описание |
|-------|----------|
| `Debug` | Отладочные символы, оптимизации отключены — для разработки |
| `Release` | Без отладки, оптимизации включены — для финальной сборки |

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

---

### Оценка 5

**Coming soon...** *(or maybe not)*
