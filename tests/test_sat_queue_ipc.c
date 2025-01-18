#include <sat.h>
#include <assert.h>
#include <string.h>

#define BUFFER_SIZE         100

int main (int argc, char *argv [])
{
    sat_queue_ipc_t queue;

    char buffer_send [BUFFER_SIZE] = "Hello World";
    char buffer_receive [BUFFER_SIZE];

    sat_status_t status = sat_queue_ipc_open (&queue, &(sat_queue_ipc_args_t)
                                                      {
                                                        .key = 65,
                                                        .flags = sat_queue_ipc_flags_read_and_write
                                                      });
    assert (sat_status_get_result (&status) == true);

    status = sat_queue_ipc_send (&queue, &(sat_queue_data_t)
                                         {
                                            .type = 1,
                                            .buffer = buffer_send,
                                            .size = BUFFER_SIZE
                                         });
    assert (sat_status_get_result (&status) == true);

    status = sat_queue_ipc_receive (&queue, &(sat_queue_data_t)
                                            {
                                                .type = 1,
                                                .buffer = buffer_receive,
                                                .size = BUFFER_SIZE
                                            });
    assert (sat_status_get_result (&status) == true);

    assert (strcmp (buffer_send, buffer_receive) == 0);

    status = sat_queue_ipc_destroy (&queue);
    assert (sat_status_get_result (&status) == true);

    return 0;
}