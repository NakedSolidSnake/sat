#ifndef SAT_PROCESS_H_
#define SAT_PROCESS_H_

#include <unistd.h>
#include <sat_status.h>

#define SAT_PROCESS_NAME_SIZE   128
#define SAT_PROCESS_ARGS_SIZE   1024

typedef enum 
{
    sat_process_mode_no_recover,
    sat_process_mode_recover,
    sat_process_mode_critical

} sat_process_mode_t;

typedef enum 
{
    sat_process_spawn_type_no_args,
    sat_process_spawn_type_with_args,

} sat_process_spawn_type_t;

typedef struct 
{
    char name [SAT_PROCESS_NAME_SIZE + 1];
    char args [SAT_PROCESS_ARGS_SIZE + 1];
    sat_process_mode_t mode;
    pid_t pid;
    bool initialized;

    sat_process_spawn_type_t type; 
    
} sat_process_t;

typedef struct 
{
    char name [SAT_PROCESS_NAME_SIZE + 1];
    char args [SAT_PROCESS_ARGS_SIZE + 1];
    sat_process_mode_t mode;

} sat_process_args_t;

sat_status_t sat_process_create (sat_process_t *object, sat_process_args_t *args);
sat_status_t sat_process_spawn (sat_process_t *object);

#endif/* SAT_PROCESS_H_ */
