#ifndef SAT_SDL_H_
#define SAT_SDL_H_

#include <sat_status.h>
#include <sat_sdl_events.h>

typedef struct sat_sdl_t sat_sdl_t;

sat_status_t sat_sdl_init (sat_sdl_t **object, const char *title, uint16_t width, uint16_t height);
sat_status_t sat_sdl_set_background (sat_sdl_t *object, sat_sdl_color_t color);
sat_status_t sat_sdl_set_image (sat_sdl_t *object, const char *name, sat_sdl_rectangle_t rectangle);
sat_status_t sat_sdl_get_image_rectangle_in_position (sat_sdl_t *object, const char *name, sat_sdl_coordinate_t coordinate, sat_sdl_rectangle_t *rectangle);
sat_status_t sat_sdl_image_add (sat_sdl_t *object, char *name, const char *file, sat_sdl_image_type_t type);
sat_status_t sat_sdl_font_add (sat_sdl_t *object, char *name, const char *file, uint16_t size);
sat_status_t sat_sdl_set_text (sat_sdl_t *object, char *font, char *text, sat_sdl_rectangle_t rectangle);
sat_status_t sat_sdl_set_event_key_pressed (sat_sdl_t *object, sat_sdl_event_on_key_pressed_t on_key_pressed);
sat_status_t sat_sdl_set_event_key_released (sat_sdl_t *object, sat_sdl_event_on_key_released_t sat_sdl_event_on_key_released_t);
sat_status_t sat_sdl_set_event_mouse_event (sat_sdl_t *object, sat_sdl_event_on_mouse_event_t on_mouse_event);
sat_status_t sat_sdl_wait_key_pressed (sat_sdl_t *object, sat_sdl_key_t *key);
sat_status_t sat_sdl_set_context (sat_sdl_t *object, void *context);
sat_status_t sat_sdl_set_viewport (sat_sdl_t *object, sat_sdl_rectangle_t rectangle);
sat_status_t sat_sdl_clear (sat_sdl_t *object);
sat_status_t sat_sdl_draw_point (sat_sdl_t *object, sat_sdl_point_t point);
sat_status_t sat_sdl_draw_line (sat_sdl_t *object, sat_sdl_line_t line);
sat_status_t sat_sdl_draw_rectangle (sat_sdl_t *object, sat_sdl_rectangle_t rectangle);
sat_status_t sat_sdl_draw_circle (sat_sdl_t *object, sat_sdl_circle_t circle);
sat_status_t sat_sdl_draw (sat_sdl_t *object);
sat_status_t sat_sdl_scan_events (sat_sdl_t *object);
sat_status_t sat_sdl_stop (sat_sdl_t *object);
sat_status_t sat_sdl_close (sat_sdl_t *object);
sat_status_t sat_sdl_audio_add (sat_sdl_t *object, char *filename, char *name, sat_sdl_audio_type_t type);
sat_status_t sat_sdl_audio_control (sat_sdl_t *object, char *name, sat_sdl_audio_control_t control);

sat_status_t sat_sdl_delay (sat_sdl_t *object, uint32_t delay);

sat_status_t sat_sdl_animate_add (sat_sdl_t *object, char *filename, char *name, sat_sdl_animate_properties_t properties);
sat_status_t sat_sdl_animate_add_states (sat_sdl_t *object, char *name, char *state, sat_sdl_frame_position_t *positions, uint8_t size);

sat_status_t sat_sdl_animate_draw (sat_sdl_t *object, char *name, char *state, sat_sdl_coordinate_t coordinate, sat_sdl_flip_t flip);

#endif/* SAT_SDL_H_ */
