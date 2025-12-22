#include <stdio.h>
#include <sat.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define KAFKA_TEST_DIRECTORY       1
#define KAFKA_CONFIGURATION_FILE   2

static bool consumer_on_record (void *object, sat_kafka_message_t *message);

void *producer_run (void *parameters);
void *consumer_run (void *parameters);

int main (int argc, char *argv[])
{
    pthread_t producer;
    pthread_t consumer;

    char *docker_path = argv [KAFKA_TEST_DIRECTORY];
    char *configuration = argv [KAFKA_CONFIGURATION_FILE];

    chdir (docker_path);

    system ("docker compose up -d");

    sleep (2);

    pthread_create (&producer, NULL, producer_run, configuration);
    pthread_create (&consumer, NULL, consumer_run, configuration);

    pthread_join (producer, NULL);
    pthread_join (consumer, NULL);

    return 0;
}

void *producer_run (void *parameters)
{
    char *filename = (char *)parameters;   

    sat_kafka_t kafka;

    char *groups_list [] = 
    {
        "default",
    };

    sat_kafka_args_t args = 
    {
        .file = filename,
        .groups = 
        {
            .list = groups_list,
            .amount = 1
        },
        .type = sat_kafka_type_publisher
    };

    sat_kafka_init (&kafka);

    sat_kafka_open (&kafka, &args);
    
    sat_kafka_message_t message =
    {
        .key = 
        {
            .data = "test",
            .size = 4
        },
        .payload = 
        {
            .data = "Hello",
            .size = 5
        }
    };

    while (true)
    {
        sat_kafka_send (&kafka, "purchases", &message, 10 * 1000);
        sat_kafka_send (&kafka, "quickstart", &message, 10 * 1000);
        usleep (500000);
    }

    sat_kafka_close (&kafka);

    return NULL;
}

void *consumer_run (void *parameters)
{
    char *filename = (char *)parameters;

    sat_kafka_t kafka;

    char *groups_list [] = 
    {
        "default",
        "consumer"
    };

    char *topics_list [] = 
    {
        "purchases",
        "quickstart"
    };

    sat_kafka_base_t base = 
    {
        .object = NULL,
        .on_record = consumer_on_record
    };

    sat_kafka_args_t args = 
    {
        .file = filename,
        .groups = 
        {
            .list = groups_list,
            .amount = 2
        },
        .topics = 
        {
            .list = topics_list,
            .amount = 2
        },
        .type = sat_kafka_type_subscriber,
        .base = &base
    };

    sat_kafka_init (&kafka);

    sat_kafka_open (&kafka, &args);

    while (true)
    {
        sat_kafka_poll (&kafka, 500);
    }

    sat_kafka_close (&kafka);

    return NULL;
}

static bool consumer_on_record (void *object, sat_kafka_message_t *message)
{
    fprintf (stdout, "[Print] Consumed event from topic %s: key %.*s value = %.*s\n",
                    message->topic,
                    (int)message->key.size,
                    (char *)message->key.data,
                    (int)message->payload.size,
                    (char *)message->payload.data);

    fflush (stdout);

    system ("docker compose down");

    exit (0);

    return true;
}