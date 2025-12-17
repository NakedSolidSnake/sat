#include <sat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

static void *sat_channel_producer (void *args);
static void *sat_channel_consumer (void *args);

typedef struct
{
    sat_channel_t channel;
    bool running;
} sat_channel_test_t;

int main (int argc, char *argv[])
{
    sat_channel_test_t channel = {.running = true};

    pthread_t producer;
    pthread_t consumer;

    sat_status_t status = sat_channel_init (&channel.channel);
    assert (sat_status_get_result (&status) == true);

    status = sat_channel_open (&channel.channel, &(sat_channel_args_t) {});
    assert (sat_status_get_result (&status) == true);

    pthread_create (&producer, NULL, sat_channel_producer, &channel);
    pthread_create (&consumer, NULL, sat_channel_consumer, &channel);

    pthread_join (producer, NULL);
    pthread_join (consumer, NULL);

    status = sat_channel_close (&channel.channel);
    assert (sat_status_get_result (&status) == true);

    return 0;
}

static void *sat_channel_producer (void *args)
{
    sat_channel_test_t *channel = (sat_channel_test_t *)args;
    uint8_t buffer [] = "Hello World";
    sat_status_t status;

    while (channel->running == true)
    {
        status = sat_channel_write (&channel->channel, buffer, strlen ((char *)buffer));
        assert (sat_status_get_result (&status) == true);

        usleep (1000);
    }

    return NULL;
}

static void *sat_channel_consumer (void *args)
{
    sat_channel_test_t *channel = (sat_channel_test_t *)args;
    uint8_t buffer [128] = {0};
    sat_status_t status;

    while (channel->running == true)
    {
        memset (buffer, 0, sizeof (buffer));

        status = sat_channel_read (&channel->channel, buffer, sizeof (buffer));
        assert (sat_status_get_result (&status) == true);

        if (strcmp ((char *)buffer, "Hello World") == 0)
        {
            channel->running = false;
        }
    }

    return NULL;
}
