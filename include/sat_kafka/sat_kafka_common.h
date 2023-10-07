#ifndef SAT_KAFKA_COMMON_H_
#define SAT_KAFKA_COMMON_H_

#include <glib-2.0/glib.h>
#include <librdkafka/rdkafka.h>

void sat_kafka_load_group (rd_kafka_conf_t *configuration, GKeyFile *key_file, const char *group);

#endif/* SAT_KAFKA_COMMON_H_ */
