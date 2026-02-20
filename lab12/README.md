**ЛАБОРАТОРНАЯ РАБОТА 12**
LIBS
-

**3 файла:**\
*warehouse.h* - заголовочный файл, содержит описание структур и функций\
*warehouse.c* - дополнительный файл, содержащий функции\
*main.c* - основной файл, содержит основной код\

**Описание функций**\
*fillArray* - заполняет массив\
*printArray* - выводит таблицу\
*bubbleSort* - сортирует пузырьком\
*measureSort* - считает время

**Как запустить?**\
with dynamic:
'''gcc -c warehouse.c -o warehouse.o''' - компилирует в машинный код
'''gcc -shared -o warehouse.dll warehouse.o''' - создает саму библиотеку
'''gcc main.c -L. -lwarehouse -o dynamic.exe''' - компилирует main.c и связывает с библиотекой

with static:
''' gcc main.c warehouse.c -c '''
''' ar r libMY_LOG.a warehouse.o main.o'''
''' gcc main.o -L. -lMY_LOG -o static'''