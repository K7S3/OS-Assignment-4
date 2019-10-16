#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
const int NOTRIDING = 0;
const int RIDING = 1;
struct Rider
{
    int id;
    int max_wait_time;
    int paid;
    int toPay;
    int ride_time;
    int state;
    int (*book_cab)(char *cab_type, int i);
    void (*make_payment)();
};
//Given in the Question
const int WAITSTATE = 0;
const int ONRIDEPREMIER = 1;
const int ONRIDEPOOLFULL = 2;
const int ONRIDEPOOLONE = 3;

struct Cab
{
    int state;
    void (*end_ride)();
};
struct Server
{
    int id;
    int isFree;
    int who;
};

typedef struct Rider Rider;
typedef struct Cab Cab;
typedef struct Server Server;
Rider *Riders;
Cab *Cabs;
Server *Servers;
// struct timespec cur_time;

int no_of_riders;
int no_of_cabs;
int no_of_servers;
int left;
pthread_t *riders_thread;
pthread_t *servers_thread;
pthread_mutex_t cab_mutex[100007];
pthread_mutex_t *rider_mutex;
pthread_mutex_t payment_mutex[100007];
int accept_ride(char *cab_type, int id)
{

    if (Riders[id].state == RIDING)
    {

        return 0;
    }

    for (int i = 0; i <= no_of_cabs - 1; i++)
    {

        pthread_mutex_lock(&cab_mutex[id]);
        if (Riders[id].state == NOTRIDING)
        {

            int cab_no = 0;
            if (cab_type == "Premier")
            {
                cab_no = 1;
            }
            else if (cab_type == "Pool")
            {
                cab_no = 2;
            }
            else
            {
                printf("The cab type selected by rider %d does not exist", id);
                return 1;
            }

            if (Cabs[i].state == WAITSTATE && cab_no == 1)
            {
                //Premier
                printf("Rider %d books cab %d succesfully.\n", id, i);
                Riders[id].state = RIDING;
                Cabs[i].state = ONRIDEPREMIER;
            }
            else if (Cabs[i].state == WAITSTATE && cab_no == 2)
            {
                //Pool
                printf("Rider %d books cab %d succesfully.\n", id, i);
                Riders[id].state = RIDING;
                Cabs[i].state = ONRIDEPOOLONE;
            }
            else if (Cabs[i].state == ONRIDEPOOLONE && cab_no == 2)
            {
                //Pool with n
                printf("Rider %d books cab %d succesfully.\n", id, i);
                Riders[id].state = RIDING;
                Cabs[i].state = ONRIDEPOOLFULL;
            }
        }
        pthread_mutex_unlock(&cab_mutex[id]);
    }
}
void accept_payment(int id)
{
    for (int i = 0; i < no_of_servers; i += 1)
    {
        pthread_mutex_lock(&payment_mutex[i]);
        if (Servers[i].isFree == 1)
        {
            Servers[i].isFree = 0;
            Servers[i].who = id;
            Riders[id].toPay = 0;
            printf("Payment intiated by rider %d to server %d\n", id, i);
            pthread_mutex_unlock(&payment_mutex[i]);
            return;
        }
        pthread_mutex_unlock(&payment_mutex[i]);
    }
}
void end_ride(int id)
{
    sleep(Riders[id].ride_time);
    printf("Ride is over for rider id %d\n", id);
    Riders[id].toPay = 1;
    accept_payment(id);
}
void timeout(int id)
{
    printf("TimeOut for Rider %d\n", id);
}
int wait_time[10000007];
struct timespec start_time[10000007];
struct timespec cur_time[10000007];
int book_cab(char *cab_type, int id)
{

    wait_time[id] = 0;
    // struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time[id]);
    printf("Rider %d wants to book a %s cab. \n", id, cab_type);
    while ((wait_time[id] < Riders[id].max_wait_time) && (Riders[id].state == NOTRIDING))
    {

        accept_ride(cab_type, id);

        // puts("sda");
        clock_gettime(CLOCK_MONOTONIC, &cur_time[id]);
        wait_time[id] = cur_time[id].tv_sec - start_time[id].tv_sec;
    }
    if (Riders[id].state == NOTRIDING)
    {
        timeout(id);
        left--;
    }
    if (Riders[id].state == RIDING)
    {
        end_ride(id);
        left--;
    }

    return 0;
}
void *riders_thread_function(void *rid)
{
    int id = ((Rider *)rid)->id;
    char *cabtype = (rand() % 2) ? "Premier" : "Pool";
    Riders[id].book_cab(cabtype, id);
}
void *servers_thread_function(void *sid)
{
    int server_id = ((Server *)sid)->id;
    while (1)
    {
        if (left <= 0)
        {
            break;
        }
        if (Servers[server_id].isFree == 0)
        {
            sleep(2);
            printf("Payment completed by rider %d to server %d\n", Servers[server_id].who, server_id);
            Servers[server_id].isFree = 1;
            Servers[server_id].who = -1;
            // break;
        }
    }
}

int main()
{
    printf("Enter the Number of Riders: ");
    scanf("%d", &no_of_riders);
    left = no_of_riders;
    Riders = (Rider *)malloc(sizeof(Rider) * no_of_riders);
    riders_thread = (pthread_t *)malloc(sizeof(pthread_t) * no_of_riders);
    printf("Enter the Number of Cabs: ");
    scanf("%d", &no_of_cabs);
    Cabs = (Cab *)malloc(sizeof(Cab) * no_of_cabs);
    // cab_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * no_of_cabs);
    printf("Enter the Number of Servers: ");
    scanf("%d", &no_of_servers);
    int i = 0;
    Servers = (Server *)malloc(sizeof(Server) * no_of_servers);
    servers_thread = (pthread_t *)malloc(sizeof(pthread_t) * no_of_servers);
    //Memory Allocation
    for (int i = 0; i <= no_of_cabs - 1;)
    {
        pthread_mutex_init(&cab_mutex[i++], NULL);
    }
    for (i = 0; i <= no_of_riders - 1; i += 1)
    {
        Riders[i].id = i;
        Riders[i].paid = 0;
        Riders[i].toPay = 0;
        Riders[i].max_wait_time = rand() % 5;
        Riders[i].state = NOTRIDING;
        Riders[i].ride_time = rand() % 10;
        Riders[i].book_cab = book_cab;
        pthread_create(&riders_thread[i], NULL, riders_thread_function, &Riders[i]);
    }
    for (int i = 0; i <= no_of_servers - 1; i += 1)
    {
        Servers[i].id = i;
        Servers[i].isFree = 1;
        Servers[i].who = -1;
        pthread_create(&servers_thread[i], NULL, servers_thread_function, &Servers[i]);
    }

    for (int i = 0; i < no_of_riders; i += 1)
    {
        pthread_join(riders_thread[i], NULL);
    }
    for (i = 0; i < no_of_servers; i += 1)
    {
        pthread_join(servers_thread[i], NULL);
    }

    return 0;
}