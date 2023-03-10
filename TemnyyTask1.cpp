// Темный Даниил Александрович
// 11.01.2023, 1 час
// Двоичное представление целого числа

#include <iostream>

void convertToBinary(int a) {
    int len = sizeof(a)*8;
    int mask = 1 << (len - 2);

    std::cout << (a < 0 ? "1" : "0");

    for (int i = 0; i < len - 1; i++) {
        std::cout << (a & mask ? "1" : "0");

        mask = mask >> 1;
    }

    std::cout << std::endl;
}

int main() {

    convertToBinary(-163);
    convertToBinary(163);

    return 0;
}