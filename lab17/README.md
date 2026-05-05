Берем код с гитхаба: https://github.com/Gabe3231/C-Calculator
Немного переделываем: функции переносим в файлы `calculator.cpp` и `calculator.h` и меняем их тип на `int`, чтобы было возможно тестировать; логику переносим в main.cpp

Для тестирования используем фреймворк Google Test.
Пишем по 2 `EXPECT_EQ` на каждую функцию. Итого получилось 4 теста.

Для сборки используем Cmake:
Импортируем `Google Test`
```cmake
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0 
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)
```
Далее линкуем библиотеку и приложение и запускаем тесты
