#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int arr[SIZE] = {29, 7, 67, 79, 3, 17, 56, 5, 4, 2};
int result[SIZE];

typedef struct {
    int from;
    int to;
} Range;

void insertion_sort(int from, int to) {
    for (int i = from + 1; i < to; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= from && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void* sort_half(void* arg) {
    Range* range = (Range*)arg;
    insertion_sort(range->from, range->to);
    pthread_exit(NULL);
}

void* merge_halves(void* arg) {
    int mid = SIZE / 2;
    int i = 0, j = mid, k = 0;

    while (i < mid && j < SIZE)
        result[k++] = (arr[i] < arr[j]) ? arr[i++] : arr[j++];

    while (i < mid) result[k++] = arr[i++];
    while (j < SIZE) result[k++] = arr[j++];

    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2, thread_merge;
    Range first_half = {0, SIZE / 2};
    Range second_half = {SIZE / 2, SIZE};

    pthread_create(&thread1, NULL, sort_half, &first_half);
    pthread_create(&thread2, NULL, sort_half, &second_half);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_create(&thread_merge, NULL, merge_halves, NULL);
    pthread_join(thread_merge, NULL);

    printf(" This is final sorted array:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    return 0;
}
