#include <sat_opengl_texture.h>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static uint16_t sat_opengl_texture_get_type (sat_opengl_texture_type_t type);
static void sat_opengl_texture_configure (sat_opengl_texture_t *object);
static void sat_opengl_texture_configure_wrapping (sat_opengl_texture_t *object);
static void sat_opengl_texture_configure_filtering (sat_opengl_texture_t *object);
static sat_status_t sat_opengl_texture_load (sat_opengl_texture_t *object, sat_opengl_texture_args_t *args);
static uint16_t sat_opengl_texture_get_format (sat_opengl_texture_format_t format);

sat_status_t sat_opengl_texture_open (sat_opengl_texture_t *object, sat_opengl_texture_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl texture open error");

    glGenTextures (1, &object->id);

    sat_opengl_texture_enable (object);

    sat_opengl_texture_configure (object);

    status = sat_opengl_texture_load (object, args);
    if (sat_status_get_result (&status) == true)
    {
        object->type = args->type;
    }

    return status;
}

void sat_opengl_texture_enable (sat_opengl_texture_t *object)
{
    glBindTexture (sat_opengl_texture_get_type (object->type), object->id);
}

void sat_opengl_texture_activate (sat_opengl_texture_t *object, uint16_t position)
{
    (void) object;

    glActiveTexture (position + GL_TEXTURE0);
}

static uint16_t sat_opengl_texture_get_type (sat_opengl_texture_type_t type)
{
    uint16_t types [] = {GL_TEXTURE_2D, GL_TEXTURE_3D};

    return types [type];
}

static void sat_opengl_texture_configure (sat_opengl_texture_t *object)
{
    sat_opengl_texture_configure_wrapping (object);
    sat_opengl_texture_configure_filtering (object);
}

static void sat_opengl_texture_configure_wrapping (sat_opengl_texture_t *object)
{
    glTexParameteri (sat_opengl_texture_get_type (object->type), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (sat_opengl_texture_get_type (object->type), GL_TEXTURE_WRAP_T, GL_REPEAT);
}

static void sat_opengl_texture_configure_filtering (sat_opengl_texture_t *object)
{
    glTexParameteri (sat_opengl_texture_get_type (object->type), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (sat_opengl_texture_get_type (object->type), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static sat_status_t sat_opengl_texture_load (sat_opengl_texture_t *object, sat_opengl_texture_args_t *args)
{
    sat_status_t status = sat_status_set (&status, false, "sat opengl texture load error");

    struct 
    {
        int width;
        int height;
        int channels;        

    } properties;

    unsigned char *data = NULL;

    if (args->flip == true)
    {
        stbi_set_flip_vertically_on_load (true);
    }

    data = stbi_load (args->filename, &properties.width, &properties.height, &properties.channels, 0);

    if (data != NULL)
    {
        uint16_t format = sat_opengl_texture_get_format (args->format);

        if (GL_TEXTURE_2D == sat_opengl_texture_get_type (args->type))
        {
            glTexImage2D (GL_TEXTURE_2D, 0, format, properties.width, properties.height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap (GL_TEXTURE_2D);
        }
        else 
        {
            // TODO
            abort ();
        }

        stbi_image_free (data);

        sat_status_set (&status, true, "");
    }

    return status;
}

static uint16_t sat_opengl_texture_get_format (sat_opengl_texture_format_t format)
{
    uint16_t formats [] = {GL_RGB, GL_RGBA};

    return formats [format];
}