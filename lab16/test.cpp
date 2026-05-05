#include <iostream>

int main() {
    // \033[31m - красный, \033[32m - зеленый, \033[0m - сброс цвета
    std::cout << "\033[31mКрасный текст\033[0m" << std::endl;
    std::cout << "\033[32mЗеленый текст\033[0m" << std::endl;
    std::cout << "\033[1;34mЖирный синий\033[0m" << std::endl;
    
    return 0;
}
