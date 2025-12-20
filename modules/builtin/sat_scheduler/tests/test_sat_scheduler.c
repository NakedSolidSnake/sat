#include <sat.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

static void sat_scheduler_handler_counter (void *object);
static void sat_scheduler_handler_counter_fast (void *object);

int main (int argc, char *argv [])
{
    sat_scheduler_t scheduler;

    sat_status_t status = sat_scheduler_init (&scheduler);
    assert (sat_status_get_result (&status) == true);

    sat_scheduler_open (&scheduler, &(sat_scheduler_args_t){.event_amount = 3});
    assert (sat_status_get_result (&status) == true);

    sat_scheduler_add_event (&scheduler, &(sat_scheduler_event_t)
                                         {
                                            .name = "counter",
                                            .object = &scheduler,
                                            .handler = sat_scheduler_handler_counter,
                                            .type = sat_scheduler_type_periodic,
                                            .timeout = 10
                                         });
    assert (sat_status_get_result (&status) == true);

    sat_scheduler_add_event (&scheduler, &(sat_scheduler_event_t)
                                         {
                                            .name = "counter fast",
                                            .object = &scheduler,
                                            .handler = sat_scheduler_handler_counter_fast,
                                            .type = sat_scheduler_type_periodic,
                                            .timeout = 1
                                         });
    assert (sat_status_get_result (&status) == true);

    sat_scheduler_add_event (&scheduler, &(sat_scheduler_event_t)
                                         {
                                            .name = "counter one shot",
                                            .object = &scheduler,
                                            .handler = sat_scheduler_handler_counter_fast,
                                            .type = sat_scheduler_type_one_shot,
                                            .timeout = 1
                                         });
    assert (sat_status_get_result (&status) == true);

    status = sat_scheduler_start (&scheduler);
    assert (sat_status_get_result (&status) == true);

     sleep (1);

    uint16_t amount = 0;
    sat_scheduler_get_amount (&scheduler, &amount);

    assert (amount == 2);

    do
    {
        usleep (10);
        status = sat_scheduler_is_running (&scheduler);
    } while (sat_status_get_result (&status) == true);

    status = sat_scheduler_close (&scheduler);
    assert (sat_status_get_result (&status) == true);


    return 0;
}

static void sat_scheduler_handler_counter (void *object)
{
    sat_scheduler_t *scheduler = (sat_scheduler_t *)object;
    static uint8_t counter = 0;

    if (counter >= 9)
    {
        sat_scheduler_stop (scheduler);
    }

    counter++;
}

static void sat_scheduler_handler_counter_fast (void *object)
{
    (void)object;

    static uint8_t counter = 0;
    counter ++;
}