#include <sat_opengl_container_texture.h>
#include <string.h>

void sat_opengl_container_texture_init (sat_opengl_container_texture_t *object, const char *name)
{
    memset (object, 0, sizeof (sat_opengl_container_texture_t));

    object->name = name;
}

sat_status_t sat_opengl_container_texture_add (sat_opengl_container_texture_t *object, sat_opengl_texture_t *texture)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl container texture add error");

    if (object->amount < SAT_OPENGL_CONTAINER_TEXTURE_AMOUNT)
    {
        memcpy (&object->list [object->amount], texture, sizeof (sat_opengl_texture_t));

        object->amount ++;

        sat_status_set (&status, true, "");
    }

    return status;
}

void sat_opengl_container_texture_enable (sat_opengl_container_texture_t *object)
{
    for (uint8_t i = 0; i < object->amount; i++)
    {
        sat_opengl_texture_activate (&object->list [i], i);
        sat_opengl_texture_enable (&object->list [i]);
    }
}