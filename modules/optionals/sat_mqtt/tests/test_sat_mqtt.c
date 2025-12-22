#include <stdio.h>
#include <sat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define SAT_MQTT_TEST_DIRECTORY     1
#define SAT_MQTT_EXIT               "exit"

void consumer_on_receive (void *object, sat_mqtt_message_t message)
{
    if (strcmp (message.payload, SAT_MQTT_EXIT) == 0)
    {
        system ("docker compose down");
        exit (0);
    }

    printf ("%s received: %s\n", (char *)object, message.payload);
}

void *producer_run (void *parameters);
void *consumer_run (void *parameters);

int main (int argc ,char *argv[])
{
    pthread_t producer;
    pthread_t consumer;

    char *docker_path = argv [SAT_MQTT_TEST_DIRECTORY];

    chdir (docker_path);

    system ("docker compose up -d");

    pthread_create (&producer, NULL, producer_run, NULL);
    pthread_create (&consumer, NULL, consumer_run, NULL);

    pthread_join (producer, NULL);
    pthread_join (consumer, NULL);

    return 0;
}

void *producer_run (void *parameters)
{
    sat_mqtt_t mqtt;
    int counter = 0;

    sat_mqtt_init (&mqtt);
    sat_mqtt_open (&mqtt, &(sat_mqtt_args_t)
                          {
                            .address = "tcp://localhost:1883",
                            .id = "publisher",
                            .persistence = sat_mqtt_persistence_type_default,
                            .type = sat_mqtt_type_publisher
                          });

    while (counter ++ < 5)
    {
        sat_mqtt_publish (&mqtt, &(sat_mqtt_message_t)
                                 {
                                    .topic = "test",
                                    .payload = "Test",
                                    .size = 4,
                                    .qos = sat_mqtt_qos_type_at_least_one
                                 });
        
        usleep (10000);
    }

    sat_mqtt_publish (&mqtt, &(sat_mqtt_message_t)
                             {
                             .topic = "test",
                             .payload = SAT_MQTT_EXIT,
                             .size = 4,
                             .qos = sat_mqtt_qos_type_at_least_one
                             });

    sat_mqtt_close (&mqtt);

    return NULL;
}

void *consumer_run (void *parameters)
{
    sat_mqtt_t mqtt;

    sat_mqtt_init (&mqtt);
    sat_mqtt_open (&mqtt, &(sat_mqtt_args_t)
                          {
                            .address = "tcp://localhost:1883",
                            .id = "consumer",
                            .persistence = sat_mqtt_persistence_type_default,
                            .type = sat_mqtt_type_subscriber,
                            .events = 
                            {
                                .on_receive = consumer_on_receive
                            },
                            .data = "Consumer"
                          });

    sat_mqtt_subscribe (&mqtt, "test", sat_mqtt_qos_type_at_least_one);

    sat_mqtt_close (&mqtt);

    return NULL;
}