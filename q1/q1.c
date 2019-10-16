#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    if (end > start)
    {
        long long piv = partition(start, arr, end);
        // printf("%lld\n", piv);
        quick_sort(start, arr, piv - 1);
        quick_sort(piv + 1, arr, end);
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
       clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
       long double st = ts.tv_nsec/(1e9)+ts.tv_sec;
    quick_sort(0, arr, n - 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    long double en = ts.tv_nsec/(1e9)+ts.tv_sec;
    if (check_sort(0, arr, n - 1) == 0)
    {
        puts("Sorting is Successful");
    }
    else
    {
        puts("Sorting is Unsuccessful");
    }
    printf("The time taken to sort is %Lf\n", en - st);
    // printf("The time taken to sort is %f\n", ((double)(end_time-start_time))/CLOCKS_PER_SEC);
    // long long arr[] = {10, 7, 4, 5, 1, 6};
    // long long n = sizeof(arr)/sizeof(long long);
    // quick_sort(0, arr, n-1);
    // for (long long i = 0; i < n; i++)
    // {
    //     printf("%lld ", a[i]);
    // }
    return 0;
}