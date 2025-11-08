#ifndef SAT_LOG_H
#define SAT_LOG_H

#include <sat_status.h>
#include <stdarg.h>
#include <stdio.h>


extern int sat_log_enabled;


#define sat_log_error(fmt, ...) \
    do { if (sat_log_enabled >= 1) fprintf (stderr, "[ERROR] %d:%s(): " fmt "\n", __LINE__, __func__, ##__VA_ARGS__); } while (0)
    // do { if (sat_log_enabled >= 1) fprintf (stderr, "[ERROR] %s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)
        
#define sat_log_warn(fmt, ...) \
    do { if (sat_log_enabled >= 2) fprintf (stderr, "[WARN] %d:%s(): " fmt "\n", __LINE__, __func__, ##__VA_ARGS__); } while (0)
    // do { if (sat_log_enabled >= 2) fprintf (stderr, "[WARN] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while (0)

#define sat_log_info(fmt, ...) \
    do { if (sat_log_enabled >= 3) fprintf (stdout, "[INFO] " fmt "\n", ##__VA_ARGS__); } while (0)

#define sat_log_debug(fmt, ...) \
    do { if (sat_log_enabled >= 4) fprintf (stderr, "[DEBUG] %d:%s(): " fmt "\n", __LINE__, __func__, ##__VA_ARGS__); } while (0)
    // do { if (sat_log_enabled >= 4) fprintf (stderr, "[DEBUG] %s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)


typedef enum 
{
  sat_log_type_none, 
  sat_log_type_default,
  sat_log_type_custom  
} sat_log_type_t;

typedef enum 
{
    sat_log_level_fatal,
    sat_log_level_error,
    sat_log_level_warning,
    sat_log_level_information,
    sat_log_level_debug,
    sat_log_level_trace
} sat_log_level_t;

typedef void (*sat_log_logger_t) (sat_log_level_t level, const char *format, va_list list);

typedef struct 
{
    sat_log_type_t type;
    sat_log_logger_t logger;
} sat_log_t;

typedef struct 
{
    sat_log_type_t type;
    sat_log_logger_t logger;
} sat_log_args_t;

sat_status_t sat_log_init (sat_log_t *object);
sat_status_t sat_log_open (sat_log_t *object, sat_log_args_t *args);
sat_status_t sat_log_registry (sat_log_t *object, sat_log_level_t level, const char *format, ...);
sat_status_t sat_log_close (sat_log_t *object);


sat_status_t sat_log_init_by_env (const char *const fallback);

#endif/* SAT_LOG_H */
