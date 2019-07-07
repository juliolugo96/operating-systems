#include <utils.h>
#include <stdio.h>

int via;
int queues[4] = {0};
int passing_cars[4] = {0};
int current_response = 0;
int time_sec = 0;

void *time_mngr(void *arg)
{
    while (time_sec < 25)
    {
        time_sec++;
        sleep(1);
    }
}

void print_stats()
{
    printf("\t\tFINAL STATISTICS\n\n");
    int counter = 0;
    for (int i = 0; i < 4; i++)
    {
        printf("\tIn queue %d: %d", i + 1, queues[i]);
        counter += queues[i];
    }

    printf("\n\n");

    for (int i = 0; i < 4; i++)
    {
        printf("\tProcessed in %d: %d", i + 1, passing_cars[i]);
        counter += passing_cars[i];
    }

    printf("\n\n");
    printf("Total cars generated: %d\n", counter);
}

/** 
 * Implementation straight from 
 * http://en.wikipedia.org/wiki/Poisson_distribution#Generating_Poisson-distributed_random_variables
 * which credits Knuth.
 * @param lambda mean value
 * @return Random number generated
 * Time complexity is O(lambda), which is not optimal.
*/
double ran_expo(double lambda)
{
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1 - u) / lambda;
}

/**
 * Reactive-like function that controls option selected in main menu.
 * @param op option selected in main menu.
 * @see sim_critical_section()
 * @see sim_client_server()
 * @return Code indicating execution status
 */
int handle_option_selected(int op, float mean)
{
    switch (op)
    {
    case CRITICAL_SECTION:
        return sim_critical_section(mean);
    case CLIENT_SERVER:
        return sim_client_server(mean);
    case EXIT:
        return SUCCESS_CODE;
    default:
        return ERROR_CODE;
    }
}

void *generate_input(void *arg)
{
    struct arg_struct *a = (struct arg_struct *)arg;
    int total_cars = 5;
    int counter = 0, time = 0;
    int t = (int)ran_expo(a->mean);
    while (counter < total_cars && current_response != -1)
    {
        if (t == time_sec)
        {
            queues[a->via_id]++;
            printf("A car has arrived at time: %d to via %d! In queue: %d\n", time, a->via_id + 1, queues[a->via_id]);
            t += (int)ran_expo(a->mean);
            while (t == time)
            {
                queues[a->via_id]++;
                printf("A car has arrived at time: %d to via %d! In queue: %d\n", time, a->via_id + 1, queues[a->via_id]);
                t += (int)ran_expo(a->mean);
            }
        }
        sleep(1);
    }

    return NULL;
}

void *via_control(void *arg)
{
    struct arg_struct *a = (struct arg_struct *)arg;
    pthread_t generator;

    int err = pthread_create(&(generator), NULL, &generate_input, (void *)a);
    if (err != 0)
        return NULL;
    printf("Waiting for unlock... %d\n", a->via_id);
    int t = 0;
    pthread_mutex_lock(&lock);
    printf("Via %d allowed to access critical zone.\n", a->via_id);
    int i = 0;
    while (t < 10)
    {

        if (queues[a->via_id] > 0)
        {
            printf("Passing car from %d through shared area...\n", a->via_id + 1);
            queues[a->via_id]--;
            passing_cars[a->via_id]++;
        }
        else
            printf("No cars currently in via %d...\n", a->via_id + 1);

        sleep(1);
        t++;
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

/**
 * Simulate Critical section syncronization method for several processes.
 * @see sim_client_server()
 * @return Code indicating execution status
 */
int sim_critical_section(float mean)
{
    int err;
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    struct arg_struct args[4];

    err = pthread_create(&(time_id), NULL, &time_mngr, NULL);
    if (err != 0)
        return ERROR_CODE;

    for (int i = 0; i < 4; i++)
    {
        args[i].mean = mean;
        args[i].via_id = i;
        err = pthread_create(&(tid[i]), NULL, &via_control, (void *)&args[i]);
        if (err != 0)
            return ERROR_CODE;
    }

    for (int i = 0; i < 4; i++)
        pthread_join(tid[i], NULL);

    pthread_join(time_id, NULL);

    pthread_mutex_destroy(&lock);
    return SUCCESS_CODE;
}

void *server_thread(void *args)
{

    printf("SERVER STARTED!: Via %d can access shared zone\n", current_response + 1);
    while (time_sec < 24)
    {
        if (time_sec > 5)
        {
            current_response++;
            printf("Access permissions changed!: %d can access shared zone, time: %d\n", current_response + 1, time_sec);
            sleep(7);
        }
    }

    current_response = -1;

    return NULL;
}

void *client_thread(void *arg)
{
    struct arg_struct *a = (struct arg_struct *)arg;
    pthread_t generator;

    int err = pthread_create(&(generator), NULL, &generate_input, (void *)a);
    if (err != 0)
        return NULL;

    int time = 0;

    if (a->via_id != current_response)
        printf("Waiting for response via %d...\n", a->via_id + 1);

    while (a->via_id != current_response)
    {
    }

    int t = 0;
    pthread_mutex_lock(&lock);

    printf("Via %d allowed to access critical zone.\n", a->via_id + 1);
    int i = 0;
    while (a->via_id == current_response)
    {

        if (queues[a->via_id] > 0)
        {
            printf("Passing car from %d through shared area...\n", a->via_id + 1);
            queues[a->via_id]--;
            passing_cars[a->via_id]++;
        }
        else
            printf("No cars currently in via %d...\n", a->via_id + 1);

        sleep(1);
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

/**
 * Simulate Client Server syncronization method for several processes.
 * @see sim_critical_section()
 * @return Code indicating execution status
 */
int sim_client_server(float mean)
{
    struct arg_struct client_args[4];
    struct server_struct server_args;
    int err;

    err = pthread_create(&(time_id), NULL, &time_mngr, NULL);
    if (err != 0)
        return ERROR_CODE;

    err = pthread_create(&(server), NULL, &server_thread, (void *)&server_args);
    if (err != 0)
        return ERROR_CODE;

    for (int i = 0; i < 4; i++)
    {
        client_args[i].mean = mean;
        client_args[i].via_id = i;
        err = pthread_create(&(client[i]), NULL, &client_thread, (void *)&client_args[i]);
        if (err != 0)
            return ERROR_CODE;
    }

    server_args.flag = true;
    server_args.via_id = 0;

    for (int i = 0; i < 4; i++)
        pthread_join(client[i], NULL);

    pthread_join(server, NULL);
    pthread_join(time_id, NULL);
    return SUCCESS_CODE;
}
