#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main() {
    int n;

    // Input array size
    printf("Enter number of integers to sort: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Sort the array
    bubble_sort(arr, n);

    // Print the sorted array
    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        char *args[n + 2];
        args[0] = "./child";  // The program to execute

        // Convert sorted array elements to strings and add to args
        for (int i = 0; i < n; i++) {
            char *num_str = malloc(10);
            sprintf(num_str, "%d", arr[i]);
            args[i + 1] = num_str;
        }
        args[n + 1] = NULL;  // Null-terminated array of arguments

        // Execute the child program with execve
        if (execve(args[0], args, NULL) == -1) {
            perror("execve failed");
            exit(1);
        }

    } else {
        // Parent process waits for child
        wait(NULL);
        printf("Parent process complete.\n");
    }

    return 0;
}
