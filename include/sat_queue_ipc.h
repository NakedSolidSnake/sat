#ifndef SAT_QUEUE_IPC_H_
#define SAT_QUEUE_IPC_H_

#include <sat_status.h>
#include <stdlib.h>

typedef enum 
{
    sat_queue_ipc_flags_read_and_write = 0666,

} sat_queue_ipc_flags_t;

typedef struct 
{
    int key;
    int id;
    bool initialized;

} sat_queue_ipc_t;

typedef struct 
{
    int key;
    sat_queue_ipc_flags_t flags;

} sat_queue_ipc_args_t;

typedef struct 
{
    long int type;
    char *buffer;
    size_t size;

} sat_queue_data_t;

sat_status_t sat_queue_ipc_open (sat_queue_ipc_t *object, sat_queue_ipc_args_t *args);
sat_status_t sat_queue_ipc_send (sat_queue_ipc_t *object, sat_queue_data_t *data);
sat_status_t sat_queue_ipc_receive (sat_queue_ipc_t *object, sat_queue_data_t *data);
sat_status_t sat_queue_ipc_destroy (sat_queue_ipc_t *object);

#endif/* SAT_QUEUE_IPC_H_ */
