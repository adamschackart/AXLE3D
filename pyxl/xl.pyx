# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from aegame.utl cimport *
from aegame.vec cimport *
from aegame.lin cimport *
from aegame.mem cimport *
from aegame.img cimport *
from aegame.exm cimport *

import sys # version info

cdef extern from "xl_core.h":
    # ==========================================================================
    # ~ [ macros & init ]
    # ==========================================================================

    int xl_is_init()

    void xl_init()
    void xl_quit()

    int xl_audio_is_init()

    void xl_audio_init()
    void xl_audio_quit()

    # make sure you're at the right house before breaking in through the backdoor
    const char* xl_implementation()
    const char* xl_audio_implementation()

    # ==========================================================================
    # ~ [ object types ]
    # ==========================================================================

    ctypedef void xl_window_t
    ctypedef void xl_controller_t
    ctypedef void xl_texture_t
    ctypedef void xl_font_t
    ctypedef void xl_sound_t
    ctypedef void xl_animation_t

    ctypedef enum xl_object_type_t:
        XL_OBJECT_TYPE_UNKNOWN
        XL_OBJECT_TYPE_WINDOW
        XL_OBJECT_TYPE_CONTROLLER
        XL_OBJECT_TYPE_TEXTURE
        XL_OBJECT_TYPE_FONT
        XL_OBJECT_TYPE_SOUND
        XL_OBJECT_TYPE_ANIMATION
        XL_OBJECT_TYPE_COUNT

    xl_object_type_t xl_object_type(void *)

    const char* xl_object_type_name[]
    const char* xl_object_type_short_name[]

    size_t xl_object_count_all()
    void xl_object_list_all(void** objects)
    void xl_object_close_all()

    # ==========================================================================
    # ~ [ window management ]
    # ==========================================================================

    xl_window_t* xl_window_create(int initially_visible)

    ctypedef enum xl_window_property_t:
        XL_WINDOW_PROPERTY_TOTAL
        XL_WINDOW_PROPERTY_ID
        XL_WINDOW_PROPERTY_TEXTURE_COUNT
        XL_WINDOW_PROPERTY_COPY_TEXTURES
        XL_WINDOW_PROPERTY_FONT_COUNT
        XL_WINDOW_PROPERTY_X
        XL_WINDOW_PROPERTY_Y
        XL_WINDOW_PROPERTY_WIDTH
        XL_WINDOW_PROPERTY_HEIGHT
        XL_WINDOW_PROPERTY_DISPLAY_X
        XL_WINDOW_PROPERTY_DISPLAY_Y
        XL_WINDOW_PROPERTY_DISPLAY_WIDTH
        XL_WINDOW_PROPERTY_DISPLAY_HEIGHT
        XL_WINDOW_PROPERTY_RENDER_WIDTH
        XL_WINDOW_PROPERTY_RENDER_HEIGHT
        XL_WINDOW_PROPERTY_FULLSCREEN
        XL_WINDOW_PROPERTY_BORDERED
        XL_WINDOW_PROPERTY_VISIBLE
        XL_WINDOW_PROPERTY_RESIZABLE
        XL_WINDOW_PROPERTY_ACTIVE
        XL_WINDOW_PROPERTY_GRABBED
        XL_WINDOW_PROPERTY_OPENGL
        XL_WINDOW_PROPERTY_VSYNC
        XL_WINDOW_PROPERTY_OPEN
        XL_WINDOW_PROPERTY_STATUS
        XL_WINDOW_PROPERTY_TITLE
        XL_WINDOW_PROPERTY_NAME
        XL_WINDOW_PROPERTY_ICON
        XL_WINDOW_PROPERTY_OPACITY
        XL_WINDOW_PROPERTY_COUNT

    const char* xl_window_property_name[]
    const char* xl_window_property_type[]

    void xl_window_set_int(xl_window_t* win, xl_window_property_t prop, int val)
    int xl_window_get_int(xl_window_t* win, xl_window_property_t prop)

    void xl_window_set_flt(xl_window_t* win, xl_window_property_t prop, float val)
    float xl_window_get_flt(xl_window_t* win, xl_window_property_t prop)

    void xl_window_set_str(xl_window_t* win, xl_window_property_t prop, const char* val)
    const char* xl_window_get_str(xl_window_t* win, xl_window_property_t prop)

    void xl_window_set_ptr(xl_window_t* win, xl_window_property_t prop, void* val)
    void* xl_window_get_ptr(xl_window_t* win, xl_window_property_t prop)

    void xl_window_set_img(xl_window_t* win, xl_window_property_t prop, ae_image_t* val)
    ae_image_t* xl_window_get_img(xl_window_t* win, xl_window_property_t prop)

    void xl_window_close(xl_window_t* window)

    void xl_window_clear(xl_window_t* window, float r, float g, float b)
    void xl_window_flip(xl_window_t* window)
    void xl_window_screenshot(xl_window_t* window, ae_image_t* image)

    size_t xl_window_count_all()

    void xl_window_list_all(xl_window_t** windows)
    void xl_window_close_all()

    size_t xl_window_count_textures(xl_window_t* window)

    void xl_window_list_textures(xl_window_t* window, xl_texture_t** textures)
    void xl_window_close_textures(xl_window_t* window)

    size_t xl_window_count_fonts(xl_window_t* window)

    void xl_window_list_fonts(xl_window_t* window, xl_font_t** fonts)
    void xl_window_close_fonts(xl_window_t* window)

    # ==========================================================================
    # ~ [ shape renderer ]
    # ==========================================================================

    void xl_draw_rect_ex(xl_window_t* window, float* rect, float* color,
                        double angle, float* center, const int outline)

    void xl_draw_rect(xl_window_t* window, float* rect, float* color)

    void xl_draw_points(xl_window_t* window, float* points, size_t count, float* color)
    void xl_draw_point(xl_window_t* window, float* point, float* color)

    void xl_draw_line(xl_window_t* window, float* a, float* b, float* color)

    void xl_draw_curve(xl_window_t* window, float* a, float* b, float* color,
                            ae_ease_mode_t mode, const size_t num_divisions)

    void xl_draw_circle(xl_window_t * window, float * center, float radius,
                    float * color, int outline, const size_t num_divisions)

    void xl_draw_triangle(xl_window_t * window, float* a, float* b, float* c,
                                                float* color, int outline)

    # ==========================================================================
    # ~ [ texture renderer ]
    # ==========================================================================

    xl_texture_t* xl_texture_create(xl_window_t* window, int width, int height)

    ctypedef enum xl_texture_property_t:
        XL_TEXTURE_PROPERTY_TOTAL
        XL_TEXTURE_PROPERTY_ID
        XL_TEXTURE_PROPERTY_DRAW_CALLS
        XL_TEXTURE_PROPERTY_WINDOW
        XL_TEXTURE_PROPERTY_WIDTH
        XL_TEXTURE_PROPERTY_HEIGHT
        XL_TEXTURE_PROPERTY_IMAGE
        XL_TEXTURE_PROPERTY_COPY_ENABLED
        XL_TEXTURE_PROPERTY_STATUS
        XL_TEXTURE_PROPERTY_PATH
        XL_TEXTURE_PROPERTY_NAME
        XL_TEXTURE_PROPERTY_RED
        XL_TEXTURE_PROPERTY_GREEN
        XL_TEXTURE_PROPERTY_BLUE
        XL_TEXTURE_PROPERTY_ALPHA
        XL_TEXTURE_PROPERTY_RGB
        XL_TEXTURE_PROPERTY_RGBA
        XL_TEXTURE_PROPERTY_FLIP
        XL_TEXTURE_PROPERTY_OPEN
        XL_TEXTURE_PROPERTY_COUNT

    const char* xl_texture_property_name[]
    const char* xl_texture_property_type[]

    void xl_texture_set_int(xl_texture_t*, xl_texture_property_t, int)
    int xl_texture_get_int(xl_texture_t*, xl_texture_property_t)

    void xl_texture_set_flt(xl_texture_t*, xl_texture_property_t, float)
    float xl_texture_get_flt(xl_texture_t*, xl_texture_property_t)

    void xl_texture_set_str(xl_texture_t*, xl_texture_property_t, const char*)
    const char* xl_texture_get_str(xl_texture_t*, xl_texture_property_t)

    void xl_texture_set_ptr(xl_texture_t*, xl_texture_property_t, void*)
    void* xl_texture_get_ptr(xl_texture_t*, xl_texture_property_t)

    void xl_texture_set_img(xl_texture_t*, xl_texture_property_t, ae_image_t*)
    ae_image_t* xl_texture_get_img(xl_texture_t*, xl_texture_property_t)

    void xl_texture_close(xl_texture_t* texture)

    ctypedef enum xl_texture_flip_t:
        XL_TEXTURE_FLIP_NONE
        XL_TEXTURE_FLIP_HORIZONTAL
        XL_TEXTURE_FLIP_VERTICAL
        XL_TEXTURE_FLIP_BOTH
        XL_TEXTURE_FLIP_COUNT

    const char* xl_texture_flip_name[]
    const char* xl_texture_flip_short_name[]

    xl_texture_flip_t xl_texture_flip_from_short_name(const char* name)

    void xl_texture_draw_ex(xl_texture_t* texture, float* src_rect,
                    float* dst_rect, double angle, float* center)

    void xl_texture_draw(xl_texture_t* texture, const float xy[2])

    xl_texture_t* xl_texture_load_from_memory(xl_window_t* window, void* buf,
                                                                size_t size)

    xl_texture_t* xl_texture_load_from_memory_ex(xl_window_t* win, void* buf,
                                size_t size, ae_image_error_t* error_status)

    xl_texture_t* xl_texture_load_ex(xl_window_t* window, const char* filename,
                                                ae_image_error_t* error_status)

    xl_texture_t* xl_texture_load(xl_window_t* window, const char* filename)

    size_t xl_texture_count_all()

    void xl_texture_list_all(xl_texture_t** textures)
    void xl_texture_close_all()

    # ==========================================================================
    # ~ [ font renderer ]
    # ==========================================================================

    ctypedef enum xl_font_property_t:
        XL_FONT_PROPERTY_TOTAL
        XL_FONT_PROPERTY_ID
        XL_FONT_PROPERTY_WINDOW
        XL_FONT_PROPERTY_POINT_SIZE
        XL_FONT_PROPERTY_LINE_SKIP
        XL_FONT_PROPERTY_STATUS
        XL_FONT_PROPERTY_PATH
        XL_FONT_PROPERTY_NAME
        XL_FONT_PROPERTY_RED
        XL_FONT_PROPERTY_GREEN
        XL_FONT_PROPERTY_BLUE
        XL_FONT_PROPERTY_ALPHA
        XL_FONT_PROPERTY_RGB
        XL_FONT_PROPERTY_RGBA
        XL_FONT_PROPERTY_OPEN
        XL_FONT_PROPERTY_COUNT

    const char* xl_font_property_name[]
    const char* xl_font_property_type[]

    void xl_font_set_int(xl_font_t* font, xl_font_property_t prop, int)
    int xl_font_get_int(xl_font_t* font, xl_font_property_t prop)

    void xl_font_set_flt(xl_font_t* font, xl_font_property_t prop, float)
    float xl_font_get_flt(xl_font_t* font, xl_font_property_t prop)

    void xl_font_set_str(xl_font_t* font, xl_font_property_t prop, const char*)
    const char* xl_font_get_str(xl_font_t* font, xl_font_property_t prop)

    void xl_font_set_ptr(xl_font_t* font, xl_font_property_t prop, void*)
    void* xl_font_get_ptr(xl_font_t* font, xl_font_property_t prop)

    void xl_font_close(xl_font_t* font)

    void xl_font_text_size(xl_font_t* font, int* w, int* h, const char* fmt, ...)
    void xl_font_render_image(xl_font_t* font, ae_image_t* img, const char* fmt, ...)
    xl_texture_t* xl_font_render_texture(xl_font_t* font, const char* fmt, ...)

    void xl_font_blit(xl_font_t * font, ae_image_t * image, int x, int y,
                        int r, int g, int b, int a, const char* fmt, ...)

    void xl_font_draw(xl_font_t* font, float xy[2], const char* fmt, ...)

    xl_font_t* xl_font_load_from_memory(xl_window_t* window, void* ptr,
                                        size_t length, int point_size)

    xl_font_t* xl_font_load(xl_window_t* window, const char* filename, int point_size)
    xl_font_t* xl_font_load_system_monospace(xl_window_t* window, int point_size)

    size_t xl_font_count_all()

    void xl_font_list_all(xl_font_t** fonts)
    void xl_font_close_all()

    # ==========================================================================
    # ~ [ streaming music ]
    # ==========================================================================

    ctypedef enum xl_music_property_t:
        XL_MUSIC_PROPERTY_PLAYING
        XL_MUSIC_PROPERTY_PAUSED
        XL_MUSIC_PROPERTY_FADING_IN
        XL_MUSIC_PROPERTY_FADING_OUT
        XL_MUSIC_PROPERTY_STATUS
        XL_MUSIC_PROPERTY_DURATION
        XL_MUSIC_PROPERTY_POSITION
        XL_MUSIC_PROPERTY_VOLUME
        XL_MUSIC_PROPERTY_PATH
        XL_MUSIC_PROPERTY_NAME
        XL_MUSIC_PROPERTY_COUNT

    const char* xl_music_property_name[]
    const char* xl_music_property_type[]

    void xl_music_set_int(xl_music_property_t property, int value)
    int xl_music_get_int(xl_music_property_t property)

    void xl_music_set_dbl(xl_music_property_t property, double value)
    double xl_music_get_dbl(xl_music_property_t property)

    void xl_music_set_str(xl_music_property_t property, const char* value)
    const char* xl_music_get_str(xl_music_property_t property)

    void xl_music_fade_in(const char* name, int loop, double fade, double start)
    void xl_music_fade_out(double fade_out)

    void xl_music_play(const char* filename)
    void xl_music_stop()

    # ==========================================================================
    # ~ [ sound effects ]
    # ==========================================================================

    ctypedef enum xl_sound_property_t:
        XL_SOUND_PROPERTY_TOTAL
        XL_SOUND_PROPERTY_ID
        XL_SOUND_PROPERTY_DURATION
        XL_SOUND_PROPERTY_VOLUME
        XL_SOUND_PROPERTY_STATUS
        XL_SOUND_PROPERTY_PATH
        XL_SOUND_PROPERTY_NAME
        XL_SOUND_PROPERTY_OPEN
        XL_SOUND_PROPERTY_COUNT

    const char* xl_sound_property_name[]
    const char* xl_sound_property_type[]

    void xl_sound_set_int(xl_sound_t* sound, xl_sound_property_t prop, int val)
    int xl_sound_get_int(xl_sound_t* sound, xl_sound_property_t prop)

    void xl_sound_set_dbl(xl_sound_t* sound, xl_sound_property_t prop, double val)
    double xl_sound_get_dbl(xl_sound_t* sound, xl_sound_property_t prop)

    void xl_sound_set_str(xl_sound_t* sound, xl_sound_property_t prop, const char* val)
    const char* xl_sound_get_str(xl_sound_t* sound, xl_sound_property_t prop)

    void xl_sound_close(xl_sound_t* sound)

    void xl_sound_fade_in(xl_sound_t* sound, int count, double fade_in, double length)
    void xl_sound_play(xl_sound_t* sound)

    void xl_sound_fade_out(xl_sound_t* sound, double fade_out)
    void xl_sound_stop(xl_sound_t* sound)

    xl_sound_t* xl_sound_load_from_memory(void* ptr, size_t length)
    xl_sound_t* xl_sound_load(const char* filename)

    size_t xl_sound_count_all()

    void xl_sound_list_all(xl_sound_t** sounds)
    void xl_sound_close_all()

    # ==========================================================================
    # ~ [ controller input ]
    # ==========================================================================

    ctypedef enum xl_controller_property_t:
        XL_CONTROLLER_PROPERTY_TOTAL
        XL_CONTROLLER_PROPERTY_ID
        XL_CONTROLLER_PROPERTY_DOWN_BUTTONS
        XL_CONTROLLER_PROPERTY_UP_BUTTONS
        XL_CONTROLLER_PROPERTY_SHOULDER_TRIBOOL
        XL_CONTROLLER_PROPERTY_DPAD_HORIZONTAL_TRIBOOL
        XL_CONTROLLER_PROPERTY_DPAD_VERTICAL_TRIBOOL
        XL_CONTROLLER_PROPERTY_STICK_TRIBOOL
        XL_CONTROLLER_PROPERTY_LAST_PRESSED_BUTTON
        XL_CONTROLLER_PROPERTY_LAST_RELEASED_BUTTON
        XL_CONTROLLER_PROPERTY_LAST_PRESSED_TIME
        XL_CONTROLLER_PROPERTY_LAST_RELEASED_TIME
        XL_CONTROLLER_PROPERTY_RIGHT_TRIGGER
        XL_CONTROLLER_PROPERTY_LEFT_TRIGGER
        XL_CONTROLLER_PROPERTY_RIGHT_DEADZONE_MODE
        XL_CONTROLLER_PROPERTY_RIGHT_DEADZONE_VALUE
        XL_CONTROLLER_PROPERTY_LEFT_DEADZONE_MODE
        XL_CONTROLLER_PROPERTY_LEFT_DEADZONE_VALUE
        XL_CONTROLLER_PROPERTY_RIGHT_STICK_ANGLE
        XL_CONTROLLER_PROPERTY_RIGHT_STICK_MAGNITUDE
        XL_CONTROLLER_PROPERTY_LEFT_STICK_ANGLE
        XL_CONTROLLER_PROPERTY_LEFT_STICK_MAGNITUDE
        XL_CONTROLLER_PROPERTY_RIGHT_STICK_X
        XL_CONTROLLER_PROPERTY_RIGHT_STICK_Y
        XL_CONTROLLER_PROPERTY_LEFT_STICK_X
        XL_CONTROLLER_PROPERTY_LEFT_STICK_Y
        XL_CONTROLLER_PROPERTY_OPEN
        XL_CONTROLLER_PROPERTY_STATUS
        XL_CONTROLLER_PROPERTY_NAME
        XL_CONTROLLER_PROPERTY_COUNT

    const char* xl_controller_property_name[]
    const char* xl_controller_property_type[]

    void xl_controller_set_int(xl_controller_t*, xl_controller_property_t, int)
    int xl_controller_get_int(xl_controller_t*, xl_controller_property_t)

    void xl_controller_set_flt(xl_controller_t*, xl_controller_property_t, float)
    float xl_controller_get_flt(xl_controller_t*, xl_controller_property_t)

    void xl_controller_set_dbl(xl_controller_t*, xl_controller_property_t, double)
    double xl_controller_get_dbl(xl_controller_t*, xl_controller_property_t)

    void xl_controller_set_str(xl_controller_t*, xl_controller_property_t, const char*)
    const char* xl_controller_get_str(xl_controller_t*, xl_controller_property_t)

    size_t xl_controller_count_all()
    void xl_controller_list_all(xl_controller_t** controllers)

    # ===== [ digital buttons ] ================================================

    ctypedef enum xl_controller_button_index_t:
        XL_CONTROLLER_BUTTON_INDEX_A
        XL_CONTROLLER_BUTTON_INDEX_B
        XL_CONTROLLER_BUTTON_INDEX_X
        XL_CONTROLLER_BUTTON_INDEX_Y
        XL_CONTROLLER_BUTTON_INDEX_SELECT
        XL_CONTROLLER_BUTTON_INDEX_START
        XL_CONTROLLER_BUTTON_INDEX_LEFT_STICK
        XL_CONTROLLER_BUTTON_INDEX_RIGHT_STICK
        XL_CONTROLLER_BUTTON_INDEX_LEFT_SHOULDER
        XL_CONTROLLER_BUTTON_INDEX_RIGHT_SHOULDER
        XL_CONTROLLER_BUTTON_INDEX_DPAD_UP
        XL_CONTROLLER_BUTTON_INDEX_DPAD_DOWN
        XL_CONTROLLER_BUTTON_INDEX_DPAD_LEFT
        XL_CONTROLLER_BUTTON_INDEX_DPAD_RIGHT
        XL_CONTROLLER_BUTTON_INDEX_COUNT

    const char* xl_controller_button_index_name[]
    const char* xl_controller_button_short_name[]

    ctypedef enum xl_controller_button_bit_t:
        pass

    xl_controller_button_index_t \
    xl_controller_button_index_from_short_name(const char* name)

    xl_controller_button_bit_t \
    xl_controller_button_bit_from_short_name(const char* name)

    int xl_controller_button_is_down(xl_controller_t*, xl_controller_button_index_t)
    int xl_controller_button_is_up(xl_controller_t*, xl_controller_button_index_t)

    double xl_controller_get_last_button_pressed_time( xl_controller_t * controller,
                                                xl_controller_button_index_t button)

    double xl_controller_get_last_button_released_time(xl_controller_t * controller,
                                                xl_controller_button_index_t button)

    void xl_controller_clear_history(xl_controller_t* controller)

    int xl_controller_check_history( xl_controller_t* controller,
                            const int* const masks, size_t count)

    # ===== [ analog axes & triggers ] =========================================

    double xl_controller_get_trigger(xl_controller_t* controller, char which)

    void xl_controller_get_deadzone(xl_controller_t * controller, char which,
                        xl_controller_deadzone_mode_t * mode, double * value)

    void xl_controller_set_deadzone(xl_controller_t * controller, char which,
                            xl_controller_deadzone_mode_t mode, double value)

    double xl_controller_get_stick_angle(xl_controller_t * c, char which)
    double xl_controller_get_stick_magnitude(xl_controller_t * c, char which)

    void xl_controller_get_stick(xl_controller_t* controller,
                            char which, double* x, double* y)

    ctypedef enum xl_controller_deadzone_mode_t:
        XL_CONTROLLER_DEADZONE_MODE_NONE
        XL_CONTROLLER_DEADZONE_MODE_AXIAL
        XL_CONTROLLER_DEADZONE_MODE_RADIAL
        XL_CONTROLLER_DEADZONE_MODE_SCALED_RADIAL
        XL_CONTROLLER_DEADZONE_MODE_X_BOWTIE
        XL_CONTROLLER_DEADZONE_MODE_Y_BOWTIE
        XL_CONTROLLER_DEADZONE_MODE_COUNT

    const char* xl_controller_deadzone_mode_name[]
    const char* xl_controller_deadzone_short_name[]

    xl_controller_deadzone_mode_t \
    xl_controller_deadzone_mode_from_short_name(const char* name)

    # ==========================================================================
    # ~ [ atlas animation ]
    # ==========================================================================

    xl_animation_t* xl_animation_create()
    xl_animation_t* xl_animation_copy(xl_animation_t* animation)

    ctypedef enum xl_animation_property_t:
        XL_ANIMATION_PROPERTY_TOTAL
        XL_ANIMATION_PROPERTY_ID
        XL_ANIMATION_PROPERTY_ATLAS
        XL_ANIMATION_PROPERTY_OWNS_ATLAS
        XL_ANIMATION_PROPERTY_FRAME_WIDTH
        XL_ANIMATION_PROPERTY_FRAME_HEIGHT
        XL_ANIMATION_PROPERTY_FIRST_FRAME
        XL_ANIMATION_PROPERTY_FRAME_COUNT
        XL_ANIMATION_PROPERTY_FRAME_TIME
        XL_ANIMATION_PROPERTY_TOTAL_TIME
        XL_ANIMATION_PROPERTY_CURRENT_FRAME
        XL_ANIMATION_PROPERTY_POSITION
        XL_ANIMATION_PROPERTY_LOOPS
        XL_ANIMATION_PROPERTY_FINISHED
        XL_ANIMATION_PROPERTY_STATUS
        XL_ANIMATION_PROPERTY_PATH
        XL_ANIMATION_PROPERTY_NAME
        XL_ANIMATION_PROPERTY_OPEN
        XL_ANIMATION_PROPERTY_COUNT

    const char* xl_animation_property_name[]
    const char* xl_animation_property_type[]

    void xl_animation_set_int(xl_animation_t*, xl_animation_property_t, int)
    int xl_animation_get_int(xl_animation_t*, xl_animation_property_t)

    void xl_animation_set_dbl(xl_animation_t*, xl_animation_property_t, double)
    double xl_animation_get_dbl(xl_animation_t*, xl_animation_property_t)

    void xl_animation_set_str(xl_animation_t*, xl_animation_property_t, const char*)
    const char* xl_animation_get_str(xl_animation_t*, xl_animation_property_t)

    void xl_animation_set_tex(xl_animation_t*, xl_animation_property_t, xl_texture_t*)
    xl_texture_t* xl_animation_get_tex(xl_animation_t*, xl_animation_property_t)

    void xl_animation_close(xl_animation_t* animation)

    void xl_animation_reset(xl_animation_t* animation)
    void xl_animation_reset_all()

    void xl_animation_update(xl_animation_t* animation, double dt)
    void xl_animation_update_all(double dt)

    void xl_animation_src_rect( xl_animation_t * animation, float * rect)
    void xl_animation_dst_rect( xl_animation_t * animation, float * rect,
                                const float pos[2], const float scale[2])

    void xl_animation_draw_ex(xl_animation_t* animation, float* dst_rect,
                                            double angle, float* center)

    void xl_animation_draw(xl_animation_t* animation, const float xy[2])

    xl_animation_t* xl_animation_load(xl_window_t* window, const char* filename,
                                            int frame_width, int frame_height)

    xl_animation_t* xl_animation_load_ex(xl_window_t* window, const char* filename,
                        int frame_width, int frame_height, ae_image_error_t* error)

    size_t xl_animation_count_all()

    void xl_animation_list_all(xl_animation_t** animations)
    void xl_animation_close_all()

    # ==========================================================================
    # ~ [ event handling ]
    # ==========================================================================

    ctypedef enum xl_event_type_t:
        XL_EVENT_NOTHING
        XL_EVENT_QUIT
        XL_EVENT_WINDOW_CLOSE
        XL_EVENT_WINDOW_MOVE
        XL_EVENT_WINDOW_RESIZE
        XL_EVENT_WINDOW_VISIBILITY_CHANGE
        XL_EVENT_WINDOW_REDRAW
        XL_EVENT_WINDOW_GAIN_FOCUS
        XL_EVENT_WINDOW_LOSE_FOCUS
        XL_EVENT_WINDOW_MOUSE_ENTER
        XL_EVENT_WINDOW_MOUSE_LEAVE
        XL_EVENT_MUSIC_FINISHED
        XL_EVENT_SOUND_FINISHED
        XL_EVENT_ANIMATION_FINISHED
        XL_EVENT_CONTROLLER_INSERT
        XL_EVENT_CONTROLLER_REMOVE
        XL_EVENT_CONTROLLER_BUTTON
        XL_EVENT_CONTROLLER_TRIGGER
        XL_EVENT_CONTROLLER_STICK
        XL_EVENT_COUNT

    const char* xl_event_type_name[]

    # NOTE: these aren't real types (see the comment below re: anon union hack)

    ctypedef struct _xl_window_event_t:
        xl_window_t* window

    ctypedef struct _xl_window_move_event_t:
        xl_window_t* window
        int x, y

    ctypedef struct _xl_window_resize_event_t:
        xl_window_t* window
        int width, height

    ctypedef struct _xl_window_visibility_change_event_t:
        xl_window_t* window
        int visible

    ctypedef struct _xl_sound_event_t:
        xl_sound_t* sound

    ctypedef struct _xl_animation_event_t:
        xl_animation_t* animation

    ctypedef struct _xl_controller_event_t:
        xl_controller_t* controller

    ctypedef struct _xl_controller_button_event_t:
        xl_controller_t* controller
        xl_controller_button_index_t button
        int pressed

    ctypedef struct _xl_controller_trigger_event_t:
        xl_controller_t* controller
        char which
        double value

    ctypedef struct _xl_controller_stick_event_t:
        xl_controller_t* controller
        char which
        double magnitude, angle, x, y

    ctypedef struct xl_event_t:
        xl_event_type_t type

        # these are all members of an anonymous union. cython doesn't know this,
        # and cython also doesn't support anonymous unions... hence this hack.

        _xl_window_event_t                      as_window_close
        _xl_window_move_event_t                 as_window_move
        _xl_window_resize_event_t               as_window_resize
        _xl_window_visibility_change_event_t    as_window_visibility_change
        _xl_window_event_t                      as_window_redraw
        _xl_window_event_t                      as_window_gain_focus
        _xl_window_event_t                      as_window_lose_focus
        _xl_window_event_t                      as_window_mouse_enter
        _xl_window_event_t                      as_window_mouse_leave
        _xl_sound_event_t                       as_sound_finished
        _xl_animation_event_t                   as_animation_finished
        _xl_controller_event_t                  as_controller_insert
        _xl_controller_event_t                  as_controller_remove
        _xl_controller_button_event_t           as_controller_button
        _xl_controller_trigger_event_t          as_controller_trigger
        _xl_controller_stick_event_t            as_controller_stick

    size_t xl_event_count_pending() # num unprocessed
    int xl_event_poll(xl_event_t * event, int wait)

def early_init(bint audio=True):
    """
    Take the initialization lag hit before opening windows and/or playing audio.
    Otherwise, XL init is performed on window open, and audio init on playback.
    """
    xl_init()
    if audio: xl_audio_init()

def implementation():
    """
    Identify the underlying platform layer, for 'safer' game-specific hacks.
    """
    cdef bytes video = xl_implementation() # implicitly cast pointers
    cdef bytes audio = xl_audio_implementation() # separate sfx layer

    if sys.version_info.major > 2:
        return (video.decode(), audio.decode()) # convert to unicode
    else:
        return (video, audio) # use oldschool python 2 ascii strings

# ==============================================================================
# ~ [ generic objects ]
# ==============================================================================

cdef class Object:
    """
    Optionally garbage-collected interface for any XL object. Not a base class.
    """
    cdef public bint collect
    cdef void * ptr

    def __init__(self, size_t reference=0, bint collect=False):
        self.collect = collect
        self.ptr = <void*>reference

    def __dealloc__(self):
        if self.collect and self.open: self.close()

    def __repr__(self):
        if self.open:
            return ('Collected' if self.collect else 'Wrapped') + repr(self.cast())
        else:
            return '{}(unknown)'.format(self.__class__.__name__)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, object other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return self.open

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    def __getattr__(self, key):
        if self.open:
            return getattr(self.cast(), key)
        else:
            raise AttributeError("'{}' object has no attribute '{}'" \
                                        .format(str(type(self)), key))

    def __setattr__(self, key, val):
        if self.open:
            setattr(self.cast(), key, val)
        else:
            raise AttributeError("'{}' object has no attribute '{}'" \
                                        .format(str(type(self)), key))

    @staticmethod
    def count_all(): return xl_object_count_all()

    @classmethod
    def list_all(cls, bint cast=True):
        """
        Gather references to all open, active, valid objects in a single list.
        """
        cdef void* xl_objects[8192]
        cdef int i
        cdef list objects = []

        if xl_object_count_all() > 8192:
            raise MemoryError("too many open xl objects for temp")

        xl_object_list_all(xl_objects)

        for i in range(<int>xl_object_count_all()):
            objects.append(cls(reference = <size_t>xl_objects[i]))

            if cast: objects[i] = objects[i].cast()

        return objects

    @staticmethod
    def close_all(): xl_object_close_all()

    property open:
        def __get__(self):
            return xl_object_type(self.ptr) != XL_OBJECT_TYPE_UNKNOWN

    def address(self):
        return <size_t>self.ptr

    def cast(self):
        """
        Get self converted to the type of object it wraps. This is slow!!!
        """
        cdef xl_object_type_t object_type = xl_object_type(self.ptr) # enum
        cdef bytes name = xl_object_type_short_name[< size_t >object_type]

        cdef str nstr # TODO: move all string code after unknown detection

        if object_type == XL_OBJECT_TYPE_UNKNOWN:
            return self

        # XXX: is there a way to detect version num without calling python?
        if sys.version_info.major > 2:
            nstr = name.decode()
        else:
            nstr = <str>name

        return globals()[nstr.replace('_', ' ').title().replace(' ', '')] \
                                            (reference = <size_t>self.ptr)

def ObjectFrom(object obj, bint collect=False):
    """
    Cheesy, easy grammar for the lazy typist. This would have been a class
    method, but 'from' is a keyword in Python for imports and generators.
    """
    return Object(obj.address(), collect)

def AutoObject(object obj):
    """
    Convenience ctor for locals, ex. `t = AutoObject(Texture().load(...))`.
    """
    return Object(obj.address(), True)

# ==============================================================================
# ~ [ window management ]
# ==============================================================================

cdef class Window:
    """
    A rendering context with an optional resolution-independent 2D renderer.
    Contains all textures and fonts that are valid to render in this window.
    """
    cdef xl_window_t* window

    def __init__(self, **kwargs):
        """
        Note that also pertains to the rest of this system: `reference` is a
        pointer value for this object, which can be retrieved with address().

        If a reference is not provided, a new window is constructed with the
        properties given by a set of keyword arguments (e.g. bordered=False).
        """
        if 'reference' in kwargs:
            self.window = <xl_window_t*>(<size_t>kwargs['reference'])
        else:
            # create default window, modify properties, and make it visible
            self.window = xl_window_create(0)

            for key, val in kwargs.items():
                setattr(self, key, val)

            # allow invisible windows to be created here for whatever reason
            if not kwargs.get('visible', True): return

            # TODO: flush the window visibility change event this generates
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_VISIBLE, 1)

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Window other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_OPEN)

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    @staticmethod
    def count_all(): return xl_window_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open windows in a single list.
        """
        cdef xl_window_t* windows[256]
        cdef int i
        cdef list objects = []

        if xl_window_count_all() > 256:
            raise MemoryError("too many open windows for temp")

        xl_window_list_all(windows)

        for i in range(<int>xl_window_count_all()):
            objects.append(cls(reference = <size_t>windows[i]))

        return objects

    @staticmethod
    def close_all(): xl_window_close_all()

    def count_textures(self):
        return xl_window_count_textures(self.window)

    def list_textures(self, object cls=Texture):
        """
        Get a list of all active textures bound to this GL context.
        """
        cdef xl_texture_t* textures[8192]
        cdef int i
        cdef list objects = []

        if xl_window_count_textures(self.window) > 8192:
            raise MemoryError("too many open textures for temp")

        xl_window_list_textures(self.window, textures)

        for i in range(<int>xl_window_count_textures(self.window)):
            objects.append(cls(reference = <size_t>textures[i]))

        return objects

    def close_textures(self):
        xl_window_close_textures(self.window); return self

    def count_fonts(self):
        return xl_window_count_fonts(self.window)

    def list_fonts(self, object cls=Font):
        """
        Get a list of all open fonts bound to this GL context.
        """
        cdef xl_font_t* fonts[1024]
        cdef int i
        cdef list objects = []

        if xl_window_count_fonts(self.window) > 1024:
            raise MemoryError("too many open fonts for temp")

        xl_window_list_fonts(self.window, fonts)

        for i in range(<int>xl_window_count_fonts(self.window)):
            objects.append(cls(reference = <size_t>fonts[i]))

        return objects

    def close_fonts(self):
        xl_window_close_fonts(self.window); return self

    property id:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_ID)

    property rect:
        def __get__(self):
            return IntRect(self.x, self.y, self.width, self.height)

        def __set__(self, object value):
            self.x, self.y, self.width, self.height = value

    property x:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_X)

        def __set__(self, int value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_X, value)

    property y:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_Y)

        def __set__(self, int value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_Y, value)

    property width:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_WIDTH)

        def __set__(self, int value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_WIDTH, value)

    property height:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_HEIGHT)

        def __set__(self, int value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_HEIGHT, value)

    property size:
        def __get__(self):
            return (self.width, self.height)

        def __set__(self, object value):
            self.width, self.height = value

    property pos:
        def __get__(self):
            return (self.x, self.y)

        def __set__(self, object value):
            self.x, self.y = value

    property display_rect:
        def __get__(self):
            cdef IntRect r = IntRect()

            r.rect[0] = xl_window_get_int(self.window, XL_WINDOW_PROPERTY_DISPLAY_X)
            r.rect[1] = xl_window_get_int(self.window, XL_WINDOW_PROPERTY_DISPLAY_Y)

            r.rect[2] = xl_window_get_int(self.window, XL_WINDOW_PROPERTY_DISPLAY_WIDTH )
            r.rect[3] = xl_window_get_int(self.window, XL_WINDOW_PROPERTY_DISPLAY_HEIGHT)

            return r

    property display_x:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_DISPLAY_X)

    property display_y:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_DISPLAY_Y)

    property display_width:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_DISPLAY_WIDTH)

    property display_height:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_DISPLAY_HEIGHT)

    property display_size:
        def __get__(self):
            return (self.display_width, self.display_height)

    property display_pos:
        def __get__(self):
            return (self.display_x, self.display_y)

    property render_width:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_RENDER_WIDTH)

        def __set__(self, int value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_RENDER_WIDTH, value)

    property render_height:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_RENDER_HEIGHT)

        def __set__(self, int value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_RENDER_HEIGHT, value)

    property render_size:
        def __get__(self):
            return (self.render_width, self.render_height)

        def __set__(self, object value):
            self.render_width, self.render_height = value

    property copy_textures:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_COPY_TEXTURES)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_COPY_TEXTURES, value)

    property opengl:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_OPENGL)

    property fullscreen:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_FULLSCREEN)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_FULLSCREEN, value)

    property bordered:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_BORDERED)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_BORDERED, value)

    property visible:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_VISIBLE)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_VISIBLE, value)

    property resizable:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_RESIZABLE)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_RESIZABLE, value)

    property active:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_ACTIVE)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_ACTIVE, value)

    property grabbed:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_GRABBED)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_GRABBED, value)

    property vsync:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_VSYNC)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_VSYNC, value)

    property open:
        def __get__(self):
            return xl_window_get_int(self.window, XL_WINDOW_PROPERTY_OPEN)

        def __set__(self, bint value):
            xl_window_set_int(self.window, XL_WINDOW_PROPERTY_OPEN, value)

    property status:
        def __get__(self):
            cdef bytes s = xl_window_get_str(self.window, XL_WINDOW_PROPERTY_STATUS)
            return s.decode() if sys.version_info.major > 2 else s

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_window_set_str(self.window, XL_WINDOW_PROPERTY_STATUS, <char*>string)

    property title:
        def __get__(self):
            cdef bytes s = xl_window_get_str(self.window, XL_WINDOW_PROPERTY_TITLE)
            return s.decode() if sys.version_info.major > 2 else s

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_window_set_str(self.window, XL_WINDOW_PROPERTY_TITLE, <char*>string)

    property name:
        def __get__(self):
            cdef bytes s = xl_window_get_str(self.window, XL_WINDOW_PROPERTY_NAME)
            return s.decode() if sys.version_info.major > 2 else s

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_window_set_str(self.window, XL_WINDOW_PROPERTY_NAME, <char*>string)

    property icon:
        def __get__(self):
            raise NotImplementedError("TODO")

        def __set__(self, Image value):
            raise NotImplementedError("TODO")

    property opacity:
        def __get__(self):
            return xl_window_get_flt(self.window, XL_WINDOW_PROPERTY_OPACITY)

        def __set__(self, float value):
            xl_window_set_flt(self.window, XL_WINDOW_PROPERTY_OPACITY, value)

    def address(self):
        return <size_t>self.window

    def cast(self):
        return self

    def close(self):
        xl_window_close(self.window); return self

    def clear(self, float r=0, float g=0, float b=0):
        xl_window_clear(self.window, r, g, b); return self

    def flip(self):
        xl_window_flip(self.window); return self

    def screenshot(self):
        """
        Copy the contents of the window to an image. This may take a while!
        """
        cdef Image image = Image()

        xl_window_screenshot(self.window, &image.image)
        return image

    def draw_rect(self, FloatRect rect=None, Vec4 color=None, double angle=0.0,
                                        Vec2 center=None, bint outline=False):
        """
        Draw a colored rectangle, with an optional rotation and outline mode.
        """
        xl_draw_rect_ex(self.window,
                        NULL if rect is None else rect.rect,
                        NULL if color is None else color.v,
                        angle,
                        NULL if center is None else center.v,
                        outline)
        return self

    def draw_points(self, Array points, Vec4 color=None):
        """
        Draw an array of 2D-dimensional points / particles as single pixels.
        """
        xl_draw_points( self.window,
                        <float*>points.array.data,
                        points.array.size / sizeof(float[2]),
                        NULL if color is None else color.v)
        return self

    def draw_point(self, Vec2 point, Vec4 color=None):
        """
        Draw a single pixel onscreen. Batched calls are obviously much faster.
        """
        xl_draw_point(self.window, point.v, NULL if color is None else color.v)
        return self

    def draw_line(self, Vec2 a, Vec2 b, Vec4 color=None):
        """
        Draw a single translucent 1-pixel line segment from point A to point B.
        """
        xl_draw_line(self.window, a.v, b.v, NULL if color is None else color.v)
        return self

    def draw_curve(self, Vec2 a, Vec2 b, Vec4 color=None, str mode='cubic_inout',
                                                        size_t num_divisions=24):
        """
        Draw simple bezier curve with a given easing mode (see aegame.exm modes).
        """
        cdef bytes b_mode

        if sys.version_info.major > 2:
            b_mode = <bytes>mode.encode('utf-8')
        else:
            b_mode = <bytes>mode

        xl_draw_curve(self.window, a.v, b.v, NULL if color is None else color.v,
                    ae_ease_mode_from_short_name(<char *>b_mode), num_divisions)

        return self

    def draw_circle(self, Circle circle, Vec4 color=None,
            bint outline=False, size_t num_divisions=24):
        """
        Draw a circle (num_division is the straight segment count in the circle).
        """
        xl_draw_circle(self.window, circle.c.v, circle.r, # handle None?
            NULL if color is None else color.v, outline, num_divisions)

        return self

    def draw_triangle(self, Vec2 a, Vec2 b, Vec2 c, Vec4 color=None, bint outline=False):
        """
        Draw a triangle, either filled or 1-pixel outlined.
        """
        xl_draw_triangle(self.window, a.v, b.v, c.v, NULL if color is None else color.v,
                                                                                outline)
        return self

    def run(self, object initial_state):
        """
        Execute an application using a stack-like state machine. Each application
        state (object of any type) can contain the following optional properties:

            - activate(window) method for when the state is brought to stack top
            - deactivate(window) method for when the state is popped or buried

            - queued_state is the next state, that we want to push onto the stack
            - has_exit indicates that we want to finish this state and remove it

            - update(dt) method for logic (entity physics, AI, simulation, etc.)
            - draw(window) for rendering (separate from update for pause menus)

        States can also have event handling methods (i.e. on_controller_button).
        See xl.event.poll() for a listing of the various event handler methods.
        """
        cdef list state_stack = []

        # Don't call activation code on the initial state if the window is invalid.
        if not self.open: return self

        # Reset the frame delta timer to prevent the first frame from being long.
        ae_frame_delta()

        def push_state(object state):
            """
            Push a queued state onto the application stack and activate it.
            """
            cdef bytes state_repr

            if sys.version_info.major > 2:
                state_repr = '{}'.format(state).encode('utf-8') # unicode
            else:
                state_repr = b'{}'.format(state) # oldschool ascii string

            if state_stack:
                if hasattr(state_stack[-1], 'deactivate'):
                    state_stack[-1].deactivate(self)

                state_stack[-1].queued_state = None

            ae_log_str('MISC', 'state push: %s', <const char*>state_repr)
            state_stack.append(state)

            if hasattr(state, 'activate'):
                state.activate(self)

        def pop_state():
            """
            Remove the top state from the application stack and deactivate it.
            """
            cdef bytes state_repr

            if sys.version_info.major > 2:
                state_repr = '{}'.format(state_stack[-1]).encode('utf-8')
            else:
                state_repr = b'{}'.format(state_stack[-1]) # utf or ascii

            if (hasattr(state_stack[-1], 'deactivate')):
                state_stack[-1].deactivate(self)

            ae_log_str('MISC', 'state pop: %s', <const char*>state_repr)
            state_stack.pop()

            if (state_stack and hasattr(state_stack[-1], 'activate')):
                state_stack[-1].activate(self)

        def emit_frame(double dt):
            """
            Run a single iteration of the game loop. Note that when the final frame
            has finished, the window framebuffer contents are not cleared to avoid
            a black flash if the user decides to do something else with the window.
            """
            # Filter out insane spikes (e.g. due to another process, or loading).
            if (dt > 0.1):
                ae_log_str('TIME', 'long frame time delta: %f seconds', dt)
                dt = 0.01

            while state_stack and getattr(state_stack[-1], 'has_exit', False):
                pop_state()

            if state_stack:
                # TODO: Flush the state stack if we encounter an unhandled on_quit
                # event (dispatch_all returns the events dispatched to a handler).
                # TODO: Paused/tool states (wait for events and don't update/draw).

                event.dispatch_all(state_stack[-1])

                # TODO: Allow users to pass a custom window clear color to run().
                self.clear()

                # NOTE: The decision to pass a window to draw() was controversial,
                # if a real controversy can exist within the mind of one person.
                # We (meaning I) should also consider the possibility of changing
                # these method names to on_update, on_draw etc. for consistency.
                # Take this to the ISO Committee on Issues Related to Yak Shaving.

                if hasattr(state_stack[-1], 'update'):
                    state_stack[-1].update(dt)

                if hasattr(state_stack[-1], 'draw'):
                    state_stack[-1].draw(self)

                self.flip()

                # TODO: Should we pop dead states from the top of the stack first?
                # Will the state stack ever grow large enough for this to matter?

                if getattr(state_stack[-1], 'queued_state', None):
                    push_state(state_stack[-1].queued_state)

        push_state(initial_state)

        while state_stack and self.open:
            emit_frame(ae_frame_delta())

        # If the window closes in the middle of execution, call all cleanup code.
        while state_stack: pop_state()

        return self

# ==============================================================================
# ~ [ texture renderer ]
# ==============================================================================

cdef class Texture:
    """
    An image resident in server (GPU) memory used for faster rendering than what
    can be accomplished with blitting. Can also be used as a "framebuffer" for
    a software renderer (in case you want weird low-res effects without shaders).
    """
    cdef xl_texture_t* texture

    def __init__(self, size_t reference=0):
        self.texture = <xl_texture_t*>reference

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Texture other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return xl_texture_get_int(self.texture, XL_TEXTURE_PROPERTY_OPEN)

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    def __call__(self, *a, **k):
        return self.draw(*a, **k)

    @staticmethod
    def count_all(): return xl_texture_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid textures in a single list.
        """
        cdef xl_texture_t* textures[8192]
        cdef int i
        cdef list objects = []

        if xl_texture_count_all() > 8192:
            raise MemoryError("too many open textures for temp")

        xl_texture_list_all(textures)

        for i in range(<int>xl_texture_count_all()):
            objects.append(cls(reference = <size_t>textures[i]))

        return objects

    @staticmethod
    def close_all(): xl_texture_close_all()

    property id:
        def __get__(self):
            return xl_texture_get_int(self.texture, XL_TEXTURE_PROPERTY_ID)

    property width:
        def __get__(self):
            return xl_texture_get_int(self.texture, XL_TEXTURE_PROPERTY_WIDTH)

    property height:
        def __get__(self):
            return xl_texture_get_int(self.texture, XL_TEXTURE_PROPERTY_HEIGHT)

    property size:
        def __get__(self):
            return (self.width, self.height)

    property image:
        def __get__(self):
            cdef ae_image_t* c = xl_texture_get_img(self.texture, XL_TEXTURE_PROPERTY_IMAGE)
            cdef Image p = Image()

            # if we're not returning None (due to copy-on-upload being disabled), we must
            # make a copy of the image data for safe use with python's memory management.
            # this is the slow path you take by setting XL_TEXTURE_PROPERTY_COPY_ENABLED!

            if c != NULL:
                ae_image_copy(&p.image, c)
                return p

        def __set__(self, Image value):
            xl_texture_set_img(self.texture, XL_TEXTURE_PROPERTY_IMAGE, &value.image)

    property copy_enabled:
        def __get__(self):
            return xl_texture_get_int(self.texture, XL_TEXTURE_PROPERTY_COPY_ENABLED)

        def __set__(self, bint value):
            xl_texture_set_int(self.texture, XL_TEXTURE_PROPERTY_COPY_ENABLED, value)

    property draw_calls:
        def __get__(self):
            return xl_texture_get_int(self.texture, XL_TEXTURE_PROPERTY_DRAW_CALLS)

        def __set__(self, int value):
            xl_texture_set_int(self.texture, XL_TEXTURE_PROPERTY_DRAW_CALLS, value)

    property window:
        def __get__(self):
            return Window(reference = <size_t> xl_texture_get_ptr(
                        self.texture, XL_TEXTURE_PROPERTY_WINDOW))

    property status:
        def __get__(self):
            cdef bytes s = xl_texture_get_str(self.texture, XL_TEXTURE_PROPERTY_STATUS)
            return s.decode() if sys.version_info.major > 2 else s # convert to unicode

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_texture_set_str(self.texture, XL_TEXTURE_PROPERTY_STATUS, <char*>string)

    property path:
        def __get__(self):
            cdef bytes s = xl_texture_get_str(self.texture, XL_TEXTURE_PROPERTY_PATH)
            return s.decode() if sys.version_info.major > 2 else s # convert to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_texture_set_str(self.texture, XL_TEXTURE_PROPERTY_PATH, <char*>string)

    property name:
        def __get__(self):
            cdef bytes s = xl_texture_get_str(self.texture, XL_TEXTURE_PROPERTY_NAME)
            return s.decode() if sys.version_info.major > 2 else s # convert to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_texture_set_str(self.texture, XL_TEXTURE_PROPERTY_NAME, <char*>string)

    property red:
        def __get__(self):
            return xl_texture_get_flt(self.texture, XL_TEXTURE_PROPERTY_RED)

        def __set__(self, float value):
            xl_texture_set_flt(self.texture, XL_TEXTURE_PROPERTY_RED, value)

    property green:
        def __get__(self):
            return xl_texture_get_flt(self.texture, XL_TEXTURE_PROPERTY_GREEN)

        def __set__(self, float value):
            xl_texture_set_flt(self.texture, XL_TEXTURE_PROPERTY_GREEN, value)

    property blue:
        def __get__(self):
            return xl_texture_get_flt(self.texture, XL_TEXTURE_PROPERTY_BLUE)

        def __set__(self, float value):
            xl_texture_set_flt(self.texture, XL_TEXTURE_PROPERTY_BLUE, value)

    property alpha:
        def __get__(self):
            return xl_texture_get_flt(self.texture, XL_TEXTURE_PROPERTY_ALPHA)

        def __set__(self, float value):
            xl_texture_set_flt(self.texture, XL_TEXTURE_PROPERTY_ALPHA, value)

    property rgb:
        def __get__(self):
            cdef void* c = xl_texture_get_ptr(self.texture, XL_TEXTURE_PROPERTY_RGB)
            cdef Vec3 p = Vec3()

            if c != NULL: vec3copy(p.v, <float*>c)
            return p

        def __set__(self, Vec3 value):
            xl_texture_set_ptr(self.texture, XL_TEXTURE_PROPERTY_RGB, value.v)

    property rgba:
        def __get__(self):
            cdef void* c = xl_texture_get_ptr(self.texture, XL_TEXTURE_PROPERTY_RGBA)
            cdef Vec4 p = Vec4()

            if c != NULL: vec4copy(p.v, <float*>c)
            return p

        def __set__(self, Vec4 value):
            xl_texture_set_ptr(self.texture, XL_TEXTURE_PROPERTY_RGBA, value.v)

    property flip:
        def __get__(self):
            cdef bytes s = xl_texture_get_str(self.texture, XL_TEXTURE_PROPERTY_FLIP)
            return s.decode() if sys.version_info.major > 2 else s # convert to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_texture_set_str(self.texture, XL_TEXTURE_PROPERTY_FLIP, <char*>string)

    property open:
        def __get__(self):
            return xl_texture_get_int(self.texture, XL_TEXTURE_PROPERTY_OPEN)

        def __set__(self, bint value):
            xl_texture_set_int(self.texture, XL_TEXTURE_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.texture

    def cast(self):
        return self

    def close(self):
        xl_texture_close(self.texture); return self

    def create(self, Window window, int width, int height, **kwargs):
        """
        Manually create a texture (set pixel data with the image property).
        """
        if not self.open:
            self.texture = xl_texture_create(window.window, width, height)

            # convenient way to set some texture attributes inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def load_from_memory(self, Window window, size_t ptr, size_t length, **kwargs):
        """
        Load a compressed image file into a texture. Raises IOError if load fails.
        """
        cdef ae_image_error_t error = AE_IMAGE_NO_CODEC # stub default

        if not self.open:
            self.texture = xl_texture_load_from_memory_ex(window.window,
                                            <void*>ptr, length, &error)

            if error != AE_IMAGE_SUCCESS:
                raise IOError(ae_image_error_message(error, NULL))

            # convenient way to set some texture attributes inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def load_from_array(self, Window window, Array data, **kwargs):
        """
        Load a texture from an array, most likely a subview pointing to an archive slot.
        """
        return self.load_from_memory(window, data.address(), len(data), **kwargs)

    def load_from_bytes(self, Window window, bytes data, **kwargs):
        return self.load_from_memory(window, <size_t>(<char*>data), len(data), **kwargs)

    def load(self, Window window, str filename, **kwargs):
        """
        Load an image into a texture. Raises IOError if image loading fails.
        """
        cdef ae_image_error_t error = AE_IMAGE_NO_CODEC # default for stubs

        cdef bytes b_filename
        cdef bytes err_string

        # convert unicode filename to ascii in python 3, leave it otherwise
        if sys.version_info.major > 2:
            b_filename = <bytes>filename.encode('utf-8')
        else:
            b_filename = <bytes>filename

        if not self.open:
            self.texture = xl_texture_load_ex(window.window,
                                <char *>b_filename, &error)

            if error != AE_IMAGE_SUCCESS:
                err_string = ae_image_error_message(error, <char*>b_filename)

                if sys.version_info.major > 2:
                    raise IOError(err_string.decode()) # convert to unicode
                else:
                    raise IOError(err_string) # use oldschool ascii string

            # convenient way to set some texture attributes inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def rect(self, Vec2 pos=Vec2(), Vec2 scale=Vec2(1.0, 1.0)):
        """
        Create a destination (or source) rect for rendering. Makes draw_ex easy.
        """
        cdef FloatRect r = FloatRect()

        r.rect[0] = pos.v[0]
        r.rect[1] = pos.v[1]

        r.rect[2] = xl_texture_get_flt(self.texture, XL_TEXTURE_PROPERTY_WIDTH )
        r.rect[3] = xl_texture_get_flt(self.texture, XL_TEXTURE_PROPERTY_HEIGHT)

        r.rect[2] *= scale.v[0]
        r.rect[3] *= scale.v[1]

        return r

    def draw_ex(self, FloatRect src_rect=None, FloatRect dst_rect=None,
                double angle=0.0, Vec2 center=None):
        """
        Draw the texture with optional clipping (src), rotation, and scale (dst).
        """
        xl_texture_draw_ex( self.texture, # NULL rects select entire area
                            NULL if src_rect is None else src_rect.rect,
                            NULL if dst_rect is None else dst_rect.rect,
                            angle, NULL if center is None else center.v)
        return self

    def draw(self, Vec2 pos=Vec2()):
        """
        Draw texture without clipping, rotation, or scaling (just translation).
        """
        xl_texture_draw(self.texture, pos.v); return self

# ==============================================================================
# ~ [ font renderer ]
# ==============================================================================

cdef class Font:
    """
    A TrueType font file used for rendering text to the screen or an image.
    """
    cdef xl_font_t* font

    def __init__(self, size_t reference=0):
        self.font = <xl_font_t*>reference

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Font other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return xl_font_get_int(self.font, XL_FONT_PROPERTY_OPEN)

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    def __call__(self, *a, **k):
        return self.draw(*a, **k)

    @staticmethod
    def count_all(): return xl_font_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid fonts in a single list.
        """
        cdef xl_font_t* fonts[1024]
        cdef int i
        cdef list objects = []

        if xl_font_count_all() > 1024:
            raise MemoryError("too many open fonts for temp")

        xl_font_list_all(fonts)

        for i in range(<int>xl_font_count_all()):
            objects.append(cls(reference = <size_t>fonts[i]))

        return objects

    @staticmethod
    def close_all(): xl_font_close_all()

    property id:
        def __get__(self):
            return xl_font_get_int(self.font, XL_FONT_PROPERTY_ID)

    property point_size:
        def __get__(self):
            return xl_font_get_int(self.font, XL_FONT_PROPERTY_POINT_SIZE)

        def __set__(self, int value):
            xl_font_set_int(self.font, XL_FONT_PROPERTY_POINT_SIZE, value)

    property line_skip:
        def __get__(self):
            return xl_font_get_int(self.font, XL_FONT_PROPERTY_LINE_SKIP)

    property window:
        def __get__(self):
            return Window(reference=<size_t>xl_font_get_ptr(
                        self.font, XL_FONT_PROPERTY_WINDOW))

    property status:
        def __get__(self):
            cdef bytes s = xl_font_get_str(self.font, XL_FONT_PROPERTY_STATUS)
            return s.decode() if sys.version_info.major > 2 else s

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_font_set_str(self.font, XL_FONT_PROPERTY_STATUS, <char*>string)

    property path:
        def __get__(self):
            cdef bytes s = xl_font_get_str(self.font, XL_FONT_PROPERTY_PATH)
            return s.decode() if sys.version_info.major > 2 else s

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_font_set_str(self.font, XL_FONT_PROPERTY_PATH, <char*>string)

    property name:
        def __get__(self):
            cdef bytes s = xl_font_get_str(self.font, XL_FONT_PROPERTY_NAME)
            return s.decode() if sys.version_info.major > 2 else s

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            xl_font_set_str(self.font, XL_FONT_PROPERTY_NAME, <char*>string)

    property red:
        def __get__(self):
            return xl_font_get_flt(self.font, XL_FONT_PROPERTY_RED)

        def __set__(self, float value):
            xl_font_set_flt(self.font, XL_FONT_PROPERTY_RED, value)

    property green:
        def __get__(self):
            return xl_font_get_flt(self.font, XL_FONT_PROPERTY_GREEN)

        def __set__(self, float value):
            xl_font_set_flt(self.font, XL_FONT_PROPERTY_GREEN, value)

    property blue:
        def __get__(self):
            return xl_font_get_flt(self.font, XL_FONT_PROPERTY_BLUE)

        def __set__(self, float value):
            xl_font_set_flt(self.font, XL_FONT_PROPERTY_BLUE, value)

    property alpha:
        def __get__(self):
            return xl_font_get_flt(self.font, XL_FONT_PROPERTY_ALPHA)

        def __set__(self, float value):
            xl_font_set_flt(self.font, XL_FONT_PROPERTY_ALPHA, value)

    property rgb:
        def __get__(self):
            cdef void* c = xl_font_get_ptr(self.font, XL_FONT_PROPERTY_RGB)
            cdef Vec3 p = Vec3()

            if c != NULL: vec3copy(p.v, <float*>c)
            return p

        def __set__(self, Vec3 value):
            xl_font_set_ptr(self.font, XL_FONT_PROPERTY_RGB, value.v)

    property rgba:
        def __get__(self):
            cdef void* c = xl_font_get_ptr(self.font, XL_FONT_PROPERTY_RGBA)
            cdef Vec4 p = Vec4()

            if c != NULL: vec4copy(p.v, <float*>c)
            return p

        def __set__(self, Vec4 value):
            xl_font_set_ptr(self.font, XL_FONT_PROPERTY_RGBA, value.v)

    property open:
        def __get__(self):
            return xl_font_get_int(self.font, XL_FONT_PROPERTY_OPEN)

        def __set__(self, bint value):
            xl_font_set_int(self.font, XL_FONT_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.font

    def cast(self):
        return self

    def close(self):
        xl_font_close(self.font); return self

    def load_from_memory(self, Window window, size_t ptr,
                size_t length, int point_size, **kwargs):
        """
        Load a TrueType font from a pointer (cast to an integer) and a buffer size.
        """
        if not self.open:
            self.font = xl_font_load_from_memory(window.window,
                                <void*>ptr, length, point_size)

            # convenient way to set font attributes inline on loading
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def load_from_array(self, Window window, Array font, int point_size, **kwargs):
        """
        Load a TrueType font from an aegame Array object (see mem.Array.append_file).
        """
        return self.load_from_memory( window, font.address(),
                            len(font), point_size, **kwargs)

    def load_from_bytes(self, Window window, bytes font, int point_size, **kwargs):
        """
        Load a TrueType font from a Python bytes object (return type of file.read).
        """
        return self.load_from_memory( window, <size_t>(<char *>font),
                                    len(font), point_size, **kwargs)

    def load(self, Window window, str filename, int point_size, **kwargs):
        """
        Load a TrueType font from a file. Font path and name are automatically set.
        """
        cdef bytes b_filename

        if not self.open:
            # convert the filename from a unicode string in python 3 to ascii bytes
            if sys.version_info.major > 2:
                b_filename = <bytes>filename.encode('utf-8')
            else:
                b_filename = <bytes>filename

            # TODO: when xl_font_load_ex exists, call it instead for exception data
            self.font = xl_font_load(window.window, <char*>b_filename, point_size)

            # for convenience, set named properties on load
            for k, v in kwargs.items(): setattr(self, k, v)

        return self

    def load_system_monospace(self, Window window, int point_size, **kwargs):
        """
        Load a monospace font from the operating system. Useful for debugging tools.
        """
        if not self.open:
            # TODO: when xl_font_load_system_monospace_ex exists, throw on errors
            self.font = xl_font_load_system_monospace(window.window, point_size)

            # for convenience, set named properties on load
            for k, v in kwargs.items(): setattr(self, k, v)

        return self

    def text_size(self, bytes string):
        """
        Get the size of a string, as it would be rendered to an image or texture.
        """
        cdef int w = 0
        cdef int h = 0

        xl_font_text_size(self.font, &w, &h, "%s", <char*>string)
        return (w, h)

    def render_image(self, bytes string, object cls=Image):
        """
        Create a new image object with `string` rendered into it.
        """
        cdef Image image = cls()

        xl_font_render_image(self.font, &image.image,
                                "%s", <char *>string)
        return image

    def render_texture(self, bytes string, object cls=Texture):
        """
        Create a new texture object with `string` rendered into it.
        """
        return cls(< size_t >xl_font_render_texture(self.font,
                                        "%s", <char *>string))

    def blit( self, bytes string, Image dst, int x, int y,
                                bint r=True, bint g=True,
                                bint b=True, bint a=True):
        """
        Blit a string into an image at (x, y), using standard alpha blending.
        """
        xl_font_blit(self.font, &dst.image, x, y, r, g, b, a,
                                        "%s", <char *>string)
        return self

    def draw(self, bytes string, Vec2 pos):
        """
        Render a string into the current window render target at (x, y).
        """
        xl_font_draw(self.font, pos.v, "%s", <char *>string)
        return self

# ==============================================================================
# ~ [ streaming music ]
# ==============================================================================

cdef class Music:
    """
    Handles a single global streaming (read in chunks) compressed audio file.
    Do not create an instance of this; it's an interface to a global system.
    """
    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __nonzero__(self):
        return xl_music_get_int(XL_MUSIC_PROPERTY_PLAYING)

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    def __call__(self, *a, **k):
        return self.play(*a, **k)

    property playing:
        def __get__(self):
            return xl_music_get_int(XL_MUSIC_PROPERTY_PLAYING)

        def __set__(self, bint value):
            xl_music_set_int(XL_MUSIC_PROPERTY_PLAYING, value)

    property paused:
        def __get__(self):
            return xl_music_get_int(XL_MUSIC_PROPERTY_PAUSED)

        def __set__(self, bint value):
            xl_music_set_int(XL_MUSIC_PROPERTY_PAUSED, value)

    property fading_in:
        def __get__(self):
            return xl_music_get_int(XL_MUSIC_PROPERTY_FADING_IN)

    property fading_out:
        def __get__(self):
            return xl_music_get_int(XL_MUSIC_PROPERTY_FADING_OUT)

    property status:
        def __get__(self):
            return xl_music_get_str(XL_MUSIC_PROPERTY_STATUS)

        def __set__(self, bytes value):
            xl_music_set_str(XL_MUSIC_PROPERTY_STATUS, <char*>value)

    property duration:
        def __get__(self):
            return xl_music_get_dbl(XL_MUSIC_PROPERTY_DURATION)

    property position:
        def __get__(self):
            return xl_music_get_dbl(XL_MUSIC_PROPERTY_POSITION)

        def __set__(self, double value):
            xl_music_set_dbl(XL_MUSIC_PROPERTY_POSITION, value)

    property volume:
        def __get__(self):
            return xl_music_get_dbl(XL_MUSIC_PROPERTY_VOLUME)

        def __set__(self, double value):
            xl_music_set_dbl(XL_MUSIC_PROPERTY_VOLUME, value)

    property path:
        def __get__(self):
            return xl_music_get_str(XL_MUSIC_PROPERTY_PATH)

        def __set__(self, bytes value):
            xl_music_set_str(XL_MUSIC_PROPERTY_PATH, <char*>value)

    property name:
        def __get__(self):
            return xl_music_get_str(XL_MUSIC_PROPERTY_NAME)

        def __set__(self, bytes value):
            xl_music_set_str(XL_MUSIC_PROPERTY_NAME, <char*>value)

    @classmethod
    def fade_in( cls, bytes filename, bint loop, double fade_in,
                                    double start_pos, **kwargs):
        """
        Begin streaming a music file at a given point (in seconds),
        optionally fading in from silence over a number of seconds.
        """
        xl_music_fade_in(<char*>filename, loop, fade_in, start_pos)

        # for k, v in kwargs.items(): setattr(cls, k, v)
        for k, v in kwargs.items(): setattr(music, k, v)

    @staticmethod
    def fade_out(double fade_out): xl_music_fade_out(fade_out)

    @classmethod
    def play(cls, bytes filename, **kwargs):
        """
        Begin streaming a music file immediately from the beginning.
        """
        cls.fade_in(filename, False, 0.0, 0.0, **kwargs)

    @staticmethod
    def stop(): xl_music_stop()

music = Music()

# ==============================================================================
# ~ [ sound effects ]
# ==============================================================================

cdef class Sound:
    """
    A static (completely loaded into memory) audio file for immediate playback.
    """
    cdef xl_sound_t* sound

    def __init__(self, size_t reference=0):
        self.sound = <xl_sound_t*>reference

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Sound other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return xl_sound_get_int(self.sound, XL_SOUND_PROPERTY_OPEN)

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    def __call__(self, *a, **k):
        return self.play(*a, **k)

    @staticmethod
    def count_all(): return xl_sound_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid sounds in a single list.
        """
        cdef xl_sound_t* sounds[4096]
        cdef int i
        cdef list objects = []

        if xl_sound_count_all() > 4096:
            raise MemoryError("too many open sounds for temp")

        xl_sound_list_all(sounds)

        for i in range(<int>xl_sound_count_all()):
            objects.append(cls(reference = <size_t>sounds[i]))

        return objects

    @staticmethod
    def close_all(): xl_sound_close_all()

    property id:
        def __get__(self):
            return xl_sound_get_int(self.sound, XL_SOUND_PROPERTY_ID)

    property duration:
        def __get__(self):
            return xl_sound_get_dbl(self.sound, XL_SOUND_PROPERTY_DURATION)

    property volume:
        def __get__(self):
            return xl_sound_get_dbl(self.sound, XL_SOUND_PROPERTY_VOLUME)

        def __set__(self, double value):
            xl_sound_set_dbl(self.sound, XL_SOUND_PROPERTY_VOLUME, value)

    property status:
        def __get__(self):
            return xl_sound_get_str(self.sound, XL_SOUND_PROPERTY_STATUS) # repr

        def __set__(self, bytes value):
            xl_sound_set_str(self.sound, XL_SOUND_PROPERTY_STATUS, <char*>value)

    property path:
        def __get__(self):
            return xl_sound_get_str(self.sound, XL_SOUND_PROPERTY_PATH)

        def __set__(self, bytes value):
            xl_sound_set_str(self.sound, XL_SOUND_PROPERTY_PATH, <char*>value)

    property name:
        def __get__(self):
            return xl_sound_get_str(self.sound, XL_SOUND_PROPERTY_NAME)

        def __set__(self, bytes value):
            xl_sound_set_str(self.sound, XL_SOUND_PROPERTY_NAME, <char*>value)

    property open:
        def __get__(self):
            return xl_sound_get_int(self.sound, XL_SOUND_PROPERTY_OPEN)

        def __set__(self, bint value):
            xl_sound_set_int(self.sound, XL_SOUND_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.sound

    def cast(self):
        return self

    def close(self):
        xl_sound_close(self.sound); return self

    def load_from_memory(self, size_t ptr, size_t length, **kwargs):
        """
        Load a sound effect from a pointer (cast to int) and a buffer size.
        """
        if not self.open:
            self.sound = xl_sound_load_from_memory(<void*>ptr, length)

            # for convenience, set named properties on load
            for k, v in kwargs.items(): setattr(self, k, v)

        return self

    def load_from_array(self, Array sound, **kwargs):
        return self.load_from_memory(sound.address(), len(sound), **kwargs)

    def load_from_bytes(self, bytes sound, **kwargs):
        return self.load_from_memory(<size_t>(<char *>sound),
                                        len(sound), **kwargs)

    def load(self, bytes filename, **kwargs):
        """
        Load a sound effect from a file. Path and name are automatically set.
        """
        if not self.open:
            self.sound = xl_sound_load(<char*>filename)

            # for convenience, set named properties on load
            for k, v in kwargs.items(): setattr(self, k, v)

        return self

    def play(self, int count = 1, double fade_in = 0, double length = -1):
        """
        Play the sound N times, with an optional fade in effect and cutoff.
        """
        xl_sound_fade_in(self.sound, count, fade_in, length); return self

    fade_in = play

    def fade_out(self, double fade_out):
        """
        Fade out all playing instances of this sound effect across N seconds.
        """
        xl_sound_fade_out(self.sound, fade_out); return self

    @staticmethod
    def fade_all_out(double fade_out): xl_sound_fade_out(NULL, fade_out)

    def stop(self):
        """
        Immediately stop all playing instances of this static sound effect.
        """
        xl_sound_stop(self.sound); return self

    @staticmethod
    def stop_all(): xl_sound_stop(NULL)

# ==============================================================================
# ~ [ controller input ]
# ==============================================================================

cdef class Controller:
    """
    Joystick and gamepad interface. May also support other exotic hardware like
    flight sticks, racing wheels, drumkits, and/or Guitar Hero controllers.
    Game controllers can't be created or destroyed programatically, only plugged
    in and unplugged by the user (use the event system to monitor controllers).
    """
    cdef xl_controller_t* controller

    def __init__(self, size_t reference):
        self.controller = <xl_controller_t*>reference

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Controller other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return xl_controller_get_int(self.controller, XL_CONTROLLER_PROPERTY_OPEN)

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    def __call__(self, bytes button):
        return bool(xl_controller_button_is_down(self.controller, # magic cast!
                    xl_controller_button_index_from_short_name(<char*>button)))

    @staticmethod
    def count_all(): return xl_controller_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open controllers in a single list.
        """
        cdef xl_controller_t* controllers[256]
        cdef int i
        cdef list objects = []

        if xl_controller_count_all() > 256:
            raise MemoryError("too many open controllers for temp")

        xl_controller_list_all(controllers)

        for i in range(<int>xl_controller_count_all()):
            objects.append(cls(reference = <size_t>controllers[i]))

        return objects

    property id:
        def __get__(self):
            return xl_controller_get_int(self.controller, XL_CONTROLLER_PROPERTY_ID)

    def _get_button_list(self, xl_controller_property_t prop):
        cdef int button_mask = xl_controller_get_int(self.controller, prop)
        cdef int i, b
        cdef list button_list = []

        for i in range(XL_CONTROLLER_BUTTON_INDEX_COUNT):
            b = 1 << i

            if (button_mask & b) != 0:
                button_list.append(xl_controller_button_short_name[i])

        return button_list

    property down_buttons:
        def __get__(self):
            return self._get_button_list(XL_CONTROLLER_PROPERTY_DOWN_BUTTONS)

    property up_buttons:
        def __get__(self):
            return self._get_button_list(XL_CONTROLLER_PROPERTY_UP_BUTTONS)

    property shoulder_tribool:
        def __get__(self): return xl_controller_get_int(self.controller,
                                XL_CONTROLLER_PROPERTY_SHOULDER_TRIBOOL)

    property dpad_horizontal_tribool:
        def __get__(self): return xl_controller_get_int(self.controller,
                        XL_CONTROLLER_PROPERTY_DPAD_HORIZONTAL_TRIBOOL)

    property dpad_vertical_tribool:
        def __get__(self): return xl_controller_get_int( self.controller,
                            XL_CONTROLLER_PROPERTY_DPAD_VERTICAL_TRIBOOL)

    property stick_tribool:
        def __get__(self): return xl_controller_get_int( self.controller,
                                    XL_CONTROLLER_PROPERTY_STICK_TRIBOOL)

    property last_pressed_button:
        def __get__(self):
            return xl_controller_button_short_name[xl_controller_get_int(
                self.controller, XL_CONTROLLER_PROPERTY_LAST_PRESSED_BUTTON)]

    property last_released_button:
        def __get__(self):
            return xl_controller_button_short_name[xl_controller_get_int(
                self.controller, XL_CONTROLLER_PROPERTY_LAST_RELEASED_BUTTON)]

    property last_pressed_time:
        def __get__(self): return xl_controller_get_dbl(self.controller,
                            XL_CONTROLLER_PROPERTY_LAST_PRESSED_TIME)

    property last_released_time:
        def __get__(self): return xl_controller_get_dbl(self.controller,
                            XL_CONTROLLER_PROPERTY_LAST_RELEASED_TIME)

    property right_trigger:
        def __get__(self): return xl_controller_get_dbl(self.controller,
                                XL_CONTROLLER_PROPERTY_RIGHT_TRIGGER)

    property left_trigger:
        def __get__(self): return xl_controller_get_dbl(self.controller,
                                    XL_CONTROLLER_PROPERTY_LEFT_TRIGGER)

    property right_deadzone:
        def __get__(self): return (
            xl_controller_get_str(self.controller, XL_CONTROLLER_PROPERTY_RIGHT_DEADZONE_MODE),
            xl_controller_get_dbl(self.controller, XL_CONTROLLER_PROPERTY_RIGHT_DEADZONE_VALUE))

        def __set__(self, tuple value):
            cdef bytes mode = value[0]

            xl_controller_set_str(self.controller, XL_CONTROLLER_PROPERTY_RIGHT_DEADZONE_MODE, <char*>mode)
            xl_controller_set_dbl(self.controller, XL_CONTROLLER_PROPERTY_RIGHT_DEADZONE_VALUE, value[1])

    property left_deadzone:
        def __get__(self): return (
            xl_controller_get_str(self.controller, XL_CONTROLLER_PROPERTY_LEFT_DEADZONE_MODE),
            xl_controller_get_dbl(self.controller, XL_CONTROLLER_PROPERTY_LEFT_DEADZONE_VALUE))

        def __set__(self, tuple value):
            cdef bytes mode = value[0]

            xl_controller_set_str(self.controller, XL_CONTROLLER_PROPERTY_LEFT_DEADZONE_MODE, <char*>mode)
            xl_controller_set_dbl(self.controller, XL_CONTROLLER_PROPERTY_LEFT_DEADZONE_VALUE, value[1])

    property right_stick_angle:
        def __get__(self): return xl_controller_get_dbl( self.controller,
                                XL_CONTROLLER_PROPERTY_RIGHT_STICK_ANGLE)

    property right_stick_magnitude:
        def __get__(self): return xl_controller_get_dbl( self.controller,
                            XL_CONTROLLER_PROPERTY_RIGHT_STICK_MAGNITUDE)

    property left_stick_angle:
        def __get__(self): return xl_controller_get_dbl(self.controller,
                                XL_CONTROLLER_PROPERTY_LEFT_STICK_ANGLE)

    property left_stick_magnitude:
        def __get__(self): return xl_controller_get_dbl(self.controller,
                            XL_CONTROLLER_PROPERTY_LEFT_STICK_MAGNITUDE)

    property right_stick:
        def __get__(self): return (
            xl_controller_get_dbl(self.controller, XL_CONTROLLER_PROPERTY_RIGHT_STICK_X),
            xl_controller_get_dbl(self.controller, XL_CONTROLLER_PROPERTY_RIGHT_STICK_Y))

    property left_stick:
        def __get__(self): return (
            xl_controller_get_dbl(self.controller, XL_CONTROLLER_PROPERTY_LEFT_STICK_X),
            xl_controller_get_dbl(self.controller, XL_CONTROLLER_PROPERTY_LEFT_STICK_Y))

    property status:
        def __get__(self): return xl_controller_get_str(  self.controller,
                                            XL_CONTROLLER_PROPERTY_STATUS)

        def __set__(self, bytes value):
            xl_controller_set_str(self.controller, XL_CONTROLLER_PROPERTY_STATUS,
                                                                    <char*>value)

    property name:
        def __get__(self): return xl_controller_get_str(self.controller,
                                            XL_CONTROLLER_PROPERTY_NAME)

    property open:
        """
        Returns True if the controller is active (currently plugged in).
        """
        def __get__(self): return xl_controller_get_int(self.controller,
                                            XL_CONTROLLER_PROPERTY_OPEN)

    def address(self):
        return <size_t>self.controller

    def cast(self):
        return self

    def last_button_pressed_time(self, bytes button):
        """
        Get the absolute last time a given button was pressed. To get the relative
        time (time since press), use aegame.utl.seconds() - last button press time.
        """
        return (xl_controller_get_last_button_pressed_time(self.controller,
                xl_controller_button_index_from_short_name(<char*>button)))

    def last_button_released_time(self, bytes button):
        """
        Get the last absolute time the user let go of a given controller button.
        """
        return (xl_controller_get_last_button_released_time(self.controller,
                xl_controller_button_index_from_short_name(<char*>button)))

    def clear_history(self):
        """
        Reset input tracking so a cheat code doesn't register an effect many times.
        """
        xl_controller_clear_history(self.controller); return self

    cdef int _button_mask(self, object button):
        cdef int mask = 0

        if isinstance(button, list):
            for sb in button:
                mask |= <int>xl_controller_button_bit_from_short_name(<char*>sb)
        else:
            mask |= <int>xl_controller_button_bit_from_short_name(<char*>button)

        return mask

    def check_history(self, list buttons):
        """
        Returns True if the last N controller inputs match a list. This can be used
        for cheat code and fighting game combo systems. List items can either be
        button names as strings, or another list of buttons for (A + B, C) combos.
        """
        cdef int m[1024]
        cdef int i

        if len(buttons) > 1024: raise MemoryError("too many buttons for temp!")
        for i, button in enumerate(buttons): m[i] = self._button_mask(button)

        return bool(xl_controller_check_history(self.controller, m, len(buttons)))

    def get_trigger(self, bytes which):
        """
        Get the shoulder trigger state based on a character identifier ('L', 'R').
        """
        return xl_controller_get_trigger(self.controller, <char>ord(which))

    def get_deadzone(self, bytes which):
        cdef xl_controller_deadzone_mode_t mode
        cdef double value

        xl_controller_get_deadzone(self.controller, <char>ord(which), &mode, &value)
        return (xl_controller_deadzone_short_name[<size_t>mode], value)

    def set_deadzone(self, bytes which, bytes mode, double value):
        """
        Set the deadzone mode and margin, for dealing with old worn out joysticks.
        """
        xl_controller_set_deadzone(self.controller, <char>ord(which),
            xl_controller_deadzone_mode_from_short_name(<char*>mode), value)

        return self

    def get_stick_angle(self, bytes which):
        return xl_controller_get_stick_angle(self.controller, <char>ord(which))

    def get_stick_magnitude(self, bytes which):
        return xl_controller_get_stick_magnitude(self.controller, <char>ord(which))

    def get_stick(self, bytes which):
        cdef double x
        cdef double y

        xl_controller_get_stick(self.controller, <char>ord(which), &x, &y)
        return (x, y)

# ==============================================================================
# ~ [ atlas animation ]
# ==============================================================================

cdef class Animation:
    """
    Render a sequence of frames within a single texture atlas, over a set period
    of time. Can be loaded from a variety of sources (textures, archives, dirs).
    """
    cdef xl_animation_t* animation

    def __init__(self, size_t reference=0):
        self.animation = <xl_animation_t*>reference

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Animation other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_OPEN)

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        return self.__class__(reference=<size_t>xl_animation_copy(self.animation))

    copy = __copy__

    def __call__(self, *a, **k):
        return self.draw(*a, **k)

    @staticmethod
    def count_all(): return xl_animation_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid animations in a single list.
        """
        cdef xl_animation_t* animations[2048]
        cdef int i
        cdef list objects = []

        if xl_animation_count_all() > 2048:
            raise MemoryError("too many open animations for temp")

        xl_animation_list_all(animations)

        for i in range(<int>xl_animation_count_all()):
            objects.append(cls(reference = <size_t>animations[i]))

        return objects

    @staticmethod
    def close_all(): xl_animation_close_all()

    property id:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_ID)

    property atlas:
        def __get__(self):
            return Texture(reference=<size_t>xl_animation_get_tex( # get tex ref
                                    self.animation, XL_ANIMATION_PROPERTY_ATLAS))

        def __set__(self, Texture value):
            xl_animation_set_tex(self.animation, XL_ANIMATION_PROPERTY_ATLAS,
                                value.texture if value is not None else NULL)

    property owns_atlas:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_OWNS_ATLAS)

        def __set__(self, bint value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_OWNS_ATLAS, value)

    property frame_width:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_FRAME_WIDTH)

        def __set__(self, int value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_FRAME_WIDTH, value)

    property frame_height:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_FRAME_HEIGHT)

        def __set__(self, int value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_FRAME_HEIGHT, value)

    property frame_size:
        def __get__(self):
            return (self.frame_width, self.frame_height)

        def __set__(self, object value):
            self.frame_width, self.frame_height = value

    property first_frame:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_FIRST_FRAME)

        def __set__(self, int value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_FIRST_FRAME, value)

    property frame_count:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_FRAME_COUNT)

        def __set__(self, int value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_FRAME_COUNT, value)

    property frame_time:
        def __get__(self):
            return xl_animation_get_dbl(self.animation, XL_ANIMATION_PROPERTY_FRAME_TIME)

        def __set__(self, double value):
            xl_animation_set_dbl(self.animation, XL_ANIMATION_PROPERTY_FRAME_TIME, value)

    property total_time:
        def __get__(self):
            return xl_animation_get_dbl(self.animation, XL_ANIMATION_PROPERTY_TOTAL_TIME)

        def __set__(self, double value):
            xl_animation_set_dbl(self.animation, XL_ANIMATION_PROPERTY_TOTAL_TIME, value)

    property current_frame:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_CURRENT_FRAME)

        def __set__(self, int value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_CURRENT_FRAME, value)

    property position:
        def __get__(self):
            return xl_animation_get_dbl(self.animation, XL_ANIMATION_PROPERTY_POSITION)

        def __set__(self, double value):
            xl_animation_set_dbl(self.animation, XL_ANIMATION_PROPERTY_POSITION, value)

    property loops:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_LOOPS)

        def __set__(self, bint value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_LOOPS, value)

    property finished:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_FINISHED)

        def __set__(self, bint value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_FINISHED, value)

    property status:
        def __get__(self):
            return xl_animation_get_str(self.animation, XL_ANIMATION_PROPERTY_STATUS) # repr

        def __set__(self, bytes value):
            xl_animation_set_str(self.animation, XL_ANIMATION_PROPERTY_STATUS, <char*>value)

    property path:
        def __get__(self):
            return xl_animation_get_str(self.animation, XL_ANIMATION_PROPERTY_PATH)

        def __set__(self, bytes value):
            xl_animation_set_str(self.animation, XL_ANIMATION_PROPERTY_PATH, <char*>value)

    property name:
        def __get__(self):
            return xl_animation_get_str(self.animation, XL_ANIMATION_PROPERTY_NAME)

        def __set__(self, bytes value):
            xl_animation_set_str(self.animation, XL_ANIMATION_PROPERTY_NAME, <char*>value)

    property open:
        def __get__(self):
            return xl_animation_get_int(self.animation, XL_ANIMATION_PROPERTY_OPEN)

        def __set__(self, bint value):
            xl_animation_set_int(self.animation, XL_ANIMATION_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.animation

    def cast(self):
        return self

    def close(self):
        xl_animation_close(self.animation); return self

    def create(self, **kwargs):
        """
        Manually create a new animation, and set properties via keyword arguments.
        """
        if not self.open:
            self.animation = xl_animation_create()

            # convenient way to set named animation properties inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def load(self, Window window, bytes filename, int frame_width=0,
                                    int frame_height=0, **kwargs ):
        """
        Load an animation file (supported file types depend upon the implementation).
        """
        cdef ae_image_error_t error_status = AE_IMAGE_NO_CODEC # default stub error

        if not self.open:
            self.animation = xl_animation_load_ex(window.window, <char*>filename,
                                        frame_width, frame_height, &error_status)

            if error_status != AE_IMAGE_SUCCESS:
                raise IOError(ae_image_error_message(error_status, <char*>filename))

            # convenient way to set named animation properties inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    @staticmethod
    def reset_all():
        xl_animation_reset_all()

    def reset(self):
        xl_animation_reset(self.animation); return self

    @staticmethod
    def update_all(double dt):
        """
        Advance all active animations by a given number of seconds (usually frame delta).
        """
        xl_animation_update_all(dt)

    def update(self, double dt):
        """
        Advance this animation by a given number of seconds (usually frame time delta).
        """
        xl_animation_update(self.animation, dt); return self

    def src_rect(self):
        """
        Map the 1D animation index (current frame) to the 2D region of the atlas to draw.
        """
        cdef FloatRect rect = FloatRect()

        xl_animation_src_rect(self.animation, rect.rect)
        return rect

    def dst_rect(self, Vec2 pos=Vec2(), Vec2 scale=Vec2(1.0, 1.0)):
        """
        Create a destination rect (region of the render target to stretch frame onto).
        """
        cdef FloatRect rect = FloatRect()

        xl_animation_dst_rect(self.animation, rect.rect, pos.v, scale.v)
        return rect

    rect = dst_rect

    def draw_ex(self, FloatRect dst_rect=None, double angle=0.0, Vec2 center=None):
        """
        Draw the current frame with rotation and scaling (no clipping of source rect).
        """
        xl_animation_draw_ex(self.animation,

            NULL if dst_rect is None else dst_rect.rect,
            angle, NULL if center is None else center.v)

        return self

    def draw(self, Vec2 pos=Vec2()):
        """
        Draw the current frame without rotation, scaling, or clipping of the source rect.
        """
        xl_animation_draw(self.animation, pos.v); return self

# ==============================================================================
# ~ [ event handling ]
# ==============================================================================

class event(object):
    """
    Interface to the system event queue that notifies us on events like a window
    being resized, or a controller being plugged in. The user can dispatch events
    to an event handler object, and/or iterate through events in a Python list.
    """
    @staticmethod
    def count_pending():
        """
        Count the number of events we haven't processed yet with dispatch or poll.
        """
        return xl_event_count_pending()

    @staticmethod
    def poll(bint wait=False):
        """
        Grab a single event from the pump, optionally blocking until a new event
        (for tools that want to save power by only updating on user interaction).
        """
        cdef Controller controller = Controller(reference=0)
        cdef Sound sound = Sound(reference=0)
        cdef Window window = Window(reference=0)
        cdef Animation animation = Animation(reference=0)

        cdef xl_event_type_t c_type
        cdef xl_event_t c_event

        xl_event_poll(&c_event, wait)
        c_type = c_event.type

        # NOTE: now we have to manually translate c structures to python tuples

        if c_type == XL_EVENT_NOTHING:
            return None

        elif c_type == XL_EVENT_QUIT:
            return ('quit',)

        elif c_type == XL_EVENT_WINDOW_CLOSE:
            window.window = c_event.as_window_close.window

            return ('window_close', window)

        elif c_type == XL_EVENT_WINDOW_MOVE:
            window.window = c_event.as_window_move.window

            return ('window_move', window,
                c_event.as_window_move.x, c_event.as_window_move.y)

        elif c_type == XL_EVENT_WINDOW_RESIZE:
            window.window = c_event.as_window_resize.window

            return ('window_resize', window,
                c_event.as_window_resize.width, c_event.as_window_resize.height)

        elif c_type == XL_EVENT_WINDOW_VISIBILITY_CHANGE:
            window.window = c_event.as_window_visibility_change.window

            return ('window_visibility_change', window,
                    bool(c_event.as_window_visibility_change.visible))

        elif c_type == XL_EVENT_WINDOW_REDRAW:
            window.window = c_event.as_window_redraw.window

            return ('window_redraw', window)

        elif c_type == XL_EVENT_WINDOW_GAIN_FOCUS:
            window.window = c_event.as_window_gain_focus.window

            return ('window_gain_focus', window)

        elif c_type == XL_EVENT_WINDOW_LOSE_FOCUS:
            window.window = c_event.as_window_lose_focus.window

            return ('window_lose_focus', window)

        elif c_type == XL_EVENT_WINDOW_MOUSE_ENTER:
            window.window = c_event.as_window_mouse_enter.window

            return ('window_mouse_enter', window)

        elif c_type == XL_EVENT_WINDOW_MOUSE_LEAVE:
            window.window = c_event.as_window_mouse_leave.window

            return ('window_mouse_leave', window)

        elif c_type == XL_EVENT_MUSIC_FINISHED:
            return ('music_finished',)

        elif c_type == XL_EVENT_SOUND_FINISHED:
            sound.sound = c_event.as_sound_finished.sound

            return ('sound_finished', sound)

        elif c_type == XL_EVENT_ANIMATION_FINISHED:
            animation.animation = c_event.as_animation_finished.animation

            return ('animation_finished', animation)

        elif c_type == XL_EVENT_CONTROLLER_INSERT:
            controller.controller = c_event.as_controller_insert.controller

            return ('controller_insert', controller)

        elif c_type == XL_EVENT_CONTROLLER_REMOVE:
            controller.controller = c_event.as_controller_remove.controller

            return ('controller_remove', controller)

        elif c_type == XL_EVENT_CONTROLLER_BUTTON:
            controller.controller = c_event.as_controller_button.controller

            return ('controller_button', controller,
                    xl_controller_button_short_name[<size_t>c_event.as_controller_button.button],
                    bool(c_event.as_controller_button.pressed))

        elif c_type == XL_EVENT_CONTROLLER_TRIGGER:
            controller.controller = c_event.as_controller_trigger.controller

            return ('controller_trigger', controller,
                chr(c_event.as_controller_trigger.which), c_event.as_controller_trigger.value)

        elif c_type == XL_EVENT_CONTROLLER_STICK:
            controller.controller = c_event.as_controller_stick.controller

            return ('controller_stick', controller,
                chr(c_event.as_controller_stick.which),

                    c_event.as_controller_stick.magnitude, c_event.as_controller_stick.angle,
                    c_event.as_controller_stick.x, c_event.as_controller_stick.y)

        assert 0, xl_event_type_name[<size_t>c_type]

    @staticmethod
    def dispatch(object handler, bint wait=False):
        """
        Same as poll, but passes the event to an optional object method.
        """
        cdef tuple py_event = event.poll(wait)

        if py_event is not None:
            attr = getattr(handler, 'on_' + py_event[0], None)

            # NOTE: we can't eat any exceptions that might happen here
            if attr is not None: attr(*py_event[1:])

        return py_event

    @staticmethod
    def dispatch_all(object handler):
        cdef tuple py_event = event.dispatch(handler)
        cdef list results = []

        while py_event:
            results.append(py_event)
            py_event = event.dispatch(handler)

        return results

    @staticmethod
    def poll_all(): return event.dispatch_all(None)

    class LoggingEventHandler(object):
        """
        Example event dispatch target for testing. Prints all events.
        """
        def on_quit(self): raise SystemExit

        def __getattr__(self, name):
            def log_event(*event):
                print name[3:] + (str(event) if len(event) != 1 else
                                        ('(' + str(event[0]) + ')'))
            return log_event

    log_handler = LoggingEventHandler()

    # NOTE: this hack is here to allow this to work with window.run()
    log_handler.has_exit = False
    log_handler.queued_state = None
    log_handler.activate = lambda window: None
    log_handler.deactivate = lambda window: None
    log_handler.update = lambda dt: None
    log_handler.draw = lambda window: None
