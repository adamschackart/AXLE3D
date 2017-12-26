/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __XL_CORE_H__
#include <xl_core.h>
#endif

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

const char* xl_implementation(void)
{
    AE_STUB(); return "";
}

int xl_is_init(void)
{
    AE_STUB(); return 0;
}

void xl_init(void)
{
    AE_STUB();
}

void xl_quit(void)
{
    AE_STUB();
}

const char* xl_audio_implementation(void)
{
    AE_STUB(); return "";
}

int xl_audio_is_init(void)
{
    AE_STUB(); return 0;
}

void xl_audio_init(void)
{
    AE_STUB();
}

void xl_audio_quit(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ object types ] ~~ *
--------------------------------------------------------------------------------
*/

xl_object_type_t xl_object_type(void* object)
{
    AE_STUB(); return XL_OBJECT_TYPE_UNKNOWN;
}

size_t xl_object_count_all(void)
{
    AE_STUB(); return 0;
}

void xl_object_list_all(void** objects)
{
    AE_STUB();
}

void xl_object_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ window management ] ~~ *
--------------------------------------------------------------------------------
*/

xl_window_t* xl_window_create(int initially_visible)
{
    AE_STUB(); return NULL;
}

xl_window_t* xl_primary_window(void)
{
    AE_STUB(); return NULL;
}

void
xl_window_set_int(xl_window_t* window, xl_window_property_t property, int value)
{
    AE_STUB();
}

int
xl_window_get_int(xl_window_t* window, xl_window_property_t property)
{
    AE_STUB(); return 0;
}

void
xl_window_set_flt(xl_window_t* window, xl_window_property_t property, float value)
{
    AE_STUB();
}

float
xl_window_get_flt(xl_window_t* window, xl_window_property_t property)
{
    AE_STUB(); return 0.0f;
}

void
xl_window_set_str(xl_window_t* window, xl_window_property_t property, const char* value)
{
    AE_STUB();
}

const char*
xl_window_get_str(xl_window_t* window, xl_window_property_t property)
{
    AE_STUB(); return "";
}

void
xl_window_set_ptr(xl_window_t* window, xl_window_property_t property, void* value)
{
    AE_STUB();
}

void*
xl_window_get_ptr(xl_window_t* window, xl_window_property_t property)
{
    AE_STUB(); return NULL;
}

void
xl_window_set_img(xl_window_t* window, xl_window_property_t property, ae_image_t* value)
{
    AE_STUB();
}

ae_image_t*
xl_window_get_img(xl_window_t* window, xl_window_property_t property)
{
    AE_STUB(); return NULL;
}

void xl_window_clear(xl_window_t* window, float r, float g, float b)
{
    AE_STUB();
}

void xl_window_flip(xl_window_t* window)
{
    AE_STUB();
}

void xl_window_screenshot(xl_window_t* window, ae_image_t* image)
{
    AE_STUB();
}

void xl_window_list_all(xl_window_t** windows)
{
    AE_STUB();
}

void xl_window_close_all(void)
{
    AE_STUB();
}

void xl_window_list_textures(xl_window_t* window, xl_texture_t** textures)
{
    AE_STUB();
}

void xl_window_close_textures(xl_window_t* window)
{
    AE_STUB();
}

void xl_window_list_fonts(xl_window_t* window, xl_font_t** fonts)
{
    AE_STUB();
}

void xl_window_close_fonts(xl_window_t* window)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ shape renderer ] ~~ *
--------------------------------------------------------------------------------
*/

void xl_draw_rect_ex(xl_window_t* window, float* rect, float* color,
                    double angle, float* center, const int outline)
{
    AE_STUB();
}

void xl_draw_rect(xl_window_t* window, float* rect, float* color)
{
    AE_STUB();
}

void xl_draw_points(xl_window_t* window, float* points, size_t count, float* color)
{
    AE_STUB();
}

void xl_draw_point(xl_window_t* window, float* point, float* color)
{
    AE_STUB();
}

void xl_draw_line(xl_window_t* window, float* a, float* b, float* color)
{
    AE_STUB();
}

void xl_draw_curve(xl_window_t* window, float* a, float* b, float* color,
                        ae_ease_mode_t mode, const size_t num_divisions)
{
    AE_STUB();
}

void xl_draw_circle(xl_window_t * window, float * center, float radius,
                float * color, int outline, const size_t num_divisions)
{
    AE_STUB();
}

void xl_draw_triangle(xl_window_t * window, float* a, float* b, float* c,
                                            float* color, int outline)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ texture renderer ] ~~ *
--------------------------------------------------------------------------------
*/

xl_texture_t* xl_texture_create(xl_window_t* win, int w, int h)
{
    AE_STUB(); return NULL;
}

void
xl_texture_set_int(xl_texture_t* texture, xl_texture_property_t property, int value)
{
    AE_STUB();
}

int
xl_texture_get_int(xl_texture_t* texture, xl_texture_property_t property)
{
    AE_STUB(); return 0;
}

void
xl_texture_set_flt(xl_texture_t* texture, xl_texture_property_t property, float value)
{
    AE_STUB();
}

float
xl_texture_get_flt(xl_texture_t* texture, xl_texture_property_t property)
{
    AE_STUB(); return 0.0f;
}

void
xl_texture_set_str(xl_texture_t* texture, xl_texture_property_t property, const char* value)
{
    AE_STUB();
}

const char*
xl_texture_get_str(xl_texture_t* texture, xl_texture_property_t property)
{
    AE_STUB(); return "";
}

void
xl_texture_set_ptr(xl_texture_t* texture, xl_texture_property_t property, void* value)
{
    AE_STUB();
}

void*
xl_texture_get_ptr(xl_texture_t* texture, xl_texture_property_t property)
{
    AE_STUB(); return NULL;
}

void
xl_texture_set_img(xl_texture_t* texture, xl_texture_property_t property, ae_image_t* value)
{
    AE_STUB();
}

ae_image_t*
xl_texture_get_img(xl_texture_t* texture, xl_texture_property_t property)
{
    AE_STUB(); return NULL;
}

xl_texture_flip_t xl_texture_flip_from_short_name(const char* name)
{
    AE_STUB(); return XL_TEXTURE_FLIP_NONE;
}

void xl_texture_draw_ex(xl_texture_t* texture, float* src_rect,
                float* dst_rect, double angle, float* center)
{
    AE_STUB();
}

void xl_texture_draw(xl_texture_t* texture, const float xy[2])
{
    AE_STUB();
}

xl_texture_t* xl_texture_load_from_memory(xl_window_t* window, void* buf, size_t len)
{
    AE_STUB(); return NULL;
}

xl_texture_t* xl_texture_load_from_memory_ex(xl_window_t* window, void* buf,
                                size_t len, ae_image_error_t* error_status)
{
    AE_STUB(); return NULL;
}

xl_texture_t* xl_texture_load(xl_window_t* window, const char* filename)
{
    AE_STUB(); return NULL;
}

xl_texture_t* xl_texture_load_ex(xl_window_t* window, const char* filename,
                                            ae_image_error_t* error_status)
{
    AE_STUB(); return NULL;
}

void xl_texture_list_all(xl_texture_t** textures)
{
    AE_STUB();
}

void xl_texture_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ font renderer ] ~~ *
--------------------------------------------------------------------------------
*/

void
xl_font_set_int(xl_font_t* font, xl_font_property_t property, int value)
{
    AE_STUB();
}

int
xl_font_get_int(xl_font_t* font, xl_font_property_t property)
{
    AE_STUB(); return 0;
}

void
xl_font_set_flt(xl_font_t* font, xl_font_property_t property, float value)
{
    AE_STUB();
}

float
xl_font_get_flt(xl_font_t* font, xl_font_property_t property)
{
    AE_STUB(); return 0.0f;
}

void
xl_font_set_str(xl_font_t* font, xl_font_property_t property, const char* value)
{
    AE_STUB();
}

const char*
xl_font_get_str(xl_font_t* font, xl_font_property_t property)
{
    AE_STUB(); return "";
}

void
xl_font_set_ptr(xl_font_t* font, xl_font_property_t property, void* value)
{
    AE_STUB();
}

void*
xl_font_get_ptr(xl_font_t* font, xl_font_property_t property)
{
    AE_STUB(); return NULL;
}

void xl_font_text_size(xl_font_t* font, int * w, int * h, const char* format, ...)
{
    AE_STUB();
}

void xl_font_render_image(xl_font_t* font, ae_image_t* image, const char* format, ...)
{
    AE_STUB();
}

xl_texture_t* xl_font_render_texture(xl_font_t* font, const char* format, ...)
{
    AE_STUB(); return NULL;
}

void xl_font_blit(xl_font_t * font, ae_image_t * image, int x, int y,
                int r, int g, int b, int a, const char* format, ...)
{
    AE_STUB();
}

void xl_font_draw(xl_font_t* font, float xy[2], const char* format, ...)
{
    AE_STUB();
}

xl_font_t* xl_font_load_from_memory(xl_window_t* window, void* ptr,
                                    size_t length, int point_size)
{
    AE_STUB(); return NULL;
}

xl_font_t* xl_font_load(xl_window_t* window, const char* filename, int point_size)
{
    AE_STUB(); return NULL;
}

xl_font_t* xl_font_load_system_monospace(xl_window_t* window, int point_size)
{
    AE_STUB(); return NULL;
}

void xl_font_list_all(xl_font_t** fonts)
{
    AE_STUB();
}

void xl_font_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ streaming music ] ~~ *
--------------------------------------------------------------------------------
*/

void
xl_music_set_int(xl_music_property_t property, int value)
{
    AE_STUB();
}

int
xl_music_get_int(xl_music_property_t property)
{
    AE_STUB(); return 0;
}

void
xl_music_set_dbl(xl_music_property_t property, double value)
{
    AE_STUB();
}

double
xl_music_get_dbl(xl_music_property_t property)
{
    AE_STUB(); return 0.0;
}

void
xl_music_set_str(xl_music_property_t property, const char* value)
{
    AE_STUB();
}

const char*
xl_music_get_str(xl_music_property_t property)
{
    AE_STUB(); return "";
}

void xl_music_fade_in(const char* filename, int loop, double fade_in, double start_pos)
{
    AE_STUB();
}

void xl_music_fade_out(double fade_out)
{
    AE_STUB();
}

void xl_music_play(const char* filename)
{
    AE_STUB();
}

void xl_music_stop(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ sound effects ] ~~ *
--------------------------------------------------------------------------------
*/

void
xl_sound_set_int(xl_sound_t* sound, xl_sound_property_t property, int value)
{
    AE_STUB();
}

int
xl_sound_get_int(xl_sound_t* sound, xl_sound_property_t property)
{
    AE_STUB(); return 0;
}

void
xl_sound_set_dbl(xl_sound_t* sound, xl_sound_property_t property, double value)
{
    AE_STUB();
}

double
xl_sound_get_dbl(xl_sound_t* sound, xl_sound_property_t property)
{
    AE_STUB(); return 0.0;
}

void
xl_sound_set_str(xl_sound_t* sound, xl_sound_property_t property, const char* value)
{
    AE_STUB();
}

const char*
xl_sound_get_str(xl_sound_t* sound, xl_sound_property_t property)
{
    AE_STUB(); return "";
}

void xl_sound_fade_in(xl_sound_t* sound, int count, double fade_in, double length)
{
    AE_STUB();
}

void xl_sound_play(xl_sound_t* sound)
{
    AE_STUB();
}

void xl_sound_fade_out(xl_sound_t* sound, double fade_out)
{
    AE_STUB();
}

void xl_sound_stop(xl_sound_t* sound)
{
    AE_STUB();
}

xl_sound_t* xl_sound_load_from_memory(void* ptr, size_t length)
{
    AE_STUB(); return NULL;
}

xl_sound_t* xl_sound_load(const char* filename)
{
    AE_STUB(); return NULL;
}

void xl_sound_list_all(xl_sound_t** sounds)
{
    AE_STUB();
}

void xl_sound_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ keyboard input ] ~~ *
--------------------------------------------------------------------------------
*/

xl_keyboard_t* xl_primary_keyboard(void)
{
    AE_STUB(); return NULL;
}

void
xl_keyboard_set_int(xl_keyboard_t* keyboard, xl_keyboard_property_t prop, int value)
{
    AE_STUB();
}

int
xl_keyboard_get_int(xl_keyboard_t* keyboard, xl_keyboard_property_t prop)
{
    AE_STUB(); return 0;
}

void
xl_keyboard_set_dbl(xl_keyboard_t* keyboard, xl_keyboard_property_t prop, double value)
{
    AE_STUB();
}

double
xl_keyboard_get_dbl(xl_keyboard_t* keyboard, xl_keyboard_property_t prop)
{
    AE_STUB(); return 0.0;
}

void
xl_keyboard_set_str(xl_keyboard_t* keyboard, xl_keyboard_property_t prop, const char* value)
{
    AE_STUB();
}

const char*
xl_keyboard_get_str(xl_keyboard_t* keyboard, xl_keyboard_property_t prop)
{
    AE_STUB(); return "";
}

void
xl_keyboard_set_ptr(xl_keyboard_t* keyboard, xl_keyboard_property_t prop, void* value)
{
    AE_STUB();
}

void*
xl_keyboard_get_ptr(xl_keyboard_t* keyboard, xl_keyboard_property_t prop)
{
    AE_STUB(); return NULL;
}

void xl_keyboard_list_all(xl_keyboard_t** keyboards)
{
    AE_STUB();
}

xl_keyboard_mod_index_t xl_keyboard_mod_index_from_short_name(const char* name)
{
    AE_STUB(); return XL_KEYBOARD_MOD_INDEX_COUNT; // XXX: might cause crashes!
}

xl_keyboard_key_index_t xl_keyboard_key_index_from_short_name(const char* name)
{
    AE_STUB(); return XL_KEYBOARD_KEY_INDEX_UNKNOWN;
}

double xl_keyboard_get_last_key_pressed_time (xl_keyboard_t* keyboard,
                                        xl_keyboard_key_index_t key)
{
    AE_STUB(); return 0.0;
}

double xl_keyboard_get_last_key_released_time(xl_keyboard_t* keyboard,
                                        xl_keyboard_key_index_t key)
{
    AE_STUB(); return 0.0;
}

void xl_keyboard_clear_history(xl_keyboard_t* keyboard)
{
    AE_STUB();
}

int xl_keyboard_check_history(xl_keyboard_t* keyboard, // cheat check!!!
                const xl_keyboard_key_bit_t* const masks, size_t count)
{
    AE_STUB(); return 0;
}

/*
================================================================================
 * ~~ [ mouse input ] ~~ *
--------------------------------------------------------------------------------
*/

xl_mouse_t* xl_primary_mouse(void)
{
    AE_STUB(); return NULL;
}

void
xl_mouse_set_int(xl_mouse_t* mouse, xl_mouse_property_t prop, int value)
{
    AE_STUB();
}

int
xl_mouse_get_int(xl_mouse_t* mouse, xl_mouse_property_t prop)
{
    AE_STUB(); return 0;
}

void
xl_mouse_set_dbl(xl_mouse_t* mouse, xl_mouse_property_t prop, double value)
{
    AE_STUB();
}

double
xl_mouse_get_dbl(xl_mouse_t* mouse, xl_mouse_property_t prop)
{
    AE_STUB(); return 0.0;
}

void
xl_mouse_set_str(xl_mouse_t* mouse, xl_mouse_property_t prop, const char* value)
{
    AE_STUB();
}

const char*
xl_mouse_get_str(xl_mouse_t* mouse, xl_mouse_property_t prop)
{
    AE_STUB(); return "";
}

void
xl_mouse_set_ptr(xl_mouse_t* mouse, xl_mouse_property_t prop, void* value)
{
    AE_STUB();
}

void*
xl_mouse_get_ptr(xl_mouse_t* mouse, xl_mouse_property_t prop)
{
    AE_STUB(); return NULL;
}

void xl_mouse_list_all(xl_mouse_t** mice)
{
    AE_STUB();
}

xl_mouse_button_index_t xl_mouse_button_index_from_short_name(const char* name)
{
    AE_STUB(); return XL_MOUSE_BUTTON_INDEX_LEFT;
}

double xl_mouse_get_last_button_pressed_time(xl_mouse_t * mouse,
                                xl_mouse_button_index_t button)
{
    AE_STUB(); return 0.0;
}

double xl_mouse_get_last_button_released_time(xl_mouse_t* mouse,
                                xl_mouse_button_index_t button)
{
    AE_STUB(); return 0.0;
}

void xl_mouse_clear_history(xl_mouse_t* mouse)
{
    AE_STUB();
}

int
xl_mouse_check_history(xl_mouse_t* mouse, const int* const masks, size_t count)
{
    AE_STUB(); return 0;
}

/*
================================================================================
 * ~~ [ controller input ] ~~ *
--------------------------------------------------------------------------------
*/

xl_controller_t* xl_primary_controller(void)
{
    AE_STUB(); return NULL;
}

void
xl_controller_set_int(xl_controller_t* controller, xl_controller_property_t prop, int value)
{
    AE_STUB();
}

int
xl_controller_get_int(xl_controller_t* controller, xl_controller_property_t prop)
{
    AE_STUB(); return 0;
}

void
xl_controller_set_flt(xl_controller_t* controller, xl_controller_property_t prop, float value)
{
    AE_STUB();
}

float
xl_controller_get_flt(xl_controller_t* controller, xl_controller_property_t prop)
{
    AE_STUB(); return 0.0f;
}

void
xl_controller_set_dbl(xl_controller_t* controller, xl_controller_property_t prop, double value)
{
    AE_STUB();
}

double
xl_controller_get_dbl(xl_controller_t* controller, xl_controller_property_t prop)
{
    AE_STUB(); return 0.0;
}

void
xl_controller_set_str(xl_controller_t* controller, xl_controller_property_t prop, const char* value)
{
    AE_STUB();
}

const char*
xl_controller_get_str(xl_controller_t* controller, xl_controller_property_t prop)
{
    AE_STUB(); return "";
}

void xl_controller_list_all(xl_controller_t** controllers)
{
    AE_STUB();
}

xl_controller_button_index_t
xl_controller_button_index_from_short_name(const char* name)
{
    AE_STUB(); return XL_CONTROLLER_BUTTON_INDEX_START;
}

double xl_controller_get_last_button_pressed_time(xl_controller_t* controller,
                                        xl_controller_button_index_t button)
{
    AE_STUB(); return 0.0;
}

double xl_controller_get_last_button_released_time(xl_controller_t* controller,
                                        xl_controller_button_index_t button)
{
    AE_STUB(); return 0.0;
}

void xl_controller_clear_history(xl_controller_t* controller)
{
    AE_STUB();
}

int xl_controller_check_history(xl_controller_t * controller,
                        const int* const masks, size_t count)
{
    AE_STUB(); return 0;
}

double xl_controller_get_trigger(xl_controller_t* c, char which)
{
    AE_STUB(); return 0.0;
}

xl_controller_deadzone_mode_t
xl_controller_deadzone_mode_from_short_name(const char* s_name)
{
    AE_STUB(); return XL_CONTROLLER_DEADZONE_MODE_NONE;
}

void xl_controller_get_deadzone(xl_controller_t * controller, char which,
                    xl_controller_deadzone_mode_t * mode, double * value)
{
    AE_STUB();
}

void xl_controller_set_deadzone(xl_controller_t * controller, char which,
                        xl_controller_deadzone_mode_t mode, double value)
{
    AE_STUB();
}

double xl_controller_get_stick_angle(xl_controller_t* controller, char which)
{
    AE_STUB(); return 0.0;
}

double xl_controller_get_stick_magnitude(xl_controller_t* controller, char which)
{
    AE_STUB(); return 0.0;
}

void xl_controller_get_stick(xl_controller_t* c, char which, double* x, double* y)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ atlas animation ] ~~ *
--------------------------------------------------------------------------------
*/

xl_animation_t* xl_animation_create(void)
{
    AE_STUB(); return NULL;
}

xl_animation_t* xl_animation_copy(xl_animation_t* animation)
{
    AE_STUB(); return NULL;
}

void
xl_animation_set_int(xl_animation_t* animation, xl_animation_property_t property, int value)
{
    AE_STUB();
}

int
xl_animation_get_int(xl_animation_t* animation, xl_animation_property_t property)
{
    AE_STUB(); return 0;
}

void
xl_animation_set_dbl(xl_animation_t* animation, xl_animation_property_t property, double value)
{
    AE_STUB();
}

double
xl_animation_get_dbl(xl_animation_t* animation, xl_animation_property_t property)
{
    AE_STUB(); return 0.0;
}

void
xl_animation_set_str(xl_animation_t* animation, xl_animation_property_t property, const char* value)
{
    AE_STUB();
}

const char*
xl_animation_get_str(xl_animation_t* animation, xl_animation_property_t property)
{
    AE_STUB(); return "";
}

void
xl_animation_set_tex(xl_animation_t* animation, xl_animation_property_t property, xl_texture_t* value)
{
    AE_STUB();
}

xl_texture_t*
xl_animation_get_tex(xl_animation_t* animation, xl_animation_property_t property)
{
    AE_STUB(); return NULL;
}

void xl_animation_reset(xl_animation_t* animation)
{
    AE_STUB();
}

void xl_animation_reset_all(void)
{
    AE_STUB();
}

void xl_animation_update(xl_animation_t* animation, double dt)
{
    AE_STUB();
}

void xl_animation_update_all(double dt)
{
    AE_STUB();
}

void xl_animation_src_rect(xl_animation_t* animation, float* rect)
{
    AE_STUB();
}

void xl_animation_dst_rect(xl_animation_t* animation, float* rect,
                        const float pos[2], const float scale[2])
{
    AE_STUB();
}

void xl_animation_draw_ex(xl_animation_t* animation, float* dst_rect,
                                        double angle, float* center)
{
    AE_STUB();
}

void xl_animation_draw(xl_animation_t* animation, const float xy[2])
{
    AE_STUB();
}

xl_animation_t* xl_animation_load(xl_window_t* window, const char* filename,
                                        int frame_width, int frame_height)
{
    AE_STUB(); return NULL;
}

xl_animation_t* xl_animation_load_ex(xl_window_t* window, const char* filename,
                    int frame_width, int frame_height, ae_image_error_t* error)
{
    AE_STUB(); return NULL;
}

void xl_animation_list_all(xl_animation_t** animations)
{
    AE_STUB();
}

void xl_animation_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ timer objects ] ~~ *
--------------------------------------------------------------------------------
*/

xl_clock_t* xl_clock_create(void)
{
    AE_STUB(); return NULL;
}

xl_clock_t* xl_clock_copy(xl_clock_t* clock)
{
    AE_STUB(); return NULL;
}

size_t xl_clock_buffer_size(xl_clock_t* clock) // serialize
{
    AE_STUB(); return 0;
}

void xl_clock_buffer_save(u8* out, xl_clock_t* clock)
{
    AE_STUB();
}

xl_clock_t* xl_clock_buffer_load(u8* buffer, size_t length)
{
    AE_STUB(); return NULL;
}

void
xl_clock_set_int(xl_clock_t* clock, xl_clock_property_t prop, int value)
{
    AE_STUB();
}

int
xl_clock_get_int(xl_clock_t* clock, xl_clock_property_t prop)
{
    AE_STUB(); return 0;
}

void
xl_clock_set_dbl(xl_clock_t* clock, xl_clock_property_t prop, double value)
{
    AE_STUB();
}

double
xl_clock_get_dbl(xl_clock_t* clock, xl_clock_property_t prop)
{
    AE_STUB(); return 0.0;
}

void
xl_clock_set_str(xl_clock_t* clock, xl_clock_property_t prop, const char* value)
{
    AE_STUB();
}

const char*
xl_clock_get_str(xl_clock_t* clock, xl_clock_property_t prop)
{
    AE_STUB(); return "";
}

void xl_clock_update(xl_clock_t* clock, double dt)
{
    AE_STUB();
}

void xl_clock_update_all(double dt)
{
    AE_STUB();
}

void xl_clock_add_timer(xl_clock_t* clock, const char* name, double seconds, int repeat)
{
    AE_STUB();
}

void xl_clock_remove_timer(xl_clock_t* clock, const char* name)
{
    AE_STUB();
}

void xl_clock_remove_all_timers(xl_clock_t* clock)
{
    AE_STUB();
}

int xl_clock_get_timer(xl_clock_t* clock, const char* name, double* current,
                                double* seconds, int* paused, int* repeats)
{
    AE_STUB(); return 0;
}

void xl_clock_set_timer_current(xl_clock_t* clock, const char* name, double value)
{
    AE_STUB();
}

void xl_clock_set_timer_seconds(xl_clock_t* clock, const char* name, double value)
{
    AE_STUB();
}

void xl_clock_set_timer_paused(xl_clock_t* clock, const char* name, int value)
{
    AE_STUB();
}

void xl_clock_set_timer_repeat(xl_clock_t* clock, const char* name, int value)
{
    AE_STUB();
}

void xl_clock_set_timer_name(xl_clock_t* clock, const char* old_name,
                                                const char* new_name)
{
    AE_STUB();
}

char** xl_clock_copy_timer_names(xl_clock_t* clock)
{
    AE_STUB(); return NULL;
}

void xl_clock_free_timer_names(xl_clock_t* clock, char** names)
{
    AE_STUB();
}

void xl_clock_list_all(xl_clock_t** clocks)
{
    AE_STUB();
}

void xl_clock_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ timed events ] ~~ *
--------------------------------------------------------------------------------
*/

void
xl_timer_register(const char* name, double seconds, int repeat)
{
    AE_STUB();
}

void
xl_timer_unregister(const char* name)
{
    AE_STUB();
}

int
xl_timer_get(const char* name, double* current, double* seconds, int* repeat)
{
    AE_STUB(); return 0;
}

void
xl_timer_set_repeat(const char* name, int repeat)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ event handling ] ~~ *
--------------------------------------------------------------------------------
*/

void xl_event_get_handler(xl_event_handler_t* handler, void** context)
{
    AE_STUB();
}

void xl_event_set_handler(xl_event_handler_t handler, void* context)
{
    AE_STUB();
}

size_t xl_event_count_pending(void)
{
    AE_STUB(); return 0;
}

int xl_event_poll(xl_event_t* event, int wait)
{
    AE_STUB(); return 0;
}
