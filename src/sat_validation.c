#include <sat_validation.h>
#include <string.h>

sat_status_t sat_validation_init (sat_validation_t *object)
{
    sat_status_t status = sat_status_set (&status, false, "sat validation init error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_validation_t));
        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_validation_add (sat_validation_t *object, sat_criteria_t criteria)
{
    sat_status_t status = sat_status_set (&status, false, "sat validation add error");

    if (object != NULL && criteria != NULL && object->amount < SAT_VALIDATION_CRITERIA_AMOUNT)
    {
        object->criterias [object->amount] = criteria;
        object->amount++;

        sat_status_set (&status, true, "");
    }

    return status;
}

sat_status_t sat_validation_verify (sat_validation_t *object, void *data)
{
    sat_status_t status = sat_status_set (&status, true, "");

    if (object != NULL && data != NULL)
    {
        for (uint8_t i = 0; i < object->amount; i++)
        {
            status = object->criterias [i](data);
            if(sat_status_get_result (&status) == false)
                break;
        }
    }

    return status;
}