/**
 * @file utils.h
 * @author Julio Manuel Paredes Lugo
 * @date 5 Jul 2019
 * @brief File containing definitions and declarations of all functions required.
 *
 * @see http://fnch.users.sourceforge.net/doxygen_c.html
 */

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
pthread_t time_id;
pthread_t server, client[4];
extern int via;
pthread_mutex_t lock;

struct arg_struct
{
    float mean;
    int via_id;
};

struct server_struct
{
    int flag;
    int via_id;
};

enum sync_case
{
    EXIT,             /**< enum value Exit option. */
    CRITICAL_SECTION, /**< enum value Critical section option. */
    CLIENT_SERVER     /**< enum value Client server section option. */
};

/// Simple helper function for final print
void print_stats();

/** 
 * @brief Random number generator
 * Implementation straight from 
 * http://en.wikipedia.org/wiki/Poisson_distribution#Generating_Poisson-distributed_random_variables
 * which credits Knuth.
 * @param lambda mean value
 * @return Random number generated
 * Time complexity is O(lambda), which is not optimal.
*/
double ran_expo(double);


/**
 * @brief Reactive-like function that controls option selected in main menu.
 * @param op option selected in main menu.
 * @see sim_critical_section()
 * @see sim_client_server()
 * @return Code indicating execution status
 */
int handle_option_selected(int, float);

/**
 * Global clock.
 * @param op a polymorphic pointer.
 * @see sim_critical_section()
 * @see sim_client_server()
 * @return Code indicating execution status
 */
void *time_mngr(void *);

/// Entity generator every random second
void *generate_input(void *);


/**
 * @brief Via thread
 * 
 * Every via has been modeled using a single thread for
 * each one. This is used in critical section simulation.
 * 
 * @param op a polymorphic pointer.
 * @see sim_critical_section()
 * @return Code indicating execution status
 */
void *via_control(void *);

/**
 * @brief Via thread
 * 
 * A Singleton-styled server which handles access permissions
 * to the shared area
 * 
 * @param op a polymorphic pointer.
 * @see sim_client_server()
 * @return Code indicating execution status
 */
void *server_thread(void *);

/**
 * @brief Via thread
 * 
 * Every via has been modeled using a single thread for
 * each one. In this case, every thread is a client.
 * 
 * @param op a polymorphic pointer.
 * @see sim_client_server()
 * @return Code indicating execution status
 */
void *client_thread(void *);


/**
 * @brief Via thread
 * 
 * Critical section threads setUp
 * 
 * @param float a mean value
 * @see sim_critical_section()
 * @see sim_client_server()
 * @return Code indicating execution status
 */
int sim_critical_section(float);

/**
 * @brief Via thread
 * 
 * Client server threads setUp
 * 
 * @param float a mean value
 * @see sim_critical_section()
 * @see sim_client_server()
 * @return Code indicating execution status
 */
int sim_client_server(float);

#endif