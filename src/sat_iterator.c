#include <sat_iterator.h>
#include <string.h>

static sat_status_t sat_iterator_is_base_object_valid (sat_iterator_t *const object, const sat_iterator_base_t *const base);
static bool sat_iterator_is_base_index_valid (const sat_iterator_base_t *const base);
static bool sat_iterator_is_base_address_valid (const sat_iterator_base_t *const base);

static sat_status_t sat_iterator_get_info (sat_iterator_t *const object, const sat_iterator_base_t *const base);
static void sat_iterator_get_info_index (sat_iterator_t *const object, const sat_iterator_base_t *const base);
static void sat_iterator_get_info_address (sat_iterator_t *const object, const sat_iterator_base_t *const base);


sat_status_t sat_iterator_open (sat_iterator_t *const object, const sat_iterator_base_t *const base)
{
    sat_status_t status = sat_status_failure (&status, "sat iterator open error");

    if (object != NULL)
    {
        memset (object, 0, sizeof (sat_iterator_t));

        do 
        {
            status = sat_iterator_is_base_object_valid (object, base);
            sat_status_break_on_error (status);

            status = sat_iterator_get_info (object, base);
            sat_status_break_on_error (status);

            object->base = base;
            object->initialized = true;

            sat_status_success (&status);

        } while (false);
    }

    return status;
}

void *sat_iterator_next (sat_iterator_t *const object)
{
    void *item = NULL;

    if (object != NULL && object->initialized == true)
    {
        if (object->type == sat_iterator_type_address && object->address.current != NULL)
        {
            item = object->base->get_data (object->address.current);

            object->address.current = object->address.next;

            object->address.next = object->base->get_next_address (object->base->object, object->address.current);
        }

        else if (object->type == sat_iterator_type_index && object->index < object->amount)
        {
            item = object->base->next (object->base, object->index);

            object->index ++;
        }
    }

    return item;
}

static sat_status_t sat_iterator_is_base_object_valid (sat_iterator_t *const object, const sat_iterator_base_t *const base)
{
    sat_status_t status;

    do
    {
        object->type = sat_iterator_type_unknown;

        if (base == NULL)
        {
            sat_status_failure (&status, "sat iterator error: base is NULL");
            break;
        }

        if (base->object == NULL)
        {
            sat_status_failure (&status, "sat iterator error: base object is NULL");
            break;
        }

        if (sat_iterator_is_base_index_valid (base) == true)
        {
            object->type = sat_iterator_type_index;
            sat_status_success (&status);
        }

        else if (sat_iterator_is_base_address_valid (base) == true)
        {
            object->type = sat_iterator_type_address;
            sat_status_success (&status);
        }

        if (object->type == sat_iterator_type_unknown)
        {
            sat_status_failure (&status, "sat iterator error: base functions are invalid");
            break;
        }

        sat_status_success (&status);

    } while (false);

    return status;
}

static sat_status_t sat_iterator_get_info (sat_iterator_t *const object, const sat_iterator_base_t *const base)
{
    sat_status_t status = sat_status_failure (&status, "sat iterator get info error");

    if (object->type == sat_iterator_type_index)
    {
        sat_iterator_get_info_index (object, base);

        sat_status_success (&status);
    }

    else if (object->type == sat_iterator_type_address)
    {
        sat_iterator_get_info_address (object, base);

        sat_status_success (&status);
    }

    return status;
}

static bool sat_iterator_is_base_index_valid (const sat_iterator_base_t *const base)
{
    bool status = false;

    if (base->get_amount != NULL &&
        base->next != NULL)
    {
        status = true;
    }

    return status;
}

static bool sat_iterator_is_base_address_valid (const sat_iterator_base_t *const base)
{
    bool status = false;

    if (base->get_address != NULL &&
        base->get_next_address != NULL &&
        base->get_data != NULL)
    {
        status = true;
    }

    return status;
}

static void sat_iterator_get_info_index (sat_iterator_t *const object, const sat_iterator_base_t *const base)
{
    object->index = 0;
    object->amount = base->get_amount (base->object);
}

static void sat_iterator_get_info_address (sat_iterator_t *const object, const sat_iterator_base_t *const base)
{
    object->address.current = base->get_address (base->object);
    object->address.next = base->get_next_address (base->object, object->address.current);
}