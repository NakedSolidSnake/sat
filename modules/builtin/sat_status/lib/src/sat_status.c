#include <sat_status.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

sat_status_t sat_status_set (sat_status_t *const object, bool result, const char *const where, const char *const motive)
{
    assert (object);

    object->result = result;
    object->where = (where == NULL || where[0] == '\0') ? "" : where;
    object->motive = (motive == NULL || motive[0] == '\0') ? "No error" : motive;

    return *object;
}

bool sat_status_get_result (const sat_status_t *const object)
{
    assert (object);
    return object->result;
}

const char *sat_status_get_motive (const sat_status_t *const object)
{
    assert (object);
    return object->motive;
}

const char *sat_status_get_where (const sat_status_t *const object)
{
    assert (object);
    return object->where;
}

sat_status_t sat_status_success (sat_status_t *const object)
{
    return sat_status_set (object, true, "", "");
}

sat_status_t sat_status_failure (sat_status_t *const object, const char *const motive)
{
    return sat_status_set (object, false, "", motive);
}

sat_status_t sat_status_failure_where (sat_status_t *const object, const char *const where, const char *const motive)
{
    return sat_status_set (object, false, where, motive);
}

void sat_status_abort (sat_status_t *const object, const char *const message)
{
    assert (object);
    
    fprintf (stderr, "Irrecoverable error: %s\n", object->motive);
    abort ();
}