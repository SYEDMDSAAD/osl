#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check if there are arguments passed
    if (argc < 2) {
        printf("No array elements received.\n");
        return 1;
    }

    printf("Array in reverse order: ");
    for (int i = argc - 1; i > 0; i--) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    return 0;
}
