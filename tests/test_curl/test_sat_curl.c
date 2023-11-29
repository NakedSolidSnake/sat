#include <stdio.h>
#include <sat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CURL_TEST_DIRECTORY     1

bool sat_curl_test_get (void);
bool sat_curl_test_post (void);
bool sat_curl_test_update (void);
bool sat_curl_test_delete (void);

int main (int argc, char *argv [])
{
    char * docker_path = argv [CURL_TEST_DIRECTORY];

    chdir (docker_path);

    system ("docker compose build");
    system ("docker compose up -d");

    assert (sat_curl_test_get () == true);
    sat_curl_test_post ();
    sat_curl_test_update ();
    sat_curl_test_delete ();

    system ("docker compose down");
    
    return 0;
}


bool sat_curl_test_get (void)
{
    char buffer [1024] = {0};

    char *expected = "[\n  {\n    \"id\": 1,\n    \"name\": \"John Doe\"\n  },\n  {\n    \"id\": 2,\n    \"name\": \"Jane Doe\"\n  }\n]";

    sat_curl_t curl;
    sat_curl_args_t args = 
    {
        .buffer = buffer,
        .size = 1024
    };

    sat_curl_init (&curl);
    sat_curl_open (&curl, &args);

    sat_curl_request (&curl, sat_curl_method_get, "http://localhost:3000/persons", NULL);

    sat_curl_close (&curl);

    return strcmp (buffer, expected) == 0;
}

bool sat_curl_test_post (void)
{
    char buffer [1024] = {0};

    char *json = "{\"name\":\"John Doe\"}";

    sat_curl_t curl;
    sat_curl_args_t args = 
    {
        .buffer = buffer,
        .size = 1024
    };

    sat_curl_init (&curl);
    sat_curl_open (&curl, &args);

    sat_curl_header_add (&curl, "Content-Type: application/json; charset=utf-8");
    sat_curl_request (&curl, sat_curl_method_post, "http://localhost:3000/persons", json);

    sat_curl_close (&curl);

    return true;

}

bool sat_curl_test_update (void)
{
    char buffer [1024] = {0};

    char *json = "{\"name\":\"Jane Doe\"}";

    sat_curl_t curl;
    sat_curl_args_t args = 
    {
        .buffer = buffer,
        .size = 1024
    };

    sat_curl_init (&curl);
    sat_curl_open (&curl, &args);

    sat_curl_header_add (&curl, "Content-Type: application/json; charset=utf-8");
    sat_curl_request (&curl, sat_curl_method_put, "http://localhost:3000/persons/3", json);

    sat_curl_close (&curl);

    return true;
}

bool sat_curl_test_delete (void)
{
    char buffer [1024] = {0};

    char *json = "{\"name\":\"Jane Doe\"}";

    sat_curl_t curl;
    sat_curl_args_t args = 
    {
        .buffer = buffer,
        .size = 1024
    };

    sat_curl_init (&curl);
    sat_curl_open (&curl, &args);

    sat_curl_request (&curl, sat_curl_method_delete, "http://localhost:3000/persons/3", json);

    sat_curl_close (&curl);

    return true;
}
