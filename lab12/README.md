**ЛАБОРАТОРНАЯ РАБОТА 12**
LIBS
-

## Структура лабы

```
lab12/
├── warehouse.h   — структуры и объявления функций
├── warehouse.c   — реализация функций
└── main.c        — основной файл
```

## Описание функций

`fillArray` — заполняет массив структур\
`printArray` — выводит таблицу на экран\
`bubbleSort` — сортировка пузырьком\
`measureSort` — замеряет время сортировки

## Как запустить

**Динамическая библиотека (.dll):**
```bash
gcc -c warehouse.c -o warehouse.o          # компилируем в объектный файл
gcc -shared -o warehouse.dll warehouse.o   # создаём динамическую библиотеку
gcc main.c -L. -lwarehouse -o dynamic.exe  # компилируем и линкуем с библиотекой
./dynamic.exe
```

**Статическая библиотека (.a):**
```bash
gcc main.c warehouse.c -c                  # компилируем в объектные файлы
ar r libMY_LOG.a warehouse.o main.o        # создаём статическую библиотеку
gcc main.o -L. -lMY_LOG -o static.exe      # линкуем
./static.exe
```
