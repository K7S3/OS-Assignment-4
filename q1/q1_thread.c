#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/shm.h>
#include <pthread.h>

struct param
{
    long long start;
    long long *arr;
    long long end;
};
typedef struct param param;
long long swapper(long long *var1, long long *var2)
{
    long long temp;
    temp = *var1;
    *var1 = *var2;
    *var2 = temp;
}
long long partition(long long start, long long arr[], long long end)
{
    long long i = start;
    long long pivot = arr[end];
    for (long long j = start; j <= end - 1; j += 1)
    {
        if (pivot >= arr[j])
        {
            i += 1;
            swapper(&arr[i - 1], &arr[j]);
        }
    }
    swapper(&arr[i], &arr[end]);
    return i;
}
long long insertion_sort(long long start, long long arr[], long long end)
{
    long long key = 0;
    for (long long i = start + 1; i <= end; i++)
    {
        long long j = i - 1;
        key = arr[i];
        for (; j >= 0 && key <= arr[j]; j -= 1)
        {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = key;
        //    for(long long i =start;i<=end;i++) {
        //     printf("%lld ", arr[i]);
        //     }
        //     printf("\n");
    }
}
long long check_sort(long long start, long long arr[], long long end)
{
    long long state = 0; //Sorted
    for (long long i = start; i <= end - 1; i++)
    {
        if (arr[i] <= arr[i + 1])
        {
            continue;
        }
        else
        {
            state = 1;
            break;
        }
    }
    return state;
}
void *quick_sort(void *params)
{
    long long start = ((param *)params)->start;
    long long *arr = ((param *)params)->arr;
    long long end = ((param *)params)->end;
    if (end - start + 1 < 5)
    {
        insertion_sort(start, arr, end);
        return NULL;
    }
    if (end > start)
    {
        long long piv = partition(start, arr, end);
        // printf("%lld\n", piv);
        pthread_t left_thread, right_thread;
        param *tmp1 = (param *)malloc(sizeof(param));
        tmp1->start = start;
        tmp1->arr = arr;
        tmp1->end = piv - 1;
        pthread_create(&left_thread, NULL, quick_sort, (void *)tmp1);
        // quick_sort(start, arr, piv - 1);
        param *tmp2 = (param *)malloc(sizeof(param));
        tmp2->start = piv + 1;
        tmp2->arr = arr;
        tmp2->end = end;
        pthread_create(&right_thread, NULL, quick_sort, (void *)tmp2);
        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);
        // quick_sort(piv + 1, arr, end);
    }
}
long long main()
{
    long long n;
    puts("Enter the size of the array.");
    scanf("%lld", &n);
    long long *arr = (long long *)malloc(sizeof(long long) * n);
    long long option;
    clock_t start_time;
    clock_t end_time;
    srand(time(NULL));

    printf("Choose an Option: \n1. Randomly intialize %lld elements.\n2. Input %lld integers.\n", n, n);
    scanf("%lld", &option);
    if (option == 1)
    {
        long long i = 0;
        while (i < n)
        {
            arr[i++] = rand();
        }
    }
    else
    {
        long long i = 0;
        while (i < n)
        {
            scanf("%lld", &arr[i++]);
        }
    }
    struct timespec ts;
    param *tmp = (param *)malloc(sizeof(param));
    pthread_t main_thread;
    tmp->start = 0;
    tmp->arr = arr;
    tmp->end = n - 1;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    long double st = ts.tv_nsec / (1e9) + ts.tv_sec;

    pthread_create(&main_thread, NULL, quick_sort, (void *)tmp);
    pthread_join(main_thread, NULL);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    long double en = ts.tv_nsec / (1e9) + ts.tv_sec;
    // quick_sort(start, arr, piv - 1);
    if (check_sort(0, arr, n - 1) == 0)
    {
        puts("Sorting is Successful");
    }
    else
    {
        puts("Sorting is Unsuccessful");
    }
    printf("The time taken to sort is %Lf\n", en - st);
    // printf("The time taken to sort is %f\n", ((double)(end_time - start_time)) / CLOCKS_PER_SEC);
    // long long arr[] = {10, 7, 4, 5, 1, 6};
    // long long n = sizeof(arr)/sizeof(long long);
    // quick_sort(0, arr, n-1);
    // for (long long i = 0; i < n; i++)
    // {
    //     printf("%lld ", a[i]);
    // }
    return 0;
}