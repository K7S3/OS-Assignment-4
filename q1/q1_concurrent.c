#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <time.h>

long long swapper(long long *var1, long long *var2)
{
    long long temp;
    temp = *var1;
    *var1 = *var2;
    *var2 = temp;
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
long long quick_sort(long long start, long long arr[], long long end)
{

    if (end - start + 1 < 5)
    {
        insertion_sort(start, arr, end);
        return 0;
    }

    pid_t right_id;
    long long piv = partition(start, arr, end);
    int status;
    pid_t left_id;
    // printf("%lld\n", piv);
    left_id = fork();
    if (left_id == 0) // Child Process
    {
        quick_sort(start, arr, piv - 1);
        exit(0);
    }
    else if (0 > left_id)
    {
        printf("Left Child cannot be created due to error in Forking\n");
        exit(-1);
    }
    else
    {
        right_id = fork();
        if (right_id == 0)
        {
            quick_sort(piv + 1, arr, end);
            exit(0);
        }
        else if (0 > right_id)
        {
            printf("Right Child cannot be created due to error in Forking\n");
            exit(-2);
        }
    }
    waitpid(left_id, &status, 0);
    waitpid(right_id, &status, 0);
}
long long main()
{
    long long shmid, n;
    int zero = 0;
    clock_t start_time;
    clock_t end_time;
    key_t key = IPC_PRIVATE;
    printf("Enter the size of the array\n");
    scanf("%lld", &n);
    long long *arr = (long long *)malloc(sizeof(long long) * n);
    size_t SHM_SIZE = sizeof(long long) * n;
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < zero)
    {
        perror("shmget :");
        exit(1);
    }
    else if ((arr = shmat(shmid, NULL, zero)) == (long long *)-1)
    {
        perror("shmat :");
        exit(1);
    }
    long long option;
    srand(time(NULL));

    struct timespec ts;
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

    // puts("The Array before sorting is : ");
    // for (long long i = 0; i <= n - 1; i++)
    // {
    //     printf("%lld ", arr[i]);
    // }
    // printf("\n");

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    long double st = ts.tv_nsec / (1e9) + ts.tv_sec;
    quick_sort(0, arr, n - 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    long double en = ts.tv_nsec / (1e9) + ts.tv_sec;
    if (check_sort(0, arr, n - 1) == 0)
    {
        puts("The Sorting is Successful");
    }
    else
    {
        puts("The Sorting is Unsuccessful");
    }
    printf("The Time Taken to sort is= %Lf\n", en - st);
    // printf("The time taken to sort is %f\n", ((double)(end_time-start_time))/CLOCKS_PER_SEC);
    // puts("The Array after sorting is : ");
    // for (long long i = 0; i <= n - 1; i++)
    // {
    //     printf("%lld ", arr[i]);
    // }
    // printf("\n");
    return 0;
}