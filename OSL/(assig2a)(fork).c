#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
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

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid > 0) {
        // Parent process
        printf("Parent process ID: %d\n", getpid());
        printf("Parent sorting array...\n");
        bubble_sort(arr, n);
        printf("Parent sorted array: ");
        print_array(arr, n);

        // Wait for child process to complete
        wait(NULL);  // Avoids zombie state
        printf("Parent process complete.\n");

    } else {
        // Child process
        printf("Child process ID: %d\n", getpid());
        printf("Child's parent process ID: %d\n", getppid());
        sleep(1);  // Delay to show orphan or zombie state effects

        printf("Child sorting array...\n");
        bubble_sort(arr, n);
        printf("Child sorted array: ");
        print_array(arr, n);

        // Child process exits
        printf("Child process complete.\n");
        exit(0);
    }

    return 0;
}
