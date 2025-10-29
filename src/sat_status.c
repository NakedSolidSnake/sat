#include <sat_status.h>
#include <string.h>
#include <assert.h>

sat_status_t sat_status_set (sat_status_t *object, bool result, char *motive)
{
    assert (object);

    object->result = result;
    object->motive = (motive == NULL || strlen (motive) == 0) ? "No error" : motive;

    return *object;
}

bool sat_status_get_result (sat_status_t *object)
{
    assert (object);
    return object->result;
}

char *sat_status_get_motive (sat_status_t *object)
{
    assert (object);
    return object->motive;
}

sat_status_t sat_status_success (sat_status_t *object)
{
    return sat_status_set (object, true, "");
}

sat_status_t sat_status_failure (sat_status_t *object, char *motive)
{
    return sat_status_set (object, false, motive);
}