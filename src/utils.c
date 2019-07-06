#include <utils.h>
#include <stdio.h>

int via;
int queues[4] = {0};

double ran_expo(double lambda)
{
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

/** 
 * Implementation straight from 
 * http://en.wikipedia.org/wiki/Poisson_distribution#Generating_Poisson-distributed_random_variables
 * which credits Knuth.
 * @param lambda mean value
 * @return Random number generated
 * Time complexity is O(lambda), which is not optimal.
*/

/* RANDTYPE poisson(double lambda){
  RANDTYPE k=0;
  double L=exp(-lambda), p=1;
  do {
    ++k;
    p *= GOODRAND/(double)GOODRANDMAX;
  } while (p > L);
  return --k;
}
*/

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

void* generate_input(void* arg)
{
    struct arg_struct *a = (struct arg_struct *)arg;
    int total_cars = 5;
    int counter = 0, time = 0;
    int t = (int) ran_expo(a -> mean);
    while(counter < total_cars)
    {
        if (t == time)
        {
            printf("A car has arrived at time: %d to via %d!\n", time, a -> via_id);
            ++counter;
            queues[a -> via_id] = counter;                
            t += (int) ran_expo(a -> mean);
            while (t == time)
            {
                printf("A car has arrived at time: %d to via %d!\n", time, a -> via_id);
                t += (int) ran_expo(a -> mean);
                ++counter;
                queues[a -> via_id] = counter;                
            }
            
        }
        sleep(1);
        ++time;
    }
    
}

void* via_control(void* arg)
{
    struct arg_struct *a = (struct arg_struct *)arg;
    pthread_t generator;

    int err = pthread_create(&(generator), NULL, &generate_input, (void *)a);
    if (err != 0)
        return NULL;
    int time = 0;
    printf("Waiting for unlock... %d\n", a -> via_id);
    pthread_mutex_lock(&lock);
        printf("Via %d allowed to access critical zone.\n", a -> via_id);
        
        while(time < 25)
        {
            for (int i = 0; i < queues[a -> via_id]; i++)
            {
                printf("Passing car %d from %d through shared area...\n", i + 1, a -> via_id);
                sleep(1);
                ++time;    
            }
            sleep(1);
            ++time;
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
    
    pthread_mutex_destroy(&lock);
    return SUCCESS_CODE;
}

/**
 * Simulate Client Server syncronization method for several processes.
 * @see sim_critical_section()
 * @return Code indicating execution status
 */
int sim_client_server(float mean)
{
    struct arg_struct args[4];
    int err;
  
    for (int i = 0; i < 4; i++)
    {    
        args[i].mean = mean;
        args[i].via_id = i;
        err = pthread_create(&(tid[i]), NULL, &via_control, (void *)&args[i]);
        if (err != 0)
            return ERROR_CODE;
    }   

    

    return SUCCESS_CODE;
}
