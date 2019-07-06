#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#define true 1
#define false 0
#define ERROR_CODE -1
#define SUCCESS_CODE 0
#define GOODRAND random()
#define GOODRANDMAX INT_MAX
#define RANDTYPE short
#define ID 999
#define ROUTE "/bin/lspci"
#define SEM_ID 0x10101011

pthread_t tid[4];
extern int via;
pthread_mutex_t lock;

struct arg_struct {
    float mean;
    int via_id;
};

enum sync_case
{
    EXIT,             /**< enum value Exit option. */
    CRITICAL_SECTION, /**< enum value Critical section option. */
    CLIENT_SERVER     /**< enum value Client server section option. */
};

double ran_expo(double);
//RANDTYPE poisson(double);
int handle_option_selected(int, float);
void* generate_input(void*);
void* via_control(void*);
int sim_critical_section(float);
int sim_client_server(float);

#endif