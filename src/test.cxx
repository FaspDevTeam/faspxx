/**
 * a test file
 */

#include <iostream>
#include <cstring>

int main() {
    char *s = "123456789";
    std::cout << strlen(s) << std::endl;

    int n = strlen(s);

    int k = 0;
    while (k < n && '0' <= s[k] && s[k] <= '9') { std::cout<<s[k]<<"  ";k++; }

    return 0;
}