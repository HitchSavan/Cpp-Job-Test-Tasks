// Темный Даниил Александрович
// 11.01.2023, 20 минут
// Удаление подряд идущих дубликатов символов в строке

#include <iostream>

void RemoveDubs(char* str) {
    int i = 1, j = 1;

    if (str[0] == '\0') {
        return;
    }

    while (str[i] != '\0') {

        if (str[i] != str[i-1]) {
            str[j] = str[i];
            j++;
        }

        i++;
    }

    str[j] = str[i];
}

int main() {
    char str1[] = "AAA BBB    AAA";
    char str2[] = "AaaaAAB";
    char str3[] = "";

    RemoveDubs(str1);
    RemoveDubs(str2);
    RemoveDubs(str3);

    std::cout << str1 << std::endl << str2 << std::endl << str3;

    return 0;
}