#include <stdio.h>

int main() {
    printf("P1\n");
    printf("500 500\n");
    for (int i = 0; i < 500; i++) {
        for (int j = 0; j < 500; j++) {
            if (i == 1 && j == 0) {
                printf("1");
            }
            else if (i == 0 || i == 499 || j == 0 || j == 499) {
                printf("0");
            }
            else {
                printf("1");
            }
        }
        
            printf("\n");
    }
}