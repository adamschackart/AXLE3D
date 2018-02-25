/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- NOTE: all internal classes must remain POD (no virtual method definitions!)
--------------------------------------------------------------------------------
--- TODO: should we disable depth masking before drawing emitter/buffer normals?
--- TODO: debug setting for drawing all buffer & emitter aabboxes, normals, etc.
--- TODO: allow objects to be copyable and serializable (especially materials!)
--- TODO: when saving a material, store texture path with the base path removed
--- TODO: values aren't clamped when setting properties for lights or materials
--- TODO: test buffer against ellipsoid/line - check aabbox, then convex polys
--- TODO: "burn barrel" particle effect - sphere clamp x, aabbox clamp bottom y
--------------------------------------------------------------------------------

--- TODO: put all internal object structures into a union and block-allocate it.
- - - - - object allocation will have to check that memory blocks it gets back
 - - - -  actually reside in the global internal object array, as the open check
- - - - - must test for array residence with ptr_in_array before dereferencing.

            typedef struct gl_internal_object_t
            {
                gl_object_type_t type; // UNKNOWN indicates object is closed.

                // closed objects will also have NULL next and previous links.
                gl_internal_object_t* next;
                gl_internal_object_t* prev;

                union
                {
                    #define N(cap, low) struct gl_internal_ ## low ## _t low;
                    GL_OBJECT_TYPE_N
                    #undef N
                };
            } \
                gl_internal_object_t;

            // links not required, but it speeds up object iteration, as the
            // alternative is traversing the object array and type checking.
            // this also eliminates the need to sort in object list calls, as
            // all objects are already listed in the order they were created.

            static gl_internal_object_t *head, *tail;

----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

// Simplify things by just calling init at the beginning of any non-trivial func.
#define GL_PROFILE_SCOPE( ) AE_PROFILE_SCOPE( ); gl_init()
#define GL_PROFILE_NAMED(n) AE_PROFILE_NAMED(n); gl_init()

// This is just to detect leaks. Feel free to bump if you legitimately hit this.
#define GL_OBJECT_COUNT_LIMIT 1024

/* Global sets for each object type, so we can check objects for liveness, plus a
 * bunch of other neat stuff. Renamed due to gl_particle_emitter_set being taken.
 */
#define N(cap, low) static ae_ptrset_t gl_ ## low ## _lst;
GL_OBJECT_TYPE_N
#undef N

// TODO: use the system GL.h
#include <SDL2/SDL_opengl.h>

// CPP headers aren't included in ae_core.h due to long template compile times.
#include <new>

/*
================================================================================
 * ~~ [ object types ] ~~ *
--------------------------------------------------------------------------------
*/

gl_object_type_t gl_object_type(void* object)
{
    #define N(cap, low)                                             \
                                                                    \
        ae_if (gl_ ## low ## _get_open((gl_ ## low ## _t *)object)) \
        {                                                           \
            return GL_OBJECT_TYPE_ ## cap;                          \
        }                                                           \

    GL_OBJECT_TYPE_N
    #undef N

    return GL_OBJECT_TYPE_UNKNOWN;
}

size_t gl_object_count_all(void)
{
    return( // accumulate
            #define N(cap, low) gl_ ## low ## _count_all() +

            GL_OBJECT_TYPE_N
            #undef N
        0);
}

void gl_object_list_all(void** objects)
{
    #define N(cap, low)                                         \
                                                                \
        gl_ ## low ## _list_all((gl_ ## low ## _t **)objects);  \
        objects += gl_ ## low ## _count_all();                  \

    GL_OBJECT_TYPE_N
    #undef N
}

void gl_object_print_all(void)
{
    #define N(cap, low) gl_ ## low ## _print_all();
    GL_OBJECT_TYPE_N
    #undef N
}

void gl_object_close_all(void)
{
    #define N(cap, low) gl_ ## low ## _close_all();
    GL_OBJECT_TYPE_N
    #undef N
}

#define N(cap, low)                                             \
                                                                \
    static int gl_ ## low ## _compare(const void*,const void*); \
                                                                \
    void gl_ ## low ## _list_all(gl_ ## low ## _t ** pointers)  \
    {                                                           \
        ae_ptrset_list(&gl_ ## low ## _lst, (void **)pointers); \
                                                                \
        qsort(pointers, gl_ ## low ## _count_all(),             \
            sizeof(gl_ ## low ## _t*), gl_ ## low ## _compare); \
    }                                                           \

GL_OBJECT_TYPE_N
#undef N

#define N(cap, low)                                                             \
                                                                                \
    void gl_ ## low ## _print_all(void)                                         \
    {                                                                           \
        aeTempArray<gl_ ## low ## _t*> objects(gl_ ## low ## _count_all());     \
        gl_ ## low ## _list_all(objects.data());                                \
                                                                                \
        for (size_t i = 0, n = gl_ ## low ## _count_all(); i < n; i++)          \
        {                                                                       \
            printf("gl_%s(%s)\n", #low, gl_ ## low ## _get_status(objects[i])); \
        }                                                                       \
    }                                                                           \

GL_OBJECT_TYPE_N
#undef N

#define N(cap, low)                                                         \
                                                                            \
    void gl_ ## low ## _close_all(void)                                     \
    {                                                                       \
        aeTempArray<gl_ ## low ## _t*> objects(gl_ ## low ## _count_all()); \
        gl_ ## low ## _list_all(objects.data());                            \
                                                                            \
        for (size_t i = 0, n = gl_ ## low ## _count_all(); i < n; i++)      \
        {                                                                   \
            gl_ ## low ## _close(objects[i]);                               \
        }                                                                   \
    }                                                                       \

GL_OBJECT_TYPE_N
#undef N

/*
================================================================================
 * ~~ [ texture renderer ] ~~ *
--------------------------------------------------------------------------------
TODO: count bind calls for each texture, get and set global texture bind counter
TODO: framebuffer (render target / render to texture) support similar to xl_core
TODO: texture resizing, even if it means deleting textures and creating new ones
TODO: texture interpolation (possible to do this without a software image lerp?)
--------------------------------------------------------------------------------
*/

struct gl_internal_texture_t
{
    GLenum target;
    GLuint id;

    int width, height, comp;
    double time_created;

    const char* path;
    const char* name;

    static int default_copy;

    ae_image_t image;
    int copy_enabled;

    gl_texture_t* wrap()
    {
        return static_cast<gl_texture_t*>(this);
    }
};

// determines whether texture images are copied by default.
int gl_internal_texture_t::default_copy = 0;

// macro for easy dereferencing of internal object structs.
#define X(p) ((gl_internal_texture_t*)(p))

static int gl_texture_compare(const void* a, const void* b)
{
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_texture_set_int(gl_texture_t* texture, gl_texture_property_t property, int value)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_DEFAULT_COPY:
        {
            gl_internal_texture_t::default_copy = value;
        }
        break;

        case GL_TEXTURE_PROPERTY_COPY_ENABLED:
        {
            if (gl_texture_get_open(texture))
            {
                ae_if (value)
                {
                    // TODO: allocate image and copy pixel data from this texture -
                    // call gl_texture_get_image once that TODO is taken care of...
                }
                else
                {
                    ae_image_free(&X(texture)->image); // free copy mem on disable
                }

                X(texture)->copy_enabled = value;
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_OPEN:
        {
            if (ae_likely(value == 0 && gl_texture_get_open(texture)))
            {
                GL_PROFILE_NAMED(gl_texture_close);

                ae_string_free(const_cast<char*>(X(texture)->path));
                ae_string_free(const_cast<char*>(X(texture)->name));

                GL_DeleteTextures(1, &X(texture)->id);
                ae_image_free(&X(texture)->image);

                // call any internal C++ object destructors
                X(texture)->~gl_internal_texture_t();

                ae_ptrset_remove(&gl_texture_lst, texture);
                ae_free(texture);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_texture_get_int(gl_texture_t* texture, gl_texture_property_t property)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_TOTAL:
        {
            return gl_texture_lst.count;
        }
        break;

        case GL_TEXTURE_PROPERTY_ID:
        {
            if (gl_texture_get_open(texture))
            {
                return static_cast<int>(X(texture)->id);
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_TARGET:
        {
            // NOTE: fatal error here in non-OpenGL implementations of this
            if (gl_texture_get_open(texture))
            {
                return static_cast<int>(X(texture)->target);
            }
            else
            {
                return GL_TEXTURE_2D;
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_WIDTH:
        {
            if (gl_texture_get_open(texture)) return X(texture)->width;
        }
        break;

        case GL_TEXTURE_PROPERTY_HEIGHT:
        {
            if (gl_texture_get_open(texture)) return X(texture)->height;
        }
        break;

        case GL_TEXTURE_PROPERTY_COMP:
        {
            if (gl_texture_get_open(texture)) return X(texture)->comp;
        }
        break;

        case GL_TEXTURE_PROPERTY_DEFAULT_COPY:
        {
            return gl_internal_texture_t::default_copy;
        }
        break;

        case GL_TEXTURE_PROPERTY_COPY_ENABLED:
        {
            if (gl_texture_get_open(texture)) return X(texture)->copy_enabled;
        }
        break;

        case GL_TEXTURE_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_texture_lst, texture);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_texture_set_str(gl_texture_t* texture, gl_texture_property_t property, const char* value)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_PATH:
        {
            if (gl_texture_get_open(texture))
            {
                ae_string_free(const_cast<char*>(X(texture)->path));
                X(texture)->path = NULL;

                if (value != NULL)
                {
                    X(texture)->path = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_NAME:
        {
            if (gl_texture_get_open(texture))
            {
                ae_string_free(const_cast<char*>(X(texture)->name));
                X(texture)->name = NULL;

                if (value != NULL)
                {
                    X(texture)->name = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_texture_get_str(gl_texture_t* texture, gl_texture_property_t property)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_STATUS:
        {
            if (gl_texture_get_open(texture))
            {
                static char gl_texture_status[1024];
                const char* name = X(texture)->name;

                if (!name || !name[0]) name = "?";

                char pixel_format[8]; // HACK truncate
                strcpy(pixel_format, "RGBA");
                pixel_format[X(texture)->comp] = '\0';

                if (AE_SNPRINTF( gl_texture_status, "\"%s\", %ix%i %s", name,
                    X(texture)->width, X(texture)->height, pixel_format) < 0)
                {
                    AE_WARN("%u bytes is not enough for texture status!",
                                (unsigned int)sizeof(gl_texture_status));
                }

                return (const char*)gl_texture_status;
            }
            else
            {
                return "closed";
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_PATH:
        {
            if (gl_texture_get_open(texture) && X(texture)->path != NULL)
            {
                return X(texture)->path;
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_NAME:
        {
            if (gl_texture_get_open(texture) && X(texture)->name != NULL)
            {
                return X(texture)->name;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

void
gl_texture_set_img(gl_texture_t* texture, gl_texture_property_t property, ae_image_t* value)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_IMAGE:
        {
            /* TODO: modern opengl implementations seem to handle non-power-of-2
             * textures gracefully, but we should have an explicit path for them.
             */
            if (gl_texture_get_open(texture))
            {
                GL_PROFILE_NAMED(gl_texture_set_image);

                gl_internal_texture_t* data = X(texture);
                GL_BindTexture(data->target, data->id);

                /* TODO: does this belong here? make it configurable per texture
                 */
                GL_TexParameteri(data->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                GL_TexParameteri(data->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                ae_assert(value->width == (size_t)data->width, "%i, %i",
                                        (int)value->width, data->width);

                ae_assert(value->height == (size_t)data->height, "%i, %i",
                                        (int)value->height, data->height);

                ae_assert(value->type == AE_IMAGE_TYPE_U8, "%s!",
                                ae_image_type_name[value->type]);

                /* TODO: allow float textures, support for mono textures as well.
                 * we can convert in software, use a u8 internal format, or do
                 * full-blown floating-point texture support - whichever's best.
                 */
                GLenum gl_type = GL_UNSIGNED_BYTE;
                GLenum gl_format = GL_RGB;
                GLint gl_internal_format = GL_RGB;

                switch (data->comp)
                {
                    case 3: break;
                    case 4:
                    {
                        gl_internal_format = GL_RGBA;
                        gl_format = GL_RGBA;
                    }
                    break;

                    default: ae_assert(0, "%i", data->comp); break;
                }

                /* Flip the source image upside-down for OpenGL, then back again.
                 */
                ae_image_flip_y(value, NULL, 1, 1, 1, 1);

                GL_TexImage2D( data->target, 0, gl_internal_format, data->width,
                            data->height, 0, gl_format, gl_type, value->pixels);

                GL_Flush();
                ae_image_flip_y(value, NULL, 1, 1, 1, 1);

                /* Create an internal copy for later use, or clear the structure.
                 */
                if (data->copy_enabled)
                {
                    ae_image_copy(&data->image, value);
                }
                else
                {
                    ae_image_free(&data->image);
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }
}

ae_image_t*
gl_texture_get_img(gl_texture_t* texture, gl_texture_property_t property)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_IMAGE:
        {
            // TODO: call GL_CopyTexImage2D here (if software image copying is disabled)
            if (gl_texture_get_open(texture) && ae_image_bytes(&X(texture)->image) != 0)
            {
                return &X(texture)->image;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }

    return NULL;
}

gl_texture_t* gl_texture_create(int width, int height, int comp)
{
    GL_PROFILE_SCOPE();

    ae_assert(gl_texture_count_all() <= GL_OBJECT_COUNT_LIMIT,
                    "hit texture object count limit - leak?");

    gl_internal_texture_t * data = ae_create(gl_internal_texture_t, clear);
    gl_texture_t * texture = (gl_texture_t *)data;

    if (ae_ptrset_add(&gl_texture_lst, data) == 0)
    {
        AE_WARN("GL texture is not new to the set (is set code stubbed?)");
    }

    data->copy_enabled = gl_internal_texture_t::default_copy;
    data->time_created = ae_seconds();

    data->width = width;
    data->height = height;
    data->comp = comp;

    data->target = GL_TEXTURE_2D;
    GL_GenTextures(1, &data->id);

    // call constructor, which calls internal member object constructors
    new (data) gl_internal_texture_t;

    #if defined(AE_DEBUG)
    {
        GL_PROFILE_NAMED(gl_texture_create_debug);

        /* TODO: do we need to set the texture target to GL_RECTANGLE_2D?
         * TODO: make sure that texture dimensions don't exceed maximums
         */
        GL_BindTexture(data->target, data->id);

        /* TODO: does this belong here? make it configurable per texture
         */
        GL_TexParameteri(data->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GL_TexParameteri(data->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        /* GL may convert the data we pass in to a more efficient format,
         * which we can specify here. however, this can add a performance
         * penalty when loading, so we opt to keep both formats the same.
         */
        GLint gl_internal_format = 0;

        ae_image_format_t ae_format = AE_IMAGE_FORMAT_MONO;
        GLenum gl_format = 0;

        /* TODO: allow support for floating-point textures in this path
         */
        ae_image_type_t ae_type = AE_IMAGE_TYPE_U8;
        GLenum gl_type = GL_UNSIGNED_BYTE;

        /* TODO: allow support for mono textures, for depth buffers etc.
         */
        switch (comp)
        {
            case 3:
            {
                gl_internal_format = GL_RGB;
                gl_format = GL_RGB;
                ae_format = AE_IMAGE_FORMAT_RGB;
            }
            break;

            case 4:
            {
                gl_internal_format = GL_RGBA;
                gl_format = GL_RGBA;
                ae_format = AE_IMAGE_FORMAT_RGBA;
            }
            break;

            default: ae_assert(0, "%i", comp); break;
        }

        // allocate image & fill it in with some noticeable default color
        ae_image_t blank =
        {
            NULL, (size_t)width, (size_t)height, ae_format, ae_type, NULL,
        };

        // TODO: generate the default image (in debug mode) in this call
        ae_image_alloc(&blank);

        #if 0
            ae_image_set_color(&blank, NULL, ae_color_magenta, 1, 1, 1, 1);
        #else
            ae_image_randomize(&blank, NULL, 1, 1, 1, 1);
        #endif

        if (1) // draw outlines around textures to easily visualize bounds
        {
            const float color[4] =
            {
                ae_random_flt(), ae_random_flt(), ae_random_flt(), 1.0f,
            };

            ae_image_blit_rect_outline(&blank, NULL, (float*)color);
        }

        if (1) // "tag" images with unique colors in the top-left corner
        {
            const int tl_rect[4] = { 4, 4, 20, 20 };
            const float color[4] =
            {
                ae_random_flt(), ae_random_flt(), ae_random_flt(), 1.0f,
            };

            ae_image_set_color(&blank, (int*)tl_rect, color, 1, 1, 1, 1);
        }

        GL_TexImage2D(data->target, 0, gl_internal_format, width, height,
                                    0, gl_format, gl_type, blank.pixels);

        GL_Flush();
        ae_image_free(&blank);
    }
    #endif
    return texture;
}

gl_texture_t* gl_texture_load_from_memory(void* buf, size_t len)
{
    GL_PROFILE_SCOPE();

    ae_image_error_t error = AE_IMAGE_NO_CODEC;
    gl_texture_t * texture = gl_texture_load_from_memory_ex(buf, len, &error);

    ae_if (error != AE_IMAGE_SUCCESS)
    {
        ae_error("%s", ae_image_error_message(error, NULL));
    }

    return texture;
}

gl_texture_t* gl_texture_load_from_memory_ex(void* buf, size_t len,
                                    ae_image_error_t* error_status)
{
    GL_PROFILE_SCOPE(); // TODO: load compressed textures directly from DDS files

    ae_image_t image = AE_ZERO_STRUCT;
    gl_texture_t* texture = NULL;

    *error_status = ae_image_load_from_memory(&image, buf, len);

    ae_if (*error_status == AE_IMAGE_SUCCESS)
    {
        texture = gl_texture_create(image.width, image.height,
                    ae_image_format_components[image.format]);

        gl_texture_set_image(texture, &image);
        ae_image_free(&image);
    }

    return texture;
}

gl_texture_t* gl_texture_load(const char* filename)
{
    GL_PROFILE_SCOPE();

    ae_image_error_t error = AE_IMAGE_NO_CODEC; // for stub calls
    gl_texture_t * texture = gl_texture_load_ex(filename, &error);

    ae_if (error != AE_IMAGE_SUCCESS)
    {
        ae_error("%s", ae_image_error_message(error, filename));
    }

    return texture;
}

gl_texture_t*
gl_texture_load_ex(const char* filename, ae_image_error_t* error_status)
{
    GL_PROFILE_SCOPE(); // TODO: load compressed textures directly from DDS files

    ae_image_t image = AE_ZERO_STRUCT;
    gl_texture_t* texture = NULL;

    *error_status = ae_image_load(&image, filename);

    ae_if (*error_status == AE_IMAGE_SUCCESS)
    {
        texture = gl_texture_create(image.width, image.height,
                    ae_image_format_components[image.format]);

        gl_texture_set_image(texture, &image);
        ae_image_free(&image);

        char tex_name[1024 * 4]; // TODO: use "safer" sized string functions here
        ae_split_file_extension(ae_filename_from_path(filename), tex_name, NULL);

        gl_texture_set_path(texture, filename);
        gl_texture_set_name(texture, tex_name);
    }

    return texture;
}

void gl_texture_bind(gl_texture_t* texture)
{
    GL_PROFILE_SCOPE();

    ae_if (gl_texture_get_open(texture))
    {
        GL_Enable(X(texture)->target);
        GL_BindTexture(X(texture)->target, X(texture)->id);
    }
    else
    {
        GL_Disable(GL_TEXTURE_2D);
    }

    gl_debug_log_error_state();
}

void gl_texture_draw_ex(gl_texture_t* texture, float x, float y, const float rgba[4])
{
    /* TODO: define modulation color and mode texture properties like we have in XL,
     * as well as allowing for XL-style clipping and rotation via src and dst rects.
     */
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_texture_get_open(texture)))
    {
        GL_PushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
        {
            if (X(texture)->comp == 4 || ae_fabsf(1.0f - rgba[3]) > GL_DEFAULT_EPSILON)
            {
                GL_Enable(GL_BLEND); // TODO: premultiplied alpha?
                GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

            GL_Enable(X(texture)->target);
            GL_BindTexture(X(texture)->target, X(texture)->id);

            GL_Color4fv(rgba);

            GL_PushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
            {
                // convert the texture rectangle to a bounding box
                const float x1 = x, x2 = x + X(texture)->width;
                const float y1 = y, y2 = y + X(texture)->height;

                float tex[8] =
                {
                    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                };

                float array[32] =
                {
                    tex[0], tex[1], 0.0f, 1.0f, x1, y1, 0.0f, 1.0f,
                    tex[2], tex[3], 0.0f, 1.0f, x2, y1, 0.0f, 1.0f,
                    tex[4], tex[5], 0.0f, 1.0f, x2, y2, 0.0f, 1.0f,
                    tex[6], tex[7], 0.0f, 1.0f, x1, y2, 0.0f, 1.0f,
                };

                GL_InterleavedArrays(GL_T4F_V4F, 0, array);
                GL_DrawArrays(GL_QUADS, 0, 4);
            }
            GL_PopClientAttrib();
        }
        GL_PopAttrib();

        gl_debug_log_error_state();
    }
}

void gl_texture_draw(gl_texture_t* texture, float x, float y)
{
    gl_texture_draw_ex(texture, x, y, ae_color_white);
}

void gl_texture_draw_skybox_ex( gl_texture_t* front,
                                gl_texture_t* back,
                                gl_texture_t* left,
                                gl_texture_t* right,
                                gl_texture_t* up,
                                gl_texture_t* down,
                                gl_coord3D_t* viewer,
                                const float color[4])
{
    GL_PROFILE_SCOPE();

    GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    GL_Disable(GL_DEPTH_TEST);
    GL_Disable(GL_LIGHTING);

    GL_Enable(GL_BLEND); // TODO: premultiplied alpha?
    GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GL_Color4fv(color);

    GL_PushMatrix();
    GL_LoadIdentity();
    {
        const float heading = viewer->heading;
        const float pitch = viewer->pitch;

        GL_Rotatef(-pitch  , 1.0f, 0.0f, 0.0f);
        GL_Rotatef(-heading, 0.0f, 1.0f, 0.0f);

        const float coords[6][4][3] =
        {
            {{ 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1}, {-1, -1, -1}},
            {{-1, -1,  1}, {-1,  1,  1}, { 1,  1,  1}, { 1, -1,  1}},
            {{-1, -1, -1}, {-1,  1, -1}, {-1,  1,  1}, {-1, -1,  1}},
            {{ 1, -1,  1}, { 1,  1,  1}, { 1,  1, -1}, { 1, -1, -1}},
            {{ 1,  1,  1}, {-1,  1,  1}, {-1,  1, -1}, { 1,  1, -1}},
            {{-1, -1, -1}, {-1, -1,  1}, { 1, -1,  1}, { 1, -1, -1}},
        };

        const float texcoords[4][2] =
        {
            { 0, 0 },
            { 0, 1 },
            { 1, 1 },
            { 1, 0 },
        };

        gl_texture_t* textures[6] =
        {
            front, back, left, right, up, down,
        };

        for (size_t i = 0; i < 6; i++)
        {
            GLenum target = (GLenum)gl_texture_get_target(textures[i]);
            gl_texture_bind(textures[i]);

            GL_TexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            GL_TexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            GL_Begin(GL_QUADS);
            {
                GL_TexCoord2fv(texcoords[0]); GL_Vertex3fv(coords[i][0]);
                GL_TexCoord2fv(texcoords[1]); GL_Vertex3fv(coords[i][1]);
                GL_TexCoord2fv(texcoords[2]); GL_Vertex3fv(coords[i][2]);
                GL_TexCoord2fv(texcoords[3]); GL_Vertex3fv(coords[i][3]);
            }
            GL_End();
        }
    }
    GL_PopMatrix();
    GL_PopAttrib();
}

void gl_texture_draw_skybox(gl_texture_t* front,
                            gl_texture_t* back,
                            gl_texture_t* left,
                            gl_texture_t* right,
                            gl_texture_t* up,
                            gl_texture_t* down,
                            gl_coord3D_t* viewer)
{
    gl_texture_draw_skybox_ex(front, back, left, right,
                    up, down, viewer, ae_color_white);
}

#undef X

/*
================================================================================
 * ~~ [ material system ] ~~ *
--------------------------------------------------------------------------------
TODO: count apply calls per material, get and set global mtl application counter
--------------------------------------------------------------------------------
*/

struct gl_internal_material_t
{
    gl_texture_t* texture;
    int id, owns_texture;
    double time_created;

    float diffuse[4];
    float ambient[4];
    float specular[4];
    float emission[4];

    int light_enabled;
    u64 flags;

    float opacity;
    float shininess;

    const char* path;
    const char* name;

    gl_material_t* wrap()
    {
        return static_cast<gl_material_t*>(this);
    }
};

#define X(p) ((gl_internal_material_t*)(p))

static int gl_material_compare(const void* a, const void* b)
{
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_material_set_int(gl_material_t* material, gl_material_property_t property,
                    int value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_OWNS_TEXTURE:
        {
            if (gl_material_get_open(material)) X(material)->owns_texture = value;
        }
        break;

        case GL_MATERIAL_PROPERTY_LIGHT_ENABLED:
        {
            if (gl_material_get_open(material)) X(material)->light_enabled = value;
        }
        break;

        case GL_MATERIAL_PROPERTY_OPEN:
        {
            if (ae_likely(value == 0 && gl_material_get_open(material)))
            {
                GL_PROFILE_NAMED(gl_material_close);

                ae_if (X(material)->owns_texture)
                {
                    gl_texture_close(X(material)->texture);
                }

                ae_string_free(const_cast<char*>(X(material)->path));
                ae_string_free(const_cast<char*>(X(material)->name));

                // call any internal C++ object destructors
                X(material)->~gl_internal_material_t();

                ae_ptrset_remove(&gl_material_lst, material);
                ae_free(material);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_material_get_int(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_TOTAL:
        {
            return gl_material_lst.count;
        }
        break;

        case GL_MATERIAL_PROPERTY_ID:
        {
            if (gl_material_get_open(material)) return X(material)->id;
        }
        break;

        case GL_MATERIAL_PROPERTY_TEXTURE:
        {
            if (gl_material_get_open(material)) return gl_texture_get_id(X(material)->texture);
        }
        break;

        case GL_MATERIAL_PROPERTY_OWNS_TEXTURE:
        {
            if (gl_material_get_open(material)) return X(material)->owns_texture;
        }
        break;

        case GL_MATERIAL_PROPERTY_LIGHT_ENABLED:
        {
            if (gl_material_get_open(material)) return X(material)->light_enabled;
        }
        break;

        case GL_MATERIAL_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_material_lst, material);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_material_set_u64(gl_material_t* material, gl_material_property_t property,
                    u64 value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_FLAGS:
        {
            if (gl_material_get_open(material)) X(material)->flags = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

u64
gl_material_get_u64(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_FLAGS:
        {
            if (gl_material_get_open(material)) return X(material)->flags;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_material_set_flt(gl_material_t* material, gl_material_property_t property,
                    float value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_SHININESS:
        {
            if (gl_material_get_open(material)) X(material)->shininess = value;
        }
        break;

        case GL_MATERIAL_PROPERTY_OPACITY:
        {
            if (gl_material_get_open(material)) X(material)->opacity = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float
gl_material_get_flt(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_SHININESS:
        {
            if (gl_material_get_open(material)) return X(material)->shininess;
        }
        break;

        case GL_MATERIAL_PROPERTY_OPACITY:
        {
            if (gl_material_get_open(material)) return X(material)->opacity;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0.0f;
}

void
gl_material_set_vec(gl_material_t* material, gl_material_property_t property,
                    float* value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_DIFFUSE:
        {
            if (gl_material_get_open(material)) vec3copy(X(material)->diffuse, value);
        }
        break;

        case GL_MATERIAL_PROPERTY_AMBIENT:
        {
            if (gl_material_get_open(material)) vec3copy(X(material)->ambient, value);
        }
        break;

        case GL_MATERIAL_PROPERTY_SPECULAR:
        {
            if (gl_material_get_open(material)) vec3copy(X(material)->specular, value);
        }
        break;

        case GL_MATERIAL_PROPERTY_EMISSION:
        {
            if (gl_material_get_open(material)) vec3copy(X(material)->emission, value);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float*
gl_material_get_vec(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_DIFFUSE:
        {
            if (gl_material_get_open(material)) return X(material)->diffuse;
        }
        break;

        case GL_MATERIAL_PROPERTY_AMBIENT:
        {
            if (gl_material_get_open(material)) return X(material)->ambient;
        }
        break;

        case GL_MATERIAL_PROPERTY_SPECULAR:
        {
            if (gl_material_get_open(material)) return X(material)->specular;
        }
        break;

        case GL_MATERIAL_PROPERTY_EMISSION:
        {
            if (gl_material_get_open(material)) return X(material)->emission;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    static const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    return const_cast<float*>(zero);
}

void
gl_material_set_str(gl_material_t* material, gl_material_property_t property,
                    const char* value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_PATH:
        {
            if (gl_material_get_open(material))
            {
                ae_string_free(const_cast<char*>(X(material)->path));
                X(material)->path = NULL;

                if (value != NULL)
                {
                    X(material)->path = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        case GL_MATERIAL_PROPERTY_NAME:
        {
            if (gl_material_get_open(material))
            {
                ae_string_free(const_cast<char*>(X(material)->name));
                X(material)->name = NULL;

                if (value != NULL)
                {
                    X(material)->name = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_material_get_str(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_STATUS:
        {
            if (gl_material_get_open(material))
            {
                static char gl_material_status[1024];
                const char* name = X(material)->name;

                if (name && name[0])
                {
                    if (AE_SNPRINTF(gl_material_status, "\"%s\"", name) < 0)
                    {
                        AE_WARN("%u bytes is not enough for material status!",
                                    (unsigned int)sizeof(gl_material_status));
                    }

                    return (const char*)gl_material_status;
                }
                else
                {
                    return "open";
                }
            }
            else
            {
                return "closed";
            }
        }
        break;

        case GL_MATERIAL_PROPERTY_PATH:
        {
            if (gl_material_get_open(material) && X(material)->path != NULL)
            {
                return X(material)->path;
            }
        }
        break;

        case GL_MATERIAL_PROPERTY_NAME:
        {
            // TODO: if the material doesn't have a name, return the texture's
            if (gl_material_get_open(material) && X(material)->name != NULL)
            {
                return X(material)->name;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

void
gl_material_set_tex(gl_material_t* material, gl_material_property_t property,
                    gl_texture_t* value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_TEXTURE:
        {
            if (gl_material_get_open(material)) X(material)->texture = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

gl_texture_t*
gl_material_get_tex(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_TEXTURE:
        {
            if (gl_material_get_open(material)) return X(material)->texture;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return NULL;
}

gl_material_t* gl_material_create(void)
{
    // NOTE temp materials are created often, as we use them to push geometry
    // through the light pipeline without having to duplicate many GL calls.
    GL_PROFILE_SCOPE();

    ae_assert(gl_material_count_all() <= GL_OBJECT_COUNT_LIMIT,
                    "hit material object count limit - leak?");

    gl_internal_material_t* data = ae_create(gl_internal_material_t, clear);
    gl_material_t* material = (gl_material_t*)data;

    data->time_created = ae_seconds();
    data->id = (int)ae_random_u32();

    data->diffuse[0] = data->diffuse[1] = data->diffuse[2] = 0.8f;
    data->ambient[0] = data->ambient[1] = data->ambient[2] = 0.2f;

    data->specular[0] = data->specular[1] = data->specular[2] = 0.0f;
    data->emission[0] = data->emission[1] = data->emission[2] = 0.0f;

    data->light_enabled = 1;
    data->opacity = 1.0f;
    data->shininess = 0.0f;

    // call constructor, which calls member object constructors
    new (data) gl_internal_material_t;

    if (ae_ptrset_add(&gl_material_lst, data) == 0)
    {
        AE_WARN("GL material is not new to the set (is set code stubbed?)");
    }

    return material;
}

void gl_material_set_all(gl_material_t* material, const float* value)
{
    if (ae_likely(gl_material_get_open(material)))
    {
        vec3copy(X(material)->ambient, value);
        vec3copy(X(material)->diffuse, value);

        // vec3copy(X(material)->specular, value);
        // vec3copy(X(material)->emission, value);
    }
}

void gl_material_lerp(gl_material_t* dst, gl_material_t* a, gl_material_t* b, float t)
{
    GL_PROFILE_SCOPE();

    /* TODO: maybe consider doing a blanket memcpy of the entire A or B structure
     * into dst, and then interpolating the values we need to prevent bugs when
     * adding new material struct members and forgetting to set dst values here...
     */
    if (gl_material_get_open(a) &&
        gl_material_get_open(b) &&
        gl_material_get_open(dst))
    {
        #if 0
            ae_assert(X(a)->texture == X(b)->texture, "TODO: texture interpolation");
            X(dst)->texture = X(a)->texture;
        #else
            if (t < 0.5f)
            {
                X(dst)->texture = X(a)->texture;
            }
            else
            {
                X(dst)->texture = X(b)->texture;
            }
        #endif

        // XXX once texture interpolation is implemented, we might need to delete
        // and create a new dst texture in the event of a resize. when that path
        // is implemented, leave this value alone - dst will have its own texture.
        X(dst)->owns_texture = 0;

        ae_assert(X(a)->light_enabled == X(b)->light_enabled,
                    "A light enabled %i, B light enabled %i",
                    X(a)->light_enabled, X(b)->light_enabled);

        X(dst)->light_enabled = X(a)->light_enabled;

        #if 1
        {
            X(dst)->shininess = ae_lerpf(X(a)->shininess, X(b)->shininess, t);
            X(dst)->opacity   = ae_lerpf(X(a)->opacity  , X(b)->opacity  , t);

            vec4lerp(X(dst)->diffuse , X(a)->diffuse , X(b)->diffuse , t);
            vec4lerp(X(dst)->ambient , X(a)->ambient , X(b)->ambient , t);
            vec4lerp(X(dst)->specular, X(a)->specular, X(b)->specular, t);
            vec4lerp(X(dst)->emission, X(a)->emission, X(b)->emission, t);
        }
        #else
        {
            if (t < 0.5f)
            {
                X(dst)->shininess = X(a)->shininess;
                X(dst)->opacity = X(a)->opacity;

                vec4copy(X(dst)->diffuse , X(a)->diffuse );
                vec4copy(X(dst)->ambient , X(a)->ambient );
                vec4copy(X(dst)->specular, X(a)->specular);
                vec4copy(X(dst)->emission, X(a)->emission);
            }
            else
            {
                X(dst)->shininess = X(b)->shininess;
                X(dst)->opacity = X(b)->opacity;

                vec4copy(X(dst)->diffuse , X(b)->diffuse );
                vec4copy(X(dst)->ambient , X(b)->ambient );
                vec4copy(X(dst)->specular, X(b)->specular);
                vec4copy(X(dst)->emission, X(b)->emission);
            }
        }
        #endif
    }
}

void gl_material_apply_ex(gl_material_t* material, gl_buffer_t* buffer)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_material_get_open(material)))
    {
        gl_texture_bind(X(material)->texture); // can disable texturing

        if (X(material)->light_enabled)
        {
            GL_Enable(GL_LIGHTING);

            X(material)->diffuse [3] =
            X(material)->ambient [3] =
            X(material)->specular[3] =
            X(material)->emission[3] = X(material)->opacity;

            GL_Materialfv(GL_FRONT, GL_DIFFUSE , X(material)->diffuse );
            GL_Materialfv(GL_FRONT, GL_AMBIENT , X(material)->ambient );
            GL_Materialfv(GL_FRONT, GL_SPECULAR, X(material)->specular);
            GL_Materialfv(GL_FRONT, GL_EMISSION, X(material)->emission);

            GL_Materialf(GL_FRONT, GL_SHININESS, X(material)->shininess);
        }
        else
        {
            // TODO: push/pop instead of apply would be better for this
            GL_Disable(GL_LIGHTING);
        }

        // FIXME: when one translucent object is drawn, all other objects
        // become slightly translucent. so for now, everything is opaque.
        // TODO should we disable depth masking for translucent materials?

    #if 0
        if (ae_fabsf(X(material)->opacity - 1.0f) > 0.0001f ||
            gl_texture_get_comp(X(material)->texture) == 4)
        {
            // TODO: custom blending modes (use ae_image blitter enums
            // translated for glBlendFunc and glBlendEquationSeparate).

            GL_Enable(GL_BLEND); // default alpha blending mode
            GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            GL_Disable(GL_BLEND);
        }
    #endif

        ae_vertex_format_t vertex_format = static_cast<ae_vertex_format_t>( \
                                        gl_buffer_get_vertex_format(buffer));

        ae_if (ae_vertex_format_has_color(vertex_format))
        {
            // TODO: mode property (requires enum definition?)
            GL_ColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

            GL_Enable(GL_COLOR_MATERIAL);
        }
        else
        {
            GL_Disable(GL_COLOR_MATERIAL);
        }
    }

    gl_debug_log_error_state();
}

void gl_material_apply(gl_material_t* material)
{
    gl_material_apply_ex(material, NULL);
}

#undef X

/*
================================================================================
 * ~~ [ lighting system ] ~~ *
--------------------------------------------------------------------------------
*/

struct gl_internal_light_t
{
    double time_created;
    int id;

    const char* path;
    const char* name;

    float ambient[4];
    float diffuse[4];
    float specular[4];
    float position[4];
    float spot_direction[4];
    float spot_exponent;
    float spot_cutoff;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;

    gl_light_t* wrap()
    {
        return static_cast<gl_light_t*>(this);
    }
};

#define X(p) ((gl_internal_light_t*)(p))

static int gl_light_compare(const void* a, const void* b)
{
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_light_set_int(gl_light_t* light, gl_light_property_t property, int value)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_OPEN:
        {
            if (ae_likely(value == 0 && gl_light_get_open(light)))
            {
                GL_PROFILE_NAMED(gl_light_close);

                ae_string_free(const_cast<char*>(X(light)->path));
                ae_string_free(const_cast<char*>(X(light)->name));

                // call any internal C++ object destructors
                X(light)->~gl_internal_light_t();

                ae_ptrset_remove(&gl_light_lst, light);
                ae_free(light);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_light_get_int(gl_light_t* light, gl_light_property_t property)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_TOTAL:
        {
            return gl_light_lst.count;
        }
        break;

        case GL_LIGHT_PROPERTY_ID:
        {
            if (gl_light_get_open(light)) return X(light)->id;
        }
        break;

        case GL_LIGHT_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_light_lst, light);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_light_set_str(gl_light_t* light, gl_light_property_t property, const char* value)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_PATH:
        {
            if (gl_light_get_open(light))
            {
                ae_string_free(const_cast<char*>(X(light)->path));
                X(light)->path = NULL;

                if (value != NULL)
                {
                    X(light)->path = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        case GL_LIGHT_PROPERTY_NAME:
        {
            if (gl_light_get_open(light))
            {
                ae_string_free(const_cast<char*>(X(light)->name));
                X(light)->name = NULL;

                if (value != NULL)
                {
                    X(light)->name = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_light_get_str(gl_light_t* light, gl_light_property_t property)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_STATUS:
        {
            if (gl_light_get_open(light))
            {
                static char gl_light_status[1024];
                const char* name = X(light)->name;

                if (name && name[0])
                {
                    if (AE_SNPRINTF(gl_light_status, "\"%s\"", name) < 0)
                    {
                        AE_WARN("%u bytes is not enough for light status!",
                                    (unsigned int)sizeof(gl_light_status));
                    }

                    return (const char *)gl_light_status;
                }
                else
                {
                    return "open";
                }
            }
            else
            {
                return "closed";
            }
        }
        break;

        case GL_LIGHT_PROPERTY_PATH:
        {
            if (gl_light_get_open(light) && X(light)->path != NULL)
            {
                return X(light)->path;
            }
        }
        break;

        case GL_LIGHT_PROPERTY_NAME:
        {
            if (gl_light_get_open(light) && X(light)->name != NULL)
            {
                return X(light)->name;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

void
gl_light_set_vec(gl_light_t* light, gl_light_property_t property, float* value)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_AMBIENT:
        {
            if (gl_light_get_open(light)) vec4copy(X(light)->ambient, value);
        }
        break;

        case GL_LIGHT_PROPERTY_DIFFUSE:
        {
            if (gl_light_get_open(light)) vec4copy(X(light)->diffuse, value);
        }
        break;

        case GL_LIGHT_PROPERTY_SPECULAR:
        {
            if (gl_light_get_open(light)) vec4copy(X(light)->specular, value);
        }
        break;

        case GL_LIGHT_PROPERTY_POSITION:
        {
            if (gl_light_get_open(light)) vec4copy(X(light)->position, value);
        }
        break;

        case GL_LIGHT_PROPERTY_SPOT_DIRECTION:
        {
            if (gl_light_get_open(light)) vec3copy(X(light)->spot_direction, value);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float*
gl_light_get_vec(gl_light_t* light, gl_light_property_t property)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_AMBIENT:
        {
            if (gl_light_get_open(light)) return X(light)->ambient;
        }
        break;

        case GL_LIGHT_PROPERTY_DIFFUSE:
        {
            if (gl_light_get_open(light)) return X(light)->diffuse;
        }
        break;

        case GL_LIGHT_PROPERTY_SPECULAR:
        {
            if (gl_light_get_open(light)) return X(light)->specular;
        }
        break;

        case GL_LIGHT_PROPERTY_POSITION:
        {
            if (gl_light_get_open(light)) return X(light)->position;
        }
        break;

        case GL_LIGHT_PROPERTY_SPOT_DIRECTION:
        {
            if (gl_light_get_open(light)) return X(light)->spot_direction;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }

    static const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    return const_cast<float*>(zero);
}

void
gl_light_set_flt(gl_light_t* light, gl_light_property_t property, float value)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_SPOT_EXPONENT:
        {
            if (gl_light_get_open(light)) X(light)->spot_exponent = value;
        }
        break;

        case GL_LIGHT_PROPERTY_SPOT_CUTOFF:
        {
            if (gl_light_get_open(light)) X(light)->spot_cutoff = value;
        }
        break;

        case GL_LIGHT_PROPERTY_CONSTANT_ATTENUATION:
        {
            if (gl_light_get_open(light)) X(light)->constant_attenuation = value;
        }
        break;

        case GL_LIGHT_PROPERTY_LINEAR_ATTENUATION:
        {
            if (gl_light_get_open(light)) X(light)->linear_attenuation = value;
        }
        break;

        case GL_LIGHT_PROPERTY_QUADRATIC_ATTENUATION:
        {
            if (gl_light_get_open(light)) X(light)->quadratic_attenuation = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float
gl_light_get_flt(gl_light_t* light, gl_light_property_t property)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_SPOT_EXPONENT:
        {
            if (gl_light_get_open(light)) return X(light)->spot_exponent;
        }
        break;

        case GL_LIGHT_PROPERTY_SPOT_CUTOFF:
        {
            if (gl_light_get_open(light)) return X(light)->spot_cutoff;
        }
        break;

        case GL_LIGHT_PROPERTY_CONSTANT_ATTENUATION:
        {
            if (gl_light_get_open(light)) return X(light)->constant_attenuation;
        }
        break;

        case GL_LIGHT_PROPERTY_LINEAR_ATTENUATION:
        {
            if (gl_light_get_open(light)) return X(light)->linear_attenuation;
        }
        break;

        case GL_LIGHT_PROPERTY_QUADRATIC_ATTENUATION:
        {
            if (gl_light_get_open(light)) return X(light)->quadratic_attenuation;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0.0f;
}

gl_light_t* gl_light_create(void)
{
    GL_PROFILE_SCOPE();

    ae_assert(gl_light_count_all() <= 8, "hit light object count limit");

    gl_internal_light_t* data = ae_create(gl_internal_light_t, clear);
    gl_light_t* light = (gl_light_t*)data;

    data->time_created = ae_seconds();
    data->id = (int)ae_random_u32();

    #define ENTRY(name, r, g, b, a) do  \
    {                                   \
        float name[4] = { r, g, b, a }; \
        vec4copy(data->name, name);     \
    }                                   \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    ENTRY(ambient, 0, 0, 0, 1);
    ENTRY(diffuse, 0, 0, 0, 0);
    ENTRY(specular, 0, 0, 0, 0);
    ENTRY(position, 0, 0, 1, 0);
    ENTRY(spot_direction, 0, 0, -1, 0);

    #undef ENTRY

    data->spot_exponent = 0.0f;
    data->spot_cutoff = 180.0f;
    data->constant_attenuation = 1.0f;
    data->linear_attenuation = 0.0f;
    data->quadratic_attenuation = 0.0f;

    // call constructor, which calls member object constructors
    new (data) gl_internal_light_t;

    if (ae_ptrset_add(&gl_light_lst, data) == 0)
    {
        AE_WARN("GL light is not new to the set (is set code stubbed?)");
    }

    return light;
}

void gl_light_set_all(gl_light_t* light, const float* value)
{
    if (ae_likely(gl_light_get_open(light)))
    {
        vec4copy(X(light)->ambient, value);
        vec4copy(X(light)->diffuse, value);
    }
}

void gl_light_lerp(gl_light_t* dst, gl_light_t* a, gl_light_t* b, float t)
{
    GL_PROFILE_SCOPE();

    /* TODO: maybe consider doing a blanket memcpy of the entire A or B structure
     * into dst, and then interpolating the values we need to prevent bugs when
     * adding new light structure members and forgetting to set dst values here...
     */
    if (gl_light_get_open(a) &&
        gl_light_get_open(b) &&
        gl_light_get_open(dst))
    {
        #if 1
        {
            // TODO: property to select interpolation mode (lerp or oldschool)
            vec4lerp(X(dst)->spot_direction, X(a)->spot_direction,
                                        X(b)->spot_direction, t);

            vec4lerp(X(dst)->diffuse , X(a)->diffuse , X(b)->diffuse , t);
            vec4lerp(X(dst)->ambient , X(a)->ambient , X(b)->ambient , t);
            vec4lerp(X(dst)->specular, X(a)->specular, X(b)->specular, t);
            vec4lerp(X(dst)->position, X(a)->position, X(b)->position, t);

            X(dst)->spot_exponent = ae_lerpf(X(a)->spot_exponent,
                                        X(b)->spot_exponent, t);

            X(dst)->spot_cutoff = ae_lerpf(X(a)->spot_cutoff,
                                        X(b)->spot_cutoff, t);

            X(dst)->constant_attenuation = ae_lerpf(X(a)->constant_attenuation,
                                                X(b)->constant_attenuation, t);

            X(dst)->linear_attenuation = ae_lerpf(X(a)->linear_attenuation,
                                                X(b)->linear_attenuation, t);

            X(dst)->quadratic_attenuation = ae_lerpf(X(a)->quadratic_attenuation,
                                                X(b)->quadratic_attenuation, t);
        }
        #else
        {
            gl_internal_light_t * src = X(ae_branch(t < 0.5f) ? a : b);
            vec4copy(X(dst)->spot_direction, src->spot_direction);

            vec4copy(X(dst)->diffuse , src->diffuse );
            vec4copy(X(dst)->ambient , src->ambient );
            vec4copy(X(dst)->specular, src->specular);
            vec4copy(X(dst)->position, src->position);

            X(dst)->spot_exponent = src->spot_exponent;
            X(dst)->spot_cutoff = src->spot_cutoff;

            X(dst)->constant_attenuation = src->constant_attenuation;
            X(dst)->linear_attenuation = src->linear_attenuation;
            X(dst)->quadratic_attenuation = src->quadratic_attenuation;
        }
        #endif
    }
}

void gl_light_default_scene(void)
{
    GL_PROFILE_SCOPE();

    // reset scene before adding default lights
    gl_light_close_all();

    #define ENTRY(name, r, g, b, a) do  \
    {                                   \
        float name[4] = { r, g, b, a }; \
        vec4copy(data->name, name);     \
    }                                   \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    if (1) // one medium white light from above
    {
        gl_light_t* light = gl_light_create();
        gl_internal_light_t * data = X(light);

        gl_light_set_name(light, "above");

        ENTRY(position, 0.0f, 1.0f, 0.0f, 0.0f);
        ENTRY(ambient, 0.2f, 0.2f, 0.2f, 1.0f);
        ENTRY(diffuse, 0.5f, 0.5f, 0.5f, 1.0f);
        ENTRY(specular, 0.0f, 0.0f, 0.0f, 1.0f);
    }

    if (1) // one softer white light from below
    {
        gl_light_t* light = gl_light_create();
        gl_internal_light_t * data = X(light);

        gl_light_set_name(light, "below");

        ENTRY(position, 0.0f, -1.0f, 0.1f, 0.0f);
        ENTRY(ambient, 0.0f, 0.0f, 0.0f, 1.0f);
        ENTRY(diffuse, 0.2f, 0.2f, 0.2f, 1.0f);
        ENTRY(specular, 0.0f, 0.0f, 0.0f, 1.0f);
    }

    #undef ENTRY
}

void gl_light_apply_all(void)
{
    GL_PROFILE_SCOPE();

    gl_light_t* lights[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    gl_light_list_all(lights);

    for (size_t i = 0; i < 8; i++)
    {
        const GLenum gl_light = GL_LIGHT0 + i;
        gl_light_t* light = lights[i];

        if (gl_light_get_open(light))
        {
            GL_Enable(gl_light);

            GL_Lightfv(gl_light, GL_AMBIENT, X(light)->ambient);
            GL_Lightfv(gl_light, GL_DIFFUSE, X(light)->diffuse);
            GL_Lightfv(gl_light, GL_SPECULAR, X(light)->specular);
            GL_Lightfv(gl_light, GL_POSITION, X(light)->position);

            GL_Lightfv(gl_light, GL_SPOT_DIRECTION, X(light)->spot_direction);
            GL_Lightf(gl_light, GL_SPOT_EXPONENT, X(light)->spot_exponent);
            GL_Lightf(gl_light, GL_SPOT_CUTOFF, X(light)->spot_cutoff);

            GL_Lightf(gl_light, GL_CONSTANT_ATTENUATION, X(light)->constant_attenuation);
            GL_Lightf(gl_light, GL_LINEAR_ATTENUATION, X(light)->linear_attenuation);
            GL_Lightf(gl_light, GL_QUADRATIC_ATTENUATION, X(light)->quadratic_attenuation);
        }
        else
        {
            GL_Disable(gl_light);
        }
    }

    gl_debug_log_error_state();
}

#undef X

/*
================================================================================
 * ~~ [ vertex buffers ] ~~ *
--------------------------------------------------------------------------------
TODO: count draw calls for each buffer, multiply by verts for global tri counter
TODO: readonly lock and unlock; take CRC in debug mode to ensure no writes occur
TODO: (d/r)einterleave: split vertices into separate element channels and rejoin
TODO: gl_buffer_change_vertex_format: add or remove elements, new elems are zero
TODO: tessellate triangles above a given surface area threshold, for light & fog
TODO: auto linear mapping and spherical texture coordinate generation algorithms
--------------------------------------------------------------------------------
TODO: smoothing groups with thresholding for hard edges, perhaps using some edge
----- based data structure to mitigate problems with seams along UV boundaries!
--------------------------------------------------------------------------------
*/

struct gl_internal_buffer_t
{
    int locked, line_mode, id, compute_aabbox, smooth_shading;
    double time_created;

    const char* path;
    const char* name;

    ae_vertex_format_t vertex_format;
    ae_array_t vertices;

    ae_index_type_t index_type;
    ae_array_t indices;

    float minv[3];
    float maxv[3];

    // usually triangles, but we can use this to draw lines too
    GLenum draw_mode;

    gl_buffer_t* wrap()
    {
        return static_cast<gl_buffer_t*>(this);
    }
};

#define X(p) ((gl_internal_buffer_t*)(p))

static int gl_buffer_compare(const void* a, const void* b)
{
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_buffer_set_int(gl_buffer_t* buffer, gl_buffer_property_t property, int value)
{
    ae_switch (property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix)
    {
        case GL_BUFFER_PROPERTY_VERTEX_FORMAT:
        {
            if (gl_buffer_get_open(buffer))
            {
                X(buffer)->vertex_format = (ae_vertex_format_t)value;
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_TYPE:
        {
            if (gl_buffer_get_open(buffer))
            {
                X(buffer)->index_type = (ae_index_type_t)value;
            }
        }
        break;

        case GL_BUFFER_PROPERTY_STATIC_STORAGE:
        /*{
            gl_buffer_set_compute_aabbox(buffer, value); // TODO use GL VBOs here
        }
        break;
        */
        case GL_BUFFER_PROPERTY_COMPUTE_AABBOX:
        {
            if (gl_buffer_get_open(buffer) && value != X(buffer)->compute_aabbox)
            {
                X(buffer)->compute_aabbox = value;

                ae_if (value)
                {
                    ae_array_t v, i; // lock & unlock to recompute aabbox
                    gl_buffer_lock(buffer, &v, &i);

                    gl_buffer_unlock(buffer, &v, X(buffer)->vertex_format,
                        &i, X(buffer)->index_type);
                }
                else
                {
                    vec3zero(X(buffer)->minv);
                    vec3zero(X(buffer)->maxv);
                }
            }
        }
        break;

        case GL_BUFFER_PROPERTY_SMOOTH_SHADING:
        {
            if (gl_buffer_get_open(buffer)) X(buffer)->smooth_shading = value;
        }
        break;

        case GL_BUFFER_PROPERTY_LINE_MODE:
        {
            if (gl_buffer_get_open(buffer)) X(buffer)->line_mode = value;
        }
        break;

        case GL_BUFFER_PROPERTY_OPEN:
        {
            if (ae_likely(value == 0 && gl_buffer_get_open(buffer)))
            {
                GL_PROFILE_NAMED(gl_buffer_close);

                ae_assert(!X(buffer)->locked, "freed locked buffer");

                ae_string_free(const_cast<char*>(X(buffer)->path));
                ae_string_free(const_cast<char*>(X(buffer)->name));

                ae_array_free(&X(buffer)->vertices);
                ae_array_free(&X(buffer)->indices);

                // call any internal C++ object destructors
                X(buffer)->~gl_internal_buffer_t();

                ae_ptrset_remove(&gl_buffer_lst, buffer);
                ae_free(buffer);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_buffer_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_buffer_get_int(gl_buffer_t* buffer, gl_buffer_property_t property)
{
    ae_switch (property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix)
    {
        case GL_BUFFER_PROPERTY_TOTAL:
        {
            return gl_buffer_lst.count;
        }
        break;

        case GL_BUFFER_PROPERTY_ID:
        {
            if (gl_buffer_get_open(buffer)) return X(buffer)->id;
        }
        break;

        case GL_BUFFER_PROPERTY_VERTEX_FORMAT:
        {
            if (gl_buffer_get_open(buffer))
            {
                return static_cast<int>(X(buffer)->vertex_format);
            }
            else
            {
                return AE_V3F;
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_TYPE:
        {
            if (gl_buffer_get_open(buffer))
            {
                return static_cast<int>(X(buffer)->index_type);
            }
        }
        break;

        case GL_BUFFER_PROPERTY_VERTEX_COUNT:
        {
            if (gl_buffer_get_open(buffer))
            {
                return X(buffer)->vertices.size / (sizeof( float ) * \
                    ae_vertex_format_size[X(buffer)->vertex_format]);
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_COUNT:
        {
            if (gl_buffer_get_open(buffer))
            {
                return X(buffer)->indices.size / ae_index_type_bytes[ \
                                                X(buffer)->index_type];
            }
        }
        break;

        case GL_BUFFER_PROPERTY_TRIANGLE_COUNT:
        {
            if (gl_buffer_get_open(buffer))
            {
                int index_count = gl_buffer_get_index_count(buffer);
                if( index_count )
                {
                    return index_count / 3;
                }
                else
                {
                    return gl_buffer_get_vertex_count(buffer) / 3;
                }
            }
        }
        break;

        case GL_BUFFER_PROPERTY_STATIC_STORAGE:
        /*{
            return gl_buffer_get_compute_aabbox(buffer); // TODO use GL VBOs
        }
        break;
        */
        case GL_BUFFER_PROPERTY_COMPUTE_AABBOX:
        {
            if (gl_buffer_get_open(buffer)) return X(buffer)->compute_aabbox;
        }
        break;

        case GL_BUFFER_PROPERTY_LOCKED:
        {
            if (gl_buffer_get_open(buffer)) return X(buffer)->locked;
        }
        break;

        case GL_BUFFER_PROPERTY_SMOOTH_SHADING:
        {
            if (gl_buffer_get_open(buffer)) return X(buffer)->smooth_shading;
        }
        break;

        case GL_BUFFER_PROPERTY_LINE_MODE:
        {
            // XXX: should line and shading modes be material properties?
            if (gl_buffer_get_open(buffer)) return X(buffer)->line_mode;
        }
        break;

        case GL_BUFFER_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_buffer_lst, buffer);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_buffer_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_buffer_set_str(gl_buffer_t* buffer, gl_buffer_property_t property, const char* value)
{
    ae_switch (property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix)
    {
        case GL_BUFFER_PROPERTY_VERTEX_FORMAT:
        {
            if (gl_buffer_get_open(buffer))
            {
                // TODO: allow for partial valid names (see index setter)
                size_t i = 0, n = AE_VERTEX_FORMAT_COUNT;

                for (; i < n; i++)
                {
                    if (strcmp(ae_vertex_format_name[i] + 3, value) == 0)
                    {
                        X(buffer)->vertex_format = (ae_vertex_format_t)i;
                        return;
                    }
                }

                AE_WARN("invalid vertex format: %s", value);
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_TYPE:
        {
            if (gl_buffer_get_open(buffer))
            {
                if (strstr(value, "8")) // major hackitude!!!
                    X(buffer)->index_type = AE_INDEX_TYPE_U8;

                else if (strstr(value, "16"))
                    X(buffer)->index_type = AE_INDEX_TYPE_U16;

                else if (strstr(value, "32"))
                    X(buffer)->index_type = AE_INDEX_TYPE_U32;

                else
                    AE_WARN("invalid index type: %s", value);
            }
        }
        break;

        case GL_BUFFER_PROPERTY_PATH:
        {
            if (gl_buffer_get_open(buffer))
            {
                ae_string_free(const_cast<char*>(X(buffer)->path));
                X(buffer)->path = NULL;

                if (value != NULL)
                {
                    X(buffer)->path = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        case GL_BUFFER_PROPERTY_NAME:
        {
            if (gl_buffer_get_open(buffer))
            {
                ae_string_free(const_cast<char*>(X(buffer)->name));
                X(buffer)->name = NULL;

                if (value != NULL)
                {
                    X(buffer)->name = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_buffer_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_buffer_get_str(gl_buffer_t* buffer, gl_buffer_property_t property)
{
    ae_switch (property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix)
    {
        case GL_BUFFER_PROPERTY_VERTEX_FORMAT:
        {
            if (gl_buffer_get_open(buffer))
            {
                return ae_vertex_format_name[X(buffer)->vertex_format] + strlen("AE_");
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_TYPE:
        {
            if (gl_buffer_get_open(buffer))
            {
                return ae_index_type_name[X(buffer)->index_type] + strlen("AE_");
            }
        }
        break;

        case GL_BUFFER_PROPERTY_STATUS:
        {
            if (gl_buffer_get_open(buffer))
            {
                // TODO: shoehorn the buffer name in here somwhere (see texture)
                static char gl_buffer_status[1024];

                const char* vn = ae_vertex_format_name[X(buffer)->vertex_format];
                const char* in = ae_index_type_name[X(buffer)->index_type];

                u32 vc = X(buffer)->vertices.size / (sizeof(float) * \
                        ae_vertex_format_size[X(buffer)->vertex_format]);

                u32 ic = X(buffer)->indices.size / ae_index_type_bytes \
                                                [X(buffer)->index_type];

                if (AE_SNPRINTF(gl_buffer_status, "%u \"%s\" vtx, %u \"%s\" idx",
                            vc, vn + strlen("AE_"), ic, in + strlen("AE_")) < 0)
                {
                    AE_WARN("%u bytes is not enough for buffer status!",
                                (unsigned int)sizeof(gl_buffer_status));
                }

                return (const char*)gl_buffer_status;
            }
            else
            {
                return "closed";
            }
        }
        break;

        case GL_BUFFER_PROPERTY_PATH:
        {
            if (gl_buffer_get_open(buffer) && X(buffer)->path != NULL)
            {
                return X(buffer)->path;
            }
        }
        break;

        case GL_BUFFER_PROPERTY_NAME:
        {
            if (gl_buffer_get_open(buffer) && X(buffer)->name != NULL)
            {
                return X(buffer)->name;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_buffer_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

gl_buffer_t* gl_buffer_create_empty(void)
{
    // NOTE: temp buffers can be created often, as they're often used to
    // draw shapes like spheres etc. in conjunction with temp materials.
    GL_PROFILE_SCOPE();

    ae_assert(gl_buffer_count_all() <= GL_OBJECT_COUNT_LIMIT,
                    "hit buffer object count limit - leak?");

    gl_internal_buffer_t * data = ae_create(gl_internal_buffer_t, clear);
    gl_buffer_t * buffer = (gl_buffer_t *)data;

    data->time_created = ae_seconds();
    data->id = (int)ae_random_u32();

    // call constructor, which calls member object constructors
    new (data) gl_internal_buffer_t;

    if (ae_ptrset_add(&gl_buffer_lst, data) == 0)
    {
        AE_WARN("GL buffer is not new to the set (is set code stubbed?)");
    }

    // for internal line rendering stuff
    data->draw_mode = GL_TRIANGLES;

    data->smooth_shading = 1;

    data->vertex_format = AE_T2F_N3F_V3F;
    data->index_type = AE_INDEX_TYPE_U32;

    return buffer;
}

gl_buffer_t* gl_buffer_create_from(ae_array_t* vertices, ae_vertex_format_t vertex_format,
                                ae_array_t* indices, ae_index_type_t index_type, int copy)
{
    GL_PROFILE_SCOPE();

    gl_buffer_t* buffer = gl_buffer_create_empty();
    ae_array_t v, i;

    gl_buffer_lock(buffer, &v, &i);
    assert(v.data == NULL && i.data == NULL);

    ae_if (copy)
    {
        ae_array_copy(&v, vertices);
        ae_array_copy(&i, indices);

        gl_buffer_unlock(buffer, &v, vertex_format, &i, index_type);
    }
    else
    {
        gl_buffer_unlock(buffer, vertices, vertex_format, indices, index_type);
    }

    return buffer;
}

void gl_buffer_lock(gl_buffer_t* buffer, ae_array_t* vertices, ae_array_t* indices)
{
    // I feel like the standard graphics nomenclature is backwards here.
    // You should unlock objects to gain access to their internals, and
    // lock them to return to a valid state in which they can be drawn.
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        ae_assert(!X(buffer)->locked, "buffer is already locked!");

        assert(vertices); *vertices = X(buffer)->vertices;
        memset(&X(buffer)->vertices, 0, sizeof(ae_array_t));

        assert(indices); *indices = X(buffer)->indices;
        memset(&X(buffer)->indices, 0, sizeof(ae_array_t));

        X(buffer)->locked = 1;
    }
}

void gl_buffer_unlock(gl_buffer_t* buffer, ae_array_t* vertices, ae_vertex_format_t v_format,
                                            ae_array_t* indices, ae_index_type_t index_type)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        ae_assert(X(buffer)->locked, "buffer is already unlocked!");

        X(buffer)->vertex_format = v_format;
        X(buffer)->index_type = index_type;

        assert(vertices); X(buffer)->vertices = *vertices;
        memset(vertices, 0, sizeof(ae_array_t));

        assert(indices); X(buffer)->indices = *indices;
        memset(indices, 0, sizeof(ae_array_t));

        if (ae_likely(X(buffer)->compute_aabbox))
        {
            vtx_aabbox3_ex( X(buffer)->minv, X(buffer)->maxv,
                            (float*)X(buffer)->vertices.data,

                            X(buffer)->vertices.size / sizeof( float ),
                            ae_vertex_format_position_offset[v_format],

                            ae_vertex_format_size[v_format]);
        }
        else
        {
            vec3zero(X(buffer)->minv);
            vec3zero(X(buffer)->maxv);
        }

        X(buffer)->locked = 0;
    }
}

void gl_buffer_get_vertex(gl_buffer_t* buffer, size_t which,
                    float* t, float* c, float* n, float* p)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        #define GL_BUFFER_TOTAL_VERTEX_FORMAT_INFO()                                    \
                                                                                        \
            /* can't use the X "deref" macro here because it's used by emitters */      \
            gl_internal_buffer_t * buffer_data = (gl_internal_buffer_t *)buffer;        \
                                                                                        \
            const ae_vertex_format_t vertex_format = buffer_data->vertex_format;        \
            const size_t vertex_size = ae_vertex_format_size[vertex_format];            \
                                                                                        \
            const size_t float_count = buffer_data->vertices.size / sizeof(float);      \
            const size_t vertex_count = float_count / vertex_size;                      \
                                                                                        \
            const size_t tex_offset = ae_vertex_format_texcoord_offset [vertex_format]; \
            const size_t col_offset = ae_vertex_format_color_offset    [vertex_format]; \
            const size_t nrm_offset = ae_vertex_format_normal_offset   [vertex_format]; \
            const size_t pos_offset = ae_vertex_format_position_offset [vertex_format]; \
                                                                                        \
            const size_t tex_size = ae_vertex_format_texcoord_size [vertex_format];     \
            const size_t col_size = ae_vertex_format_color_size    [vertex_format];     \
            const size_t nrm_size = ae_vertex_format_normal_size   [vertex_format];     \
            const size_t pos_size = ae_vertex_format_position_size [vertex_format];     \

        GL_BUFFER_TOTAL_VERTEX_FORMAT_INFO();
        ae_assert(which < vertex_count, "%u >= %u", (u32)which, (u32)vertex_count);

        const float* const vertex = &((const float* const)
                                    X(buffer)->vertices.data)[which * vertex_size];

        if (t) vec_copy(t, vertex + tex_offset, tex_size);
        if (c) vec_copy(c, vertex + col_offset, col_size);
        if (n) vec_copy(n, vertex + nrm_offset, nrm_size);
        if (p) vec_copy(p, vertex + pos_offset, pos_size);
    }
}

void gl_buffer_set_vertex(gl_buffer_t* buffer, size_t which,
                    float* t, float* c, float* n, float* p)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_BUFFER_TOTAL_VERTEX_FORMAT_INFO();
        assert(!X(buffer)->locked);

        ae_assert(which < vertex_count, "%u >= %u", (u32)which, (u32)vertex_count);
        float * vertex = ((float *)X(buffer)->vertices.data) + which * vertex_size;

        if (t) vec_copy(vertex + tex_offset, t, tex_size);
        if (c) vec_copy(vertex + col_offset, c, col_size);
        if (n) vec_copy(vertex + nrm_offset, n, nrm_size);
        if (p)
        {
            vec_copy(vertex + pos_offset, p, pos_size);

            #define GL_BUFFER_RECOMPUTE_AABBOX_FROM_SINGLE_VERTEX(data) \
                                                                        \
                if (ae_likely((data)->compute_aabbox))                  \
                {                                                       \
                    if (ae_likely((data)->vertices.size != 0))          \
                    {                                                   \
                        vec3min_vec((data)->minv, (data)->minv, p);     \
                        vec3max_vec((data)->maxv, (data)->maxv, p);     \
                    }                                                   \
                    else                                                \
                    {                                                   \
                        vec3copy((data)->minv, p);                      \
                        vec3copy((data)->maxv, p);                      \
                    }                                                   \
                }                                                       \
                else                                                    \
                {                                                       \
                    vec3zero((data)->minv);                             \
                    vec3zero((data)->maxv);                             \
                }                                                       \

            GL_BUFFER_RECOMPUTE_AABBOX_FROM_SINGLE_VERTEX(buffer_data);
        }
    }
}

void gl_buffer_append_vertex(gl_buffer_t* buffer, float* t, float* c, float* n, float* p)
{
    /* NOTE: in a VBO environment, this could buffer soft verts and inds until the
     * VBO is actually required for drawing, at which point the data is committed.
     */
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked); // buffer isn't in "edit" mode

        ae_vertex_format_t f = X(buffer)->vertex_format;
        assert(ae_vertex_format_position_size[f] == 3);

        ae_assert(ae_vertex_format_has_texcoord(f) ? t != NULL : 1,
                                    "NULL vertex texcoord pointer");

        ae_assert(ae_vertex_format_has_color(f) ? c != NULL : 1,
                                    "NULL vertex color pointer");

        ae_assert(ae_vertex_format_has_normal(f) ? n != NULL : 1,
                                    "NULL vertex normal pointer");

        ae_assert(ae_vertex_format_has_position(f) ? p != NULL : 1,
                                    "NULL vertex position pointer");

        GL_BUFFER_RECOMPUTE_AABBOX_FROM_SINGLE_VERTEX(X(buffer));

        ae_array_reserve(&X(buffer)->vertices, sizeof(float) *
                                    ae_vertex_format_size[f]);

        ae_array_append(&X(buffer)->vertices, t, sizeof(float) *
                            ae_vertex_format_texcoord_size[f]);

        ae_array_append(&X(buffer)->vertices, c, sizeof(float) *
                                ae_vertex_format_color_size[f]);

        ae_array_append(&X(buffer)->vertices, n, sizeof(float) *
                                ae_vertex_format_normal_size[f]);

        ae_array_append(&X(buffer)->vertices, p, sizeof(float) *
                            ae_vertex_format_position_size[f]);
    }
}

void gl_buffer_get_face(gl_buffer_t * buffer, size_t which,
                        size_t *v0, size_t *v1, size_t *v2)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        const // XXX: do we need this in release mode if we're just bounds checking?
        size_t index_count = static_cast<size_t>(gl_buffer_get_index_count(buffer));

        // TODO: (ae_)switch on the index type just once instead of three times here
        const size_t tri_index = which * 3;
        const ae_index_type_t index_type = X(buffer)->index_type;

        void* indices = X(buffer)->indices.data;

        if (v0) *v0 = idx_value_get(indices, index_type, index_count, tri_index + 0);
        if (v1) *v1 = idx_value_get(indices, index_type, index_count, tri_index + 1);
        if (v2) *v2 = idx_value_get(indices, index_type, index_count, tri_index + 2);
    }
}

void gl_buffer_set_face(gl_buffer_t * buffer, size_t which,
                        size_t v0, size_t v1, size_t v2)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        const // XXX: do we need this in release mode if we're just bounds checking?
        size_t index_count = static_cast<size_t>(gl_buffer_get_index_count(buffer));

        // TODO: (ae_)switch on the index type just once instead of three times here
        const size_t tri_index = which * 3;
        const ae_index_type_t index_type = X(buffer)->index_type;

        void* indices = X(buffer)->indices.data;

        idx_value_set(indices, index_type, index_count, tri_index + 0, v0);
        idx_value_set(indices, index_type, index_count, tri_index + 1, v1);
        idx_value_set(indices, index_type, index_count, tri_index + 2, v2);
    }
}

void gl_buffer_append_face(gl_buffer_t* buffer, size_t v0, size_t v1, size_t v2)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        /* TODO: if any value exceeds possible type size, expand the index buffer
         */
        ae_index_type_t type = (X(buffer)->index_type);
        ae_array_t * indices = &X(buffer)->indices;

        /* alloc tri bytes
         */
        ae_array_reserve(indices, indices->size + 3 * ae_index_type_bytes[type]);

        /* TODO: switch once instead of three times here
         */
        idx_value_append(indices, type, v0);
        idx_value_append(indices, type, v1);
        idx_value_append(indices, type, v2);
    }
}

// TODO: implement the functional opposite of this (gl_buffer_slice), which
// would create a copy of a buffer's vertices and inds within a given range.

void gl_buffer_merge(gl_buffer_t* dst, gl_buffer_t* src, int copy)
{
    GL_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(src) && gl_buffer_get_open(dst))
    {
        #define GL_BUFFER_BINARY_VERTEX_FORMAT_INFO(src, dst)                   \
                                                                                \
            ae_array_t* src_vertices = &X(src)->vertices;                       \
            ae_array_t* dst_vertices = &X(dst)->vertices;                       \
                                                                                \
            ae_vertex_format_t src_vertex_format = X(src)->vertex_format;       \
            ae_vertex_format_t dst_vertex_format = X(dst)->vertex_format;       \
                                                                                \
            size_t src_vertex_size = ae_vertex_format_size[src_vertex_format];  \
            size_t dst_vertex_size = ae_vertex_format_size[dst_vertex_format];  \
                                                                                \
            size_t src_vertex_count = X(src)->vertices.size / (                 \
                                sizeof(float) * src_vertex_size);               \
                                                                                \
            size_t dst_vertex_count = X(dst)->vertices.size / (                 \
                                sizeof(float) * dst_vertex_size);               \
                                                                                \
            ae_array_t* src_indices = &X(src)->indices;                         \
            ae_array_t* dst_indices = &X(dst)->indices;                         \
                                                                                \
            ae_index_type_t src_index_type = X(src)->index_type;                \
            ae_index_type_t dst_index_type = X(dst)->index_type;                \
                                                                                \
            size_t src_index_type_bytes = ae_index_type_bytes[src_index_type];  \
            size_t dst_index_type_bytes = ae_index_type_bytes[dst_index_type];  \
                                                                                \
            size_t src_index_count = X(src)->indices.size /                     \
                                    src_index_type_bytes;                       \
                                                                                \
            size_t dst_index_count = X(dst)->indices.size /                     \
                                    dst_index_type_bytes;                       \

        GL_BUFFER_BINARY_VERTEX_FORMAT_INFO(src, dst);

        ae_array_reserve(dst_vertices, dst_vertices->size + // reserve vertices
                            dst_vertex_size * src_vertex_count * sizeof(float));

        ae_if (src_index_count && !dst_index_count)
        {
            dst_index_type = X(dst)->index_type = src_index_type;
            dst_index_type_bytes = src_index_type_bytes;
            dst_index_count = dst_vertex_count;

            ae_array_resize(dst_indices, dst_index_type_bytes * dst_index_count);
            idx_identity(dst_indices->data, dst_index_type, dst_index_count);
        }

        // TODO: ae_array_reserve(dst_indices, dst_indices->size + ?);
        // TODO: fast path if two u16 buffers will still fit into u16

        for (size_t i = 0, src_tris = src_index_count / 3; i < src_tris; i++)
        {
            size_t v0, v1, v2;
            gl_buffer_get_face(src, i, &v0, &v1, &v2);

            v0 += dst_vertex_count;
            v1 += dst_vertex_count;
            v2 += dst_vertex_count;

            gl_buffer_append_face(dst, v0, v1, v2);
        }

        ae_if (dst_index_count && !src_index_count)
        {
            ae_assert(0, "TODO append offset identity indices to dst buffer");
        }
    /*
        ae_if (src_vertex_count && !dst_vertex_count)
        {
            dst_vertex_size = src_vertex_size; // set empty dst buffer format
            dst_vertex_format = X(dst)->vertex_format = src_vertex_format;
        }
    */
        ae_if (src_vertex_format == dst_vertex_format)
        {
            ae_array_append(dst_vertices, src_vertices->data, src_vertices->size);

            if (X(dst)->compute_aabbox)
            {
                if (X(src)->compute_aabbox)
                {
                    vec3min_vec(X(dst)->minv, X(dst)->minv, X(src)->minv);
                    vec3max_vec(X(dst)->maxv, X(dst)->maxv, X(src)->maxv);
                }
                else
                {
                    ae_array_t temp_v, temp_i; // recompute by unlocking
                    gl_buffer_lock(dst, &temp_v, &temp_i);

                    gl_buffer_unlock(dst, &temp_v, X(dst)->vertex_format,
                                            &temp_i, X(dst)->index_type);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < src_vertex_count; i++)
            {
                float tex[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                float col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                float nrm[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                float pos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

                gl_buffer_get_vertex(src, i, tex, col, nrm, pos);
                gl_buffer_append_vertex(dst, tex, col, nrm, pos);
            }
        }

        ae_if (!copy)
        {
            gl_buffer_close(src);
        }

        ae_array_trim(&X(dst)->vertices);
        ae_array_trim(&X(dst)->indices);
    }
}

void gl_buffer_get_aabbox(gl_buffer_t* buffer, float* minv, float* maxv)
{
    #if defined(AE_DEBUG)
        GL_PROFILE_NAMED(gl_buffer_get_aabbox_debug);
    #else
        GL_PROFILE_SCOPE();
    #endif

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        #if defined(AE_DEBUG)
        {
            ae_vertex_format_t vertex_format = X(buffer)->vertex_format;

            float r_min[3];
            float r_max[3];

            vtx_aabbox3_ex( r_min, r_max, (float *)X(buffer)->vertices.data,
                            X(buffer)->vertices.size / sizeof(float),

                            ae_vertex_format_position_offset[vertex_format],
                            ae_vertex_format_size[vertex_format]);

            if (vec3cmp_e(r_min, X(buffer)->minv, 0.00001f)  &&
                vec3cmp_e(r_max, X(buffer)->maxv, 0.00001f)) {} else
            {
                if (X(buffer)->compute_aabbox)
                {
                    ae_assert(0, "vtx_aabbox3_ex computed incorrectly");
                }
                else
                {
                    AE_WARN("queried for buffer aabbox (not computed)");
                }
            }
        }
        #endif

        vec3copy(minv, X(buffer)->minv);
        vec3copy(maxv, X(buffer)->maxv);
    }
    else
    {
        AE_WARN("got an empty bounding box from a closed vertex buffer");

        vec3zero(minv);
        vec3zero(maxv);
    }
}

void gl_buffer_translate(gl_buffer_t* buffer, const float v[3])
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        #define GL_POS_TRANSFORM_FUNCTION_SETUP()                                   \
                                                                                    \
            ae_vertex_format_t vertex_format = X(buffer)->vertex_format;            \
            assert(!X(buffer)->locked);                                             \
            assert(ae_vertex_format_position_size[vertex_format] == 3);             \
                                                                                    \
            const size_t size = ae_vertex_format_size[vertex_format]; /* stride */  \
            const size_t vpos = ae_vertex_format_position_offset[vertex_format];    \
                                                                                    \
            ae_array_t* array = &X(buffer)->vertices;                               \

        GL_POS_TRANSFORM_FUNCTION_SETUP();

        if (size == 3)
        {
            // legacy holdover from when this was the only possible SSE vtx path
            vtx_vec3add((float *)(array->data), v, array->size / sizeof(float));
        }
        else
        {
            vtx_vec3add_ex((float *)array->data, v, array->size / sizeof(float),
                                                                    vpos, size);
        }

        ae_if (X(buffer)->compute_aabbox)
        {
            vec3add_vec(X(buffer)->minv, X(buffer)->minv, v);
            vec3add_vec(X(buffer)->maxv, X(buffer)->maxv, v);
        }
    }
}

void gl_buffer_scale(gl_buffer_t* buffer, const float v[3])
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_POS_TRANSFORM_FUNCTION_SETUP();

        if (size == 3)
        {
            // legacy holdover from when this was the only possible SSE vtx path
            vtx_vec3mul((float *)(array->data), v, array->size / sizeof(float));
        }
        else
        {
            vtx_vec3mul_ex((float *)array->data, v, array->size / sizeof(float),
                                                                    vpos, size);
        }

        ae_if (X(buffer)->compute_aabbox)
        {
            vec3mul_vec(X(buffer)->minv, X(buffer)->minv, v);
            vec3mul_vec(X(buffer)->maxv, X(buffer)->maxv, v);
        }
    }
}

void gl_buffer_lerp(gl_buffer_t* dst, gl_buffer_t* a, gl_buffer_t* b, float t)
{
    GL_PROFILE_SCOPE();

    if (gl_buffer_get_open(a) &&
        gl_buffer_get_open(b) &&
        gl_buffer_get_open(dst))
    {
        assert(!X(dst)->locked);
        assert(!X( a )->locked);
        assert(!X( b )->locked);

        ae_assert(  X(a)->vertex_format == X(b)->vertex_format,
                    "%s != %s",
                    ae_vertex_format_name[X(a)->vertex_format],
                    ae_vertex_format_name[X(b)->vertex_format]);

        ae_assert(  X(a)->vertices.size == X(b)->vertices.size,
                    "%i != %i",
                    gl_buffer_get_vertex_count(a),
                    gl_buffer_get_vertex_count(b));

        ae_assert(  X(a)->index_type == X(b)->index_type,
                    "%s != %s",
                    ae_index_type_name[X(a)->index_type],
                    ae_index_type_name[X(b)->index_type]);

        ae_assert(  X(a)->indices.size == X(b)->indices.size,
                    "%i != %i",
                    gl_buffer_get_index_count(a),
                    gl_buffer_get_index_count(b));

        ae_array_t dst_vertices;
        ae_array_t dst_indices;
        gl_buffer_lock(dst, &dst_vertices, &dst_indices);

        #if 1
        {
            ae_array_resize(&dst_vertices, X(a)->vertices.size);

            // interpolate all components, not just position
            vtx_lerp((float*)dst_vertices.data,

                    (const float* const)X(a)->vertices.data,
                    (const float* const)X(b)->vertices.data,

                    t, dst_vertices.size / sizeof( float ));
        }
        #else
        {
            if (t < 0.5f)
            {
                ae_array_copy(&dst_vertices, &X(a)->vertices);
            }
            else
            {
                ae_array_copy(&dst_vertices, &X(b)->vertices);
            }
        }
        #endif

        ae_assert(ae_array_cmp(&X(a)->indices, &X(b)->indices) == 1,
            "a and b index buffers do not have the same contents!");

        ae_array_copy(&dst_indices, &X(a)->indices);

        gl_buffer_unlock( dst, &dst_vertices, X(a)->vertex_format,
                                &dst_indices, X(a)->index_type );
    }
}

void gl_buffer_transform(gl_buffer_t* buffer, const float m[16], int normalize)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_POS_TRANSFORM_FUNCTION_SETUP(); // setup locals

        ae_array_t lock_v, lock_i; // recompute aabbox
        gl_buffer_lock(buffer, &lock_v, &lock_i);

        // TODO: transform vertex normals and positions in one pass for better cache use
        ae_if (ae_vertex_format_has_normal(vertex_format))
        {
            assert(ae_vertex_format_normal_size[vertex_format] == 3);

            vtx_mat4x4row_mul_vector3_ex((float *)lock_v.data, m, normalize, lock_v.size \
                    / sizeof(float), ae_vertex_format_normal_offset[vertex_format], size);
        }

        vtx_mat4x4row_mul_point3_ex((float*)lock_v.data,
            m, lock_v.size / sizeof(float), vpos, size);

        /* TODO: function to transform aabbox by matrix
         */
        gl_buffer_unlock(buffer, &lock_v, vertex_format,
                        &lock_i, X(buffer)->index_type);
    }
}

void gl_buffer_center_at_origin(gl_buffer_t* buffer)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        float min_v[3], max_v[3], center[3];

        if (X(buffer)->compute_aabbox)
        {
            gl_buffer_get_aabbox(buffer, min_v, max_v);
        }
        else
        {
            ae_vertex_format_t vertex_format = X(buffer)->vertex_format;

            vtx_aabbox3_ex( min_v, max_v, (float *)X(buffer)->vertices.data,
                            X(buffer)->vertices.size / sizeof(float),

                            ae_vertex_format_position_offset[vertex_format],
                            ae_vertex_format_size[vertex_format]);
        }

        aabbox3_get_center(center, min_v, max_v);
        vec3neg(center, center);
        gl_buffer_translate(buffer, center);
    }
}

float gl_buffer_unique_vertex_ratio(gl_buffer_t* buffer, float epsilon)
{
    GL_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(buffer) && X(buffer)->vertices.size)
    {
        assert(!X(buffer)->locked);

        /* TODO push this down into aecore, use stack
         */
        ae_array_t temp_v = AE_ZERO_STRUCT;
        ae_array_t temp_i = AE_ZERO_STRUCT;

        ae_array_copy(&temp_v, &X(buffer)->vertices);

        vtx_weld( &temp_v, X(buffer)->vertex_format,
            &temp_i, X(buffer)->index_type, epsilon);

        float ratio = (float)temp_v.size / (float)
                            X(buffer)->vertices.size;

        ae_array_free(&temp_v);
        ae_array_free(&temp_i);

        return ratio;
    }

    return 1.0f;
}

void gl_buffer_weld(gl_buffer_t* buffer, float epsilon)
{
    GL_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(buffer) && X(buffer)->indices.size == 0)
    {
        assert(!X(buffer)->locked);

        vtx_weld( &X(buffer)->vertices, X(buffer)->vertex_format,
            &X(buffer)->indices, X(buffer)->index_type, epsilon);

        // XXX: if a massive epsilon value is passed, vertices can be
        // destroyed, causing the bounding box to possibly shrink.
        // due to the rarity of this, we don't recompute in release.
        #if defined(AE_DEBUG)
        {
            if (X(buffer)->compute_aabbox)
            {
                float minv[3], maxv[3]; // checks aabbox
                gl_buffer_get_aabbox(buffer, minv, maxv);
            }
        }
        #endif
    }
}

void gl_buffer_weld_dist2(gl_buffer_t* buffer, float threshold_sq)
{
    GL_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(buffer) && X(buffer)->indices.size == 0)
    {
        assert(!X(buffer)->locked);

        vtx_weld_dist2(&X(buffer)->vertices, X(buffer)->vertex_format,
            &X(buffer)->indices, X(buffer)->index_type, threshold_sq);

        #if defined(AE_DEBUG)
        {
            if (X(buffer)->compute_aabbox)
            {
                float minv[3], maxv[3]; // checks aabbox
                gl_buffer_get_aabbox(buffer, minv, maxv);
            }
        }
        #endif
    }
}

void gl_buffer_unweld(gl_buffer_t* buffer)
{
    GL_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(buffer) && X(buffer)->indices.size != 0)
    {
        assert(!X(buffer)->locked);

        vtx_unweld(&X(buffer)->vertices, X(buffer)->vertex_format,
                    &X(buffer)->indices, X(buffer)->index_type );
    }
}

void gl_buffer_shrink_indices(gl_buffer_t* buffer)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked); // optimize index buffer without re-ordering

        size_t vertex_count = X(buffer)->vertices.size / (sizeof(float) *
                        ae_vertex_format_size[X(buffer)->vertex_format]);

        const size_t index_count = (X(buffer)->indices.size /
                ae_index_type_bytes[X(buffer)->index_type]);

        ae_if (vertex_count <= 256) // pow(2, sizeof(u8) * CHAR_BIT)
        {
            if (X(buffer)->index_type == AE_INDEX_TYPE_U32 ||
                X(buffer)->index_type == AE_INDEX_TYPE_U32)
            {
                assert( idx_smallest_fitting_type(X(buffer)->indices.data, // check
                        X(buffer)->index_type, index_count) == AE_INDEX_TYPE_U8 );

                idx_type_convert(&X(buffer)->indices, /* shrink indices */
                                X(buffer)->index_type, AE_INDEX_TYPE_U8);

                X(buffer)->index_type = AE_INDEX_TYPE_U8;
            }
        }

        else ae_if (vertex_count <= 65536) // pow(2, sizeof(u16) * CHAR_BIT)
        {
            if (X(buffer)->index_type == AE_INDEX_TYPE_U32)
            {
                assert( idx_smallest_fitting_type(X(buffer)->indices.data, // check
                        X(buffer)->index_type, index_count) == AE_INDEX_TYPE_U16);

                idx_type_convert(&X(buffer)->indices, /* shrink indices */
                                X(buffer)->index_type, AE_INDEX_TYPE_U16);

                X(buffer)->index_type = AE_INDEX_TYPE_U16;
            }
        }
    }
}

void gl_buffer_vertex_cache_optimize(gl_buffer_t* buffer)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        size_t vertex_count = X(buffer)->vertices.size / (sizeof(float) *
                        ae_vertex_format_size[X(buffer)->vertex_format]);

        const size_t index_count = (X(buffer)->indices.size /
                ae_index_type_bytes[X(buffer)->index_type]);

        idx_vertex_cache_optimize_ex(X(buffer)->indices.data,
            X(buffer)->index_type, index_count, vertex_count);
    }
}

double gl_buffer_vertex_cache_miss_ratio(gl_buffer_t* buffer,
                                    int cache_size, int FIFO)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        const size_t index_count = (X(buffer)->indices.size /
                ae_index_type_bytes[X(buffer)->index_type]);

        return idx_vertex_cache_miss_ratio( X(buffer)->indices.data,
                                            X(buffer)->index_type,
                                    index_count, cache_size, FIFO);
    }

    return 0.0;
}

int gl_buffer_intersect_ray3(gl_buffer_t * buffer,

                            const float ray_pos[3],
                            const float ray_dir[3],

                            u32 hit_indices[3],
                            float * hit_point,
                            float * t,
                            float * u,
                            float * v,

                            const float epsilon)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        // TODO: if compute_aabbox, check the ray against the aabbox first.
        // TODO: compute shader implementation with vertex buffer objects?
        GL_POS_TRANSFORM_FUNCTION_SETUP();

        assert(ray_pos && ray_dir && hit_indices && hit_point && t && u && v);

        int hit = tri3_mesh_vs_ray_ex(
            (float*)ray_pos,
            (float*)ray_dir,
            (float*)array->data,
            array->size / sizeof(float),
            vpos,
            size,
            X(buffer)->indices.data,
            X(buffer)->index_type,
            X(buffer)->indices.size / ae_index_type_bytes[X(buffer)->index_type],
            hit_indices,
            t, u, v,
            epsilon);

        if (hit)
        {
            tri3_ray_hit_point(
                hit_point,
                ray_pos,
                ray_dir,
                &((const float*)array->data)[hit_indices[0] * size + vpos],
                &((const float*)array->data)[hit_indices[1] * size + vpos],
                &((const float*)array->data)[hit_indices[2] * size + vpos],
                *t, *u, *v);
        }

        return hit;
    }

    return 0;
}

void gl_buffer_depthsort(gl_buffer_t* buffer, const float p[3])
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_POS_TRANSFORM_FUNCTION_SETUP();

        tri3_mesh_depthsort_ex(p, (float*)array->data, array->size / sizeof(float),
            vpos, size, X(buffer)->indices.data, X(buffer)->index_type,
            X(buffer)->indices.size / ae_index_type_bytes[X(buffer)->index_type]);
    }
}

void gl_buffer_invert_faces(gl_buffer_t* buffer)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_POS_TRANSFORM_FUNCTION_SETUP();

        tri3_mesh_invert_ex( (float *) array->data, array->size / sizeof( float ),
            size, X(buffer)->indices.data, X(buffer)->index_type,
            X(buffer)->indices.size / ae_index_type_bytes[X(buffer)->index_type]);
    }
}

static GLenum ae_index_type_to_opengl(ae_index_type_t type)
{
    ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8: return GL_UNSIGNED_BYTE;
        case AE_INDEX_TYPE_U16: return GL_UNSIGNED_SHORT;
        case AE_INDEX_TYPE_U32: return GL_UNSIGNED_INT;

        default:
        {
            ae_assert(0, "%s", ae_index_type_name[type]);
            return 0;
        }
        break;
    }
}

static GLenum ae_vertex_format_to_opengl(ae_vertex_format_t format)
{
    ae_switch (format, ae_vertex_format, AE_VERTEX_FORMAT, suffix)
    {
        case AE_T2F_N3F_V3F: return GL_T2F_N3F_V3F; // most common

        case AE_T2F_V3F: return GL_T2F_V3F;
        case AE_C3F_V3F: return GL_C3F_V3F;
        case AE_N3F_V3F: return GL_N3F_V3F;
        case AE_T4F_V4F: return GL_T4F_V4F;

        case AE_V2F: return GL_V2F;
        case AE_V3F: return GL_V3F;

        case AE_T2F_C3F_V3F: return GL_T2F_C3F_V3F;
        case AE_C4F_N3F_V3F: return GL_C4F_N3F_V3F;

        case AE_T2F_C4F_N3F_V3F: return GL_T2F_C4F_N3F_V3F;

        default:
        {
            ae_assert(0, "%s", ae_vertex_format_name[format]);
            return 0;
        }
        break;
    }
}

void gl_buffer_draw_ex(gl_buffer_t* buffer, gl_material_t* material)
{
    GL_PROFILE_SCOPE();

    if (gl_buffer_get_open(buffer))
    {
        assert(!X(buffer)->locked);

        GL_PushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
        GL_PushAttrib(GL_POLYGON_BIT | GL_LIGHTING_BIT);

        gl_material_apply_ex(material, buffer);

        // XXX: assumes we're in default fill mode in order to minimize state changes
        if (X(buffer)->line_mode)
        {
            GL_PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        // XXX: assumes we're in default smooth shading to minimize OpenGL API calls
        if (!X(buffer)->smooth_shading)
        {
            GL_ShadeModel(GL_FLAT);
        }

        GLenum gl_vertex_format = ae_vertex_format_to_opengl(X(buffer)->vertex_format);
        GL_InterleavedArrays(gl_vertex_format, 0, X(buffer)->vertices.data);

        ae_if (X(buffer)->indices.size)
        {
            GLenum gl_index_type = ae_index_type_to_opengl(X(buffer)->index_type);

            GLsizei index_count = X(buffer)->indices.size /
                ae_index_type_bytes[X(buffer)->index_type];

            assert(X(buffer)->draw_mode == GL_TRIANGLES ? \
                                index_count % 3 == 0 : 1);

            GL_DrawElements(X(buffer)->draw_mode, index_count, gl_index_type,
                                                    X(buffer)->indices.data);
        }
        else
        {
            GLsizei vertex_count = X(buffer)->vertices.size / (sizeof(float) *
                            ae_vertex_format_size[X(buffer)->vertex_format]);

            assert(X(buffer)->draw_mode == GL_TRIANGLES ? \
                                vertex_count % 3 == 0 : 1);

            GL_DrawArrays(X(buffer)->draw_mode, 0, vertex_count);
        }

        GL_PopAttrib();
        GL_PopClientAttrib();

        // TODO: GL_DEBUG, draw these only if it's #defined, color/enabled properties
        #if 0
        {
            static const float* c = ae_color_flt[ae_random_bounded_u32(AE_COLOR_COUNT)];
            gl_buffer_draw_aabbox(buffer, c);
        }
        #endif

        // TODO: GL_DEBUG, draw these only if it's #defined, color/enabled properties
        #if 0
        {
            static const u32 s = ae_random_bounded_u32(AE_COLOR_COUNT);
            static const u32 e = ae_random_bounded_u32(AE_COLOR_COUNT);

            gl_buffer_draw_normals_ex(buffer, ae_color_flt[s], ae_color_flt[e], 1.0f);
        }
        #endif
    }

    gl_debug_log_error_state();
}

void gl_buffer_draw(gl_buffer_t* buffer)
{
    gl_buffer_draw_ex(buffer, NULL);
}

void gl_buffer_draw_aabbox_ex(gl_buffer_t* buffer, const float* rgba,
                                                    float line_width)
{
    GL_PROFILE_SCOPE();

    if (gl_buffer_get_open(buffer))
    {
        float minv[3];
        float maxv[3];

        // TODO: check compute_aabbox property here?
        gl_buffer_get_aabbox(buffer, minv, maxv);
        gl_aabbox_ex(minv, maxv, rgba, line_width);
    }
}

void gl_buffer_draw_aabbox(gl_buffer_t* buffer, const float* rgba)
{
    gl_buffer_draw_aabbox_ex(buffer, rgba, 1.0f);
}

void gl_buffer_draw_normals_ex(gl_buffer_t* buffer, const float* start_color,
                                    const float* end_color, float line_width)
{
    GL_PROFILE_SCOPE();

    ae_vertex_format_t format = static_cast<ae_vertex_format_t>(
                            gl_buffer_get_vertex_format(buffer));

    if (ae_vertex_format_has_normal(format))
    {
        assert(!X(buffer)->locked);

        GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT);
        {
            GL_Disable(GL_TEXTURE_2D);
            GL_Disable(GL_LIGHTING);

            GL_LineWidth(line_width);

            GL_Enable(GL_BLEND); // default alpha blending mode
            GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            GL_Begin(GL_LINES);
            {
                const int num_verts = gl_buffer_get_vertex_count(buffer);
                const int vert_size = (int)ae_vertex_format_size[format];

                size_t p_offset = ae_vertex_format_position_offset[format];
                size_t n_offset = ae_vertex_format_normal_offset[format];

                for (int i = 0; i < num_verts * vert_size; i += vert_size)
                {
                    float* v = ((float*)X(buffer)->vertices.data) + i;

                    float* n = v + n_offset;
                    float* p = v + p_offset;

                    GL_Color4fv(start_color);
                    GL_Vertex3fv(p);

                    GL_Color4fv(end_color);
                    GL_Vertex3f(p[0] + n[0], p[1] + n[1], p[2] + n[2]);
                }
            }
            GL_End();
        }
        GL_PopAttrib();
    }

    gl_debug_log_error_state();
}

void gl_buffer_draw_normals(gl_buffer_t* buffer, const float* rgba)
{
    gl_buffer_draw_normals_ex(buffer, rgba, rgba, 1.0f);
}

#undef X

/* ===== [ mesh generation ] ================================================ */

#define PAR_PRINT(msg)          ae_log(OPENGL, "%s", msg)

#define PAR_PI                  M_PI /* TODO: redefine sin and cos */
#define PAR_MIN(a, b)           ((a) > (b) ? (b) : (a))
#define PAR_MAX(a, b)           ((a) > (b) ? (a) : (b))
#define PAR_SWAP(T, A, B)       { T tmp = B; B = A; A = tmp; }

#define PAR_MALLOC(T, N)        ((T*)ae_malloc((N) * sizeof(T)))
#define PAR_CALLOC(T, N)        ((T*)ae_calloc((N) * sizeof(T), 1))
#define PAR_REALLOC(T, P, N)    ((T*)ae_realloc((P), sizeof(T) * (N)))
#define PAR_FREE(P)             ae_free(P)

#define PAR_SHAPES_IMPLEMENTATION
#include <par_shapes.h>

static gl_buffer_t* gl_buffer_from_par_mesh(par_shapes_mesh* mesh)
{
    GL_PROFILE_SCOPE();

    gl_buffer_t* buffer = gl_buffer_create_empty(); // find format from mesh
    gl_internal_buffer_t* data = (gl_internal_buffer_t*)buffer;

    data->vertex_format = ae_vertex_format_from_sizes(mesh->tcoords ? 2 : 0,
                                                0, mesh->normals ? 3 : 0, 3);
    data->index_type = AE_INDEX_TYPE_U16;

    float* t = mesh->tcoords;
    float* n = mesh->normals;
    float* p = mesh->points;

    ae_array_reserve(&data->vertices, mesh->npoints * // avoid over-allocation
                ae_vertex_format_size[data->vertex_format] * sizeof(float));

    for (int i = 0; i < mesh->npoints; i++, t += 2, n += 3, p += 3)
    {
        /* TODO: this is terribly inefficient - the first step is to allocate
         * the buffer up front and switch on vertex format, the second is to
         * consume the par_shapes_mesh directly for indices. this means we have
         * to know if array memory is allocated with ae_malloc - maybe define
         * a hackish function like ae_array_uses_global_malloc or something...
         */
        gl_buffer_append_vertex(buffer, t, NULL, n, p);
    }

    assert( data->vertices.size == mesh->npoints * ae_vertex_format_size[
                                    data->vertex_format] * sizeof(float));

    if (mesh->npoints > 256) // shrink indices if possible, otherwise copy
    {
        ae_array_append(&data->indices, mesh->triangles,
                    mesh->ntriangles * 3 * sizeof(u16));
    }
    else
    {
        const ae_index_type_t s = AE_INDEX_TYPE_U16;
        const ae_index_type_t d = AE_INDEX_TYPE_U8;

        const int c = mesh->ntriangles * 3;
        assert(idx_fits_type(mesh->triangles, c, s, d));

        ae_array_resize(&data->indices, c);
        data->index_type = d;

        idx_type_converted_ex(mesh->triangles, s, data->indices.data, d, c);
    }

    par_shapes_free_mesh(mesh);
    return buffer;
}

#define X(c) GL_PROFILE_SCOPE(); return gl_buffer_from_par_mesh(c)

gl_buffer_t* gl_buffer_create_cylinder(int slices, int stacks)
{
    X(par_shapes_create_cylinder(slices, stacks));
}

gl_buffer_t* gl_buffer_create_torus(int slices, int stacks, float radius)
{
    X(par_shapes_create_torus(slices, stacks, radius));
}

gl_buffer_t* gl_buffer_create_parametric_sphere(int slices, int stacks)
{
    X(par_shapes_create_parametric_sphere(slices, stacks));
}

gl_buffer_t* gl_buffer_create_subdivided_sphere(int nsubdivisions)
{
    X(par_shapes_create_subdivided_sphere(nsubdivisions));
}

gl_buffer_t* gl_buffer_create_klein_bottle(int slices, int stacks)
{
    X(par_shapes_create_klein_bottle(slices, stacks));
}

gl_buffer_t* gl_buffer_create_trefoil_knot(int slices, int stacks, float radius)
{
    X(par_shapes_create_trefoil_knot(slices, stacks, radius));
}

gl_buffer_t* gl_buffer_create_hemisphere(int slices, int stacks)
{
    X(par_shapes_create_hemisphere(slices, stacks));
}

gl_buffer_t* gl_buffer_create_plane(int slices, int stacks)
{
    X(par_shapes_create_plane(slices, stacks));
}

gl_buffer_t* gl_buffer_create_parametric_shape(void (*func)(float const* uv,
            float* xyz, void* data), int slices, int stacks, void* userdata)
{
    X(par_shapes_create_parametric(func, slices, stacks, userdata));
}

gl_buffer_t* gl_buffer_create_icosahedron(void)
{
    X(par_shapes_create_icosahedron());
}

gl_buffer_t* gl_buffer_create_dodecahedron(void)
{
    X(par_shapes_create_dodecahedron());
}

gl_buffer_t* gl_buffer_create_octahedron(void)
{
    X(par_shapes_create_octahedron());
}

gl_buffer_t* gl_buffer_create_tetrahedron(void)
{
    X(par_shapes_create_tetrahedron());
}

gl_buffer_t* gl_buffer_create_cube(void)
{
    // FIXME: this function doesn't generate texture coordinates!
    X(par_shapes_create_cube());
}

gl_buffer_t* gl_buffer_create_disk( float radius, int slices,
                    float const* center, float const* normal)
{
    X(par_shapes_create_disk(radius, slices, center, normal));
}

gl_buffer_t* gl_buffer_create_rock(int seed, int nsubdivisions)
{
    X(par_shapes_create_rock(seed, nsubdivisions));
}

gl_buffer_t* gl_buffer_create_lsystem(char const* program,
                                int slices, int max_depth)
{
    X(par_shapes_create_lsystem(program, slices, max_depth));
}

#undef X

/*
================================================================================
 * ~~ [ particle system ] ~~ *
--------------------------------------------------------------------------------
TODO: readonly lock and unlock; take CRC in debug mode to ensure no writes occur
TODO: (un)lock --- should we interleave arrays in case we add a color component?
TODO: particle emitter auto-update mode similar to xl clock? (ae frame callback)
--------------------------------------------------------------------------------
*/

struct gl_internal_particle_emitter_t
{
    double time_created;
    int id, locked;

    const char* path;
    const char* name;

    gl_texture_t* texture;
    int owns_texture;
    float color[4];

    int frozen, burst_size;
    float gravity[3];
    double period, time;

    gl_particle_emitter_velgen_mode_t velgen_mode;
    float velgen_scale;

    int num_particles;
    int max_particles;
    int current_index;

    float min_point_size;
    float max_point_size;

    ae_array_t particles;
    ae_array_t vel_array;

    float position[3];
    float position_sigma[3];
    float velocity[3];
    float velocity_sigma[3];

    gl_particle_emitter_t* wrap()
    {
        return static_cast<gl_particle_emitter_t*>(this);
    }
};

#define X(p) ((gl_internal_particle_emitter_t*)(p))

static int gl_particle_emitter_compare(const void* a, const void* b)
{
    /* TODO: attempt lexicographical name sort before comparing age?
     */
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_particle_emitter_set_int(gl_particle_emitter_t* emitter, // integer
                            gl_particle_emitter_property_t property,
                            int value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_OWNS_TEXTURE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->owns_texture = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->velgen_mode = \
                                        (gl_particle_emitter_velgen_mode_t)value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_FROZEN:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->frozen = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_BURST_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->burst_size = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_NUM_PARTICLES:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                X(emitter)->num_particles = ae_imin(X(emitter)->max_particles, value);
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MAX_PARTICLES:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                const size_t floats = value * 3;
                const size_t bytes = floats * sizeof(float);

                ae_array_resize(&X(emitter)->particles, bytes);
                ae_array_resize(&X(emitter)->vel_array, bytes);

                X(emitter)->num_particles = ae_imin(X(emitter)->num_particles, value);
                X(emitter)->current_index = ae_imin(X(emitter)->current_index, value);

                X(emitter)->max_particles = value;
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_OPEN:
        {
            if (value == 0 && gl_particle_emitter_get_open(emitter))
            {
                GL_PROFILE_NAMED(gl_particle_emitter_close);

                ae_assert(!X(emitter)->locked, "freed locked emitter");

                ae_if (X(emitter)->owns_texture)
                {
                    gl_texture_close(X(emitter)->texture);
                }

                ae_string_free(const_cast<char*>(X(emitter)->path));
                ae_string_free(const_cast<char*>(X(emitter)->name));

                ae_array_free(&X(emitter)->particles);
                ae_array_free(&X(emitter)->vel_array);

                X(emitter)->~gl_internal_particle_emitter_t();

                ae_ptrset_remove(&gl_particle_emitter_lst, emitter);
                ae_free(emitter);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_particle_emitter_get_int(gl_particle_emitter_t* emitter, // integer
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_TOTAL:
        {
            return gl_particle_emitter_lst.count;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_ID:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->id;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_TEXTURE:
        {
            return gl_texture_get_id(gl_particle_emitter_get_texture(emitter));
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_OWNS_TEXTURE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->owns_texture;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->velgen_mode;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_FROZEN:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->frozen;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_BURST_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->burst_size;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_NUM_PARTICLES:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->num_particles;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MAX_PARTICLES:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->max_particles;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_particle_emitter_lst, emitter);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_particle_emitter_set_str(gl_particle_emitter_t* emitter, // string
                            gl_particle_emitter_property_t property,
                            const char* value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE:
        {
            gl_particle_emitter_set_velgen_mode(emitter, // TODO: avoid looping if closed
                                gl_particle_emitter_velgen_mode_from_short_name(value));
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_PATH:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                ae_string_free(const_cast<char*>(X(emitter)->path));
                X(emitter)->path = NULL;

                if (value != NULL)
                {
                    X(emitter)->path = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_NAME:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                ae_string_free(const_cast<char*>(X(emitter)->name));
                X(emitter)->name = NULL;

                if (value != NULL)
                {
                    X(emitter)->name = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_particle_emitter_get_str(gl_particle_emitter_t* emitter, // string
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                return gl_particle_emitter_velgen_mode_short_name[X(emitter)->velgen_mode];
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_STATUS:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                static char gl_emitter_status[1024];
                const char* name = X(emitter)->name;

                if (!name || !name[0]) name = "?";

                if (AE_SNPRINTF(gl_emitter_status, "\"%s\", num %i, max %i", name,
                        X(emitter)->num_particles, X(emitter)->max_particles) < 0)
                {
                    AE_WARN("%u bytes is not enough for emitter status!",
                                (unsigned int)sizeof(gl_emitter_status));
                }

                return (const char*)gl_emitter_status;
            }
            else
            {
                return "closed";
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_PATH:
        {
            // TODO: if the emitter doesn't have a path, return the texture's path
            if (gl_particle_emitter_get_open(emitter) && X(emitter)->path != NULL)
            {
                return X(emitter)->path;
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_NAME:
        {
            // TODO: if the emitter doesn't have a name, return the texture's name
            if (gl_particle_emitter_get_open(emitter) && X(emitter)->name != NULL)
            {
                return X(emitter)->name;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

void
gl_particle_emitter_set_vec(gl_particle_emitter_t* emitter, // vector
                            gl_particle_emitter_property_t property,
                            float* value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_COLOR:
        {
            if (gl_particle_emitter_get_open(emitter)) vec4copy(X(emitter)->color, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_POSITION:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->position, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_POSITION_SIGMA:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->position_sigma, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELOCITY:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->velocity, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELOCITY_SIGMA:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->velocity_sigma, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_GRAVITY:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->gravity, value);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float*
gl_particle_emitter_get_vec(gl_particle_emitter_t* emitter, // vector
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_COLOR:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->color;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_POSITION:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->position;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_POSITION_SIGMA:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->position_sigma;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELOCITY:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->velocity;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELOCITY_SIGMA:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->velocity_sigma;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_GRAVITY:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->gravity;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    static const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    return const_cast<float*>(zero);
}

void
gl_particle_emitter_set_flt(gl_particle_emitter_t* emitter, // float
                            gl_particle_emitter_property_t property,
                            float value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_SCALE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->velgen_scale = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MIN_POINT_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->min_point_size = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MAX_POINT_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->max_point_size = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float
gl_particle_emitter_get_flt(gl_particle_emitter_t* emitter, // float
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_SCALE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->velgen_scale;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MIN_POINT_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->min_point_size;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MAX_POINT_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->max_point_size;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0.0f;
}

void
gl_particle_emitter_set_dbl(gl_particle_emitter_t* emitter, // double
                            gl_particle_emitter_property_t property,
                            double value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_PERIOD:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->period = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

double
gl_particle_emitter_get_dbl(gl_particle_emitter_t* emitter, // double
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_PERIOD:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->period;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0.0;
}

void
gl_particle_emitter_set_tex(gl_particle_emitter_t* emitter, // texture
                            gl_particle_emitter_property_t property,
                            gl_texture_t* value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_TEXTURE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->texture = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

gl_texture_t*
gl_particle_emitter_get_tex(gl_particle_emitter_t* emitter, // texture
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_TEXTURE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->texture;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return NULL;
}

gl_particle_emitter_t* gl_particle_emitter_create(void)
{
    GL_PROFILE_SCOPE();

    ae_assert(gl_particle_emitter_count_all() <= GL_OBJECT_COUNT_LIMIT,
                    "hit particle emitter object count limit - leak?");

    gl_internal_particle_emitter_t* data = ae_create(gl_internal_particle_emitter_t, clear);
    gl_particle_emitter_t* emitter = data->wrap();

    data->time_created = ae_seconds();
    data->id = (int)ae_random_u32();

    data->color[0] = data->color[1] = // white
    data->color[2] = data->color[3] = 1.0f;

    data->velgen_mode = GL_PARTICLE_EMITTER_VELGEN_MODE_NONE;
    data->velgen_scale = 1.0f;

    data->burst_size = 1;
    data->period = 0.1;

    data->min_point_size = 0.0f;
    data->max_point_size = 2.0f;

    // call constructor, which calls member object constructors
    new (data) gl_internal_particle_emitter_t;

    if (ae_ptrset_add(&gl_particle_emitter_lst, data) == 0)
    {
        AE_WARN("particle emitter is not new to the set (is set code stubbed?)");
    }

    return emitter;
}

void gl_particle_emitter_lock(gl_particle_emitter_t* emitter,
                ae_array_t* vel_array, ae_array_t* particles)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        ae_assert(!X(emitter)->locked, "particle emitter is already locked!");

        assert(particles); *particles = X(emitter)->particles;
        memset(&X(emitter)->particles, 0, sizeof(ae_array_t));

        assert(vel_array); *vel_array = X(emitter)->vel_array;
        memset(&X(emitter)->vel_array, 0, sizeof(ae_array_t));

        const size_t num_bytes = X(emitter)->num_particles * sizeof(float[3]);

        ae_array_resize(particles, num_bytes);
        ae_array_resize(vel_array, num_bytes);

        X(emitter)->locked = 1;
    }
}

void gl_particle_emitter_unlock(gl_particle_emitter_t* emitter,
                ae_array_t* vel_array, ae_array_t* particles)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        // TODO: assert particle and velocity array sizes match with each other
        // TODO: set new (num/max)_particles members if the array sizes change
        ae_assert(X(emitter)->locked, "particle emitter is already unlocked!");

        const size_t num_bytes = X(emitter)->max_particles * sizeof(float[3]);

        ae_array_resize(particles, num_bytes);
        ae_array_resize(vel_array, num_bytes);

        assert(particles); X(emitter)->particles = *particles;
        memset(particles, 0, sizeof(ae_array_t));

        assert(vel_array); X(emitter)->vel_array = *vel_array;
        memset(vel_array, 0, sizeof(ae_array_t));

        X(emitter)->locked = 0;
    }
}

void gl_particle_emitter_get(gl_particle_emitter_t* emitter,
                    size_t which, float * vel, float * pos)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        ae_assert((int)which < X(emitter)->num_particles, "idx %i >= max %i",
                                    (int)which, X(emitter)->num_particles);

        assert(!X(emitter)->locked);

        vec3copy(vel, (float*)X(emitter)->vel_array.data + which * 3);
        vec3copy(pos, (float*)X(emitter)->particles.data + which * 3);
    }
    else
    {
        AE_WARN("tried to get a particle from a closed emitter");

        vec3zero(vel);
        vec3zero(pos);
    }
}

void gl_particle_emitter_set(gl_particle_emitter_t* emitter,
                    size_t which, float * vel, float * pos)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        ae_assert((int)which < X(emitter)->num_particles, "idx %i >= max %i",
                                    (int)which, X(emitter)->num_particles);

        assert(!X(emitter)->locked);

        vec3copy((float*)X(emitter)->vel_array.data + which * 3, vel);
        vec3copy((float*)X(emitter)->particles.data + which * 3, pos);
    }
    else
    {
        AE_WARN("tried to set a particle on a closed emitter");
    }
}

void gl_particle_emitter_append(gl_particle_emitter_t * emitter,
                            float* vel, float* pos, int expand)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        ae_if (X(emitter)->num_particles == X(emitter)->max_particles)
        {
            ae_if (expand)
            {
                gl_particle_emitter_set_max_particles(emitter,
                                X(emitter)->max_particles+1);

                goto copy_and_increment_num_particles;
            }
            else
            {
                ae_assert(0, "TODO: copy particle to current index?");
            }
        }
        else
        {
            copy_and_increment_num_particles:
            {
                vec3copy((float *)X(emitter)->vel_array.data +
                        X(emitter)->num_particles * 3, vel);

                vec3copy((float *)X(emitter)->particles.data +
                        X(emitter)->num_particles * 3, pos);

                X(emitter)->num_particles++;
            }
        }
    }
}

// TODO: implement the functional opposite of this (gl_particle_emitter_slice), which
// would create a copy of a particle emitter's particles & vels within a given range.

void gl_particle_emitter_merge(gl_particle_emitter_t* dst,
                    gl_particle_emitter_t* src, int copy)
{
    GL_PROFILE_SCOPE();

    ae_if (gl_particle_emitter_get_open(dst) && gl_particle_emitter_get_open(src))
    {
        for (int i = 0; i < X(src)->num_particles; i++)
        {
            float vel[3], pos[3]; // TODO: faster path!!!

            gl_particle_emitter_get(src, i, vel, pos);
            gl_particle_emitter_append(dst, vel, pos, 1);
        }

        ae_if (!copy)
        {
            gl_particle_emitter_close(src);
        }
    }
}

void
gl_particle_emitter_get_aabbox(gl_particle_emitter_t* emitter, float* minv, float* maxv)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        vtx_aabbox3(minv, maxv, (float*)X(emitter)->particles.data,
                                    X(emitter)->num_particles * 3);
    }
    else
    {
        AE_WARN("got an empty bounding box from a closed emitter");

        vec3zero(minv);
        vec3zero(maxv);
    }
}

void gl_particle_emitter_translate(gl_particle_emitter_t* emitter, float* xyz)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        vtx_vec3add(static_cast<float*>(X(emitter)->particles.data),
                                xyz, X(emitter)->num_particles * 3);
    }
}

void gl_particle_emitter_scale(gl_particle_emitter_t* emitter, float* xyz)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        vtx_vec3mul(static_cast<float*>(X(emitter)->particles.data),
                                xyz, X(emitter)->num_particles * 3);
    }
}

void
gl_particle_emitter_lerp(gl_particle_emitter_t* dst, gl_particle_emitter_t* a,
                                            gl_particle_emitter_t* b, float t)
{
    GL_PROFILE_SCOPE();

    if (gl_particle_emitter_get_open(a) &&
        gl_particle_emitter_get_open(b) &&
        gl_particle_emitter_get_open(dst))
    {
        assert(!X(dst)->locked);
        assert(!X( a )->locked);
        assert(!X( b )->locked);

        #if 0
            ae_assert(X(a)->texture == X(b)->texture, "TODO: texture interpolation");
            X(dst)->texture = X(a)->texture;
        #else
            if (t < 0.5f)
            {
                X(dst)->texture = X(a)->texture;
            }
            else
            {
                X(dst)->texture = X(b)->texture;
            }
        #endif

        // XXX once texture interpolation is implemented, we might need to delete
        // and create a new dst texture in the event of a resize. when that path
        // is implemented, leave this value alone - dst will have its own texture.
        X(dst)->owns_texture = 0;

        ae_assert(X(a)->num_particles == X(b)->num_particles,
                    "A num particles %i, B num particles %i",
                    X(a)->num_particles, X(b)->num_particles);

        ae_assert(X(a)->max_particles == X(b)->max_particles,
                    "A max particles %i, B max particles %i",
                    X(a)->max_particles, X(b)->max_particles);

        X(dst)->num_particles = X(a)->num_particles;
        X(dst)->max_particles = X(a)->max_particles;

        ae_array_t dst_particles;
        ae_array_t dst_vel_array;
        gl_particle_emitter_lock(dst, &dst_vel_array, &dst_particles);

        // TODO: should we lerp position, velocity, gravity, etc?
        #if 1
        {
            vtx_lerp((float*)dst_particles.data, // pos array

                    (const float* const)X(a)->particles.data,
                    (const float* const)X(b)->particles.data,

                    t, dst_particles.size / sizeof( float ));

            vtx_lerp((float*)dst_vel_array.data, // vel array

                    (const float* const)X(a)->vel_array.data,
                    (const float* const)X(b)->vel_array.data,

                    t, dst_vel_array.size / sizeof( float ));

            X(dst)->min_point_size = ae_lerpf(X(a)->min_point_size,
                                        X(b)->min_point_size, t);

            X(dst)->max_point_size = ae_lerpf(X(a)->max_point_size,
                                        X(b)->max_point_size, t);

            vec4lerp(X(dst)->color, X(a)->color, X(b)->color, t);
        }
        #else
        {
            if (t < 0.5f)
            {
                ae_array_copy(&dst_particles, &X(a)->particles);
                ae_array_copy(&dst_vel_array, &X(a)->vel_array);

                vec4copy(X(dst)->color, X(a)->color);

                X(dst)->min_point_size = X(a)->min_point_size;
                X(dst)->max_point_size = X(a)->max_point_size;
            }
            else
            {
                ae_array_copy(&dst_particles, &X(b)->particles);
                ae_array_copy(&dst_vel_array, &X(b)->vel_array);

                vec4copy(X(dst)->color, X(b)->color);

                X(dst)->min_point_size = X(b)->min_point_size;
                X(dst)->max_point_size = X(b)->max_point_size;
            }
        }
        #endif

        gl_particle_emitter_unlock(dst, &dst_vel_array, &dst_particles);
    }
}

void gl_particle_emitter_transform(gl_particle_emitter_t* emitter, float* matrix)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        vtx_mat4x4row_mul_point3(static_cast<float*>(X(emitter)->particles.data),
                                            matrix, X(emitter)->num_particles*3);
    }
}

void gl_particle_emitter_center_at_origin(gl_particle_emitter_t* emitter)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        float minv[3], maxv[3], center[3];
        gl_particle_emitter_get_aabbox(emitter, minv, maxv);

        aabbox3_get_center(center, minv, maxv);
        vec3neg(center, center);
        gl_particle_emitter_translate(emitter, center);
    }
}

gl_particle_emitter_velgen_mode_t
gl_particle_emitter_velgen_mode_from_short_name(const char* name)
{
    // TODO: this could be sped up with a hashtable (short name -> index)
    GL_PROFILE_SCOPE();

    size_t i = 0, n = GL_PARTICLE_EMITTER_VELGEN_MODE_COUNT;
    for (; i < n; i++)
    {
        if (!strcmp(gl_particle_emitter_velgen_mode_short_name[i], name))
        {
            return (gl_particle_emitter_velgen_mode_t)i;
        }
    }

    ae_assert(0, "\"%s\" is not a valid velgen mode", name);
    return GL_PARTICLE_EMITTER_VELGEN_MODE_COUNT;
}

void gl_particle_emitter_aabbox_limit(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        /* TODO: ae_particle/pfx_aabbox3_limit, SSE path? (it's slow)
         */
        float* ptr = static_cast<float*>(X(emitter)->particles.data);
        float* end = ptr + X(emitter)->num_particles * 3;

        // select a point to return to, either emitter pos or center
        float origin[3];

        if (aabbox3_vs_point(minv, maxv, X(emitter)->position))
        {
            vec3copy(origin, X(emitter)->position);
        }
        else
        {
            aabbox3_get_center(origin, minv, maxv);
        }

        for (; ptr < end; ptr += 3)
        {
            if( ptr[0] < minv[0] ||
                ptr[1] < minv[1] ||
                ptr[2] < minv[2] ||
                ptr[0] > maxv[0] ||
                ptr[1] > maxv[1] ||
                ptr[2] > maxv[2] )
            {
                vec3copy(ptr, origin);
            }
        }
    }
}

void gl_particle_emitter_aabbox_clamp(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        /* TODO: move this function to ae_particle/pfx_aabbox3_clamp
         */
        float* ptr = static_cast<float*>(X(emitter)->particles.data);
        float* end = ptr + X(emitter)->num_particles * 3;

        #if defined(__SSE__)
        if(ae_cpuinfo_sse())
        {
            if (ae_likely(num_is_aligned((size_t)ptr, 16))) // packed main loop unrolling
            {
                const size_t count = X(emitter)->num_particles;
                float *unr = ptr + (count - (count % 12));

                const __m128 min0 = _mm_setr_ps(minv[0], minv[1], minv[2], minv[0]);
                const __m128 min1 = _mm_setr_ps(minv[1], minv[2], minv[0], minv[1]);
                const __m128 min2 = _mm_setr_ps(minv[2], minv[0], minv[1], minv[2]);

                const __m128 max0 = _mm_setr_ps(maxv[0], maxv[1], maxv[2], maxv[0]);
                const __m128 max1 = _mm_setr_ps(maxv[1], maxv[2], maxv[0], maxv[1]);
                const __m128 max2 = _mm_setr_ps(maxv[2], maxv[0], maxv[1], maxv[2]);

                for (; ptr < unr; ptr += 12)
                {
                    _mm_store_ps(ptr + 0, _mm_clamp_ps(_mm_load_ps(ptr + 0), min0, max0));
                    _mm_store_ps(ptr + 4, _mm_clamp_ps(_mm_load_ps(ptr + 4), min1, max1));
                    _mm_store_ps(ptr + 8, _mm_clamp_ps(_mm_load_ps(ptr + 8), min2, max2));
                }
            }

            if (1) // unaligned / leftover path
            {
                const __m128 min0 = _mm_set_ss(minv[0]);
                const __m128 min1 = _mm_set_ss(minv[1]);
                const __m128 min2 = _mm_set_ss(minv[2]);

                const __m128 max0 = _mm_set_ss(maxv[0]);
                const __m128 max1 = _mm_set_ss(maxv[1]);
                const __m128 max2 = _mm_set_ss(maxv[2]);

                for (; ptr < end; ptr += 3)
                {
                    _mm_store_ss(ptr + 0, _mm_min_ss(_mm_max_ss(
                                _mm_set_ss(ptr[0]), min0), max0));

                    _mm_store_ss(ptr + 1, _mm_min_ss(_mm_max_ss(
                                _mm_set_ss(ptr[1]), min1), max1));

                    _mm_store_ss(ptr + 2, _mm_min_ss(_mm_max_ss(
                                _mm_set_ss(ptr[2]), min2), max2));
                }
            }
        }
        #endif

        for (; ptr < end; ptr += 3)
        {
            vec3clamp_vec(ptr, ptr, minv, maxv);
        }
    }
}

void gl_particle_emitter_aabbox_reset(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        float center[3]; // get center for extent vector
        aabbox3_get_center(center, minv, maxv);

        float extent[3] =
        {
            ae_fabsf(maxv[0] - center[0]),
            ae_fabsf(maxv[1] - center[1]),
            ae_fabsf(maxv[2] - center[2]),
        };

        X(emitter)->num_particles = X(emitter)->max_particles;

        float* pos = (float*)X(emitter)->particles.data;
        float* end = pos + X(emitter)->num_particles * 3;
        float* vel = (float*)X(emitter)->vel_array.data;

        #define SIDE_UP                                             \
            pos[0] = center[0] + extent[0] * ae_random_bi_flt();    \
            pos[1] = center[1] + extent[1];                         \
            pos[2] = center[2] + extent[2] * ae_random_bi_flt();    \

        #define SIDE_DN                                             \
            pos[0] = center[0] + extent[0] * ae_random_bi_flt();    \
            pos[1] = center[1] - extent[1];                         \
            pos[2] = center[2] + extent[2] * ae_random_bi_flt();    \

        #define SIDE_BK                                             \
            pos[0] = center[0] + extent[0] * ae_random_bi_flt();    \
            pos[1] = center[1] + extent[1] * ae_random_bi_flt();    \
            pos[2] = center[2] + extent[2];                         \

        #define SIDE_FT                                             \
            pos[0] = center[0] + extent[0] * ae_random_bi_flt();    \
            pos[1] = center[1] + extent[1] * ae_random_bi_flt();    \
            pos[2] = center[2] - extent[2];                         \

        #define SIDE_RT                                             \
            pos[0] = center[0] + extent[0];                         \
            pos[1] = center[1] + extent[1] * ae_random_bi_flt();    \
            pos[2] = center[2] + extent[2] * ae_random_bi_flt();    \

        #define SIDE_LF                                             \
            pos[0] = center[0] - extent[0];                         \
            pos[1] = center[1] + extent[1] * ae_random_bi_flt();    \
            pos[2] = center[2] + extent[2] * ae_random_bi_flt();    \

        ae_switch (X(emitter)->velgen_mode, gl_particle_emitter_velgen_mode,
                                    GL_PARTICLE_EMITTER_VELGEN_MODE, suffix)
        {
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    switch (ae_random_bounded_u32(6))
                    {
                        case 0:
                        {
                            SIDE_UP;

                            vel[0] = 0.0f;
                            vel[1] = +X(emitter)->velgen_scale;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 1:
                        {
                            SIDE_DN;

                            vel[0] = 0.0f;
                            vel[1] = -X(emitter)->velgen_scale;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 2:
                        {
                            SIDE_BK;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = +X(emitter)->velgen_scale;
                        }
                        break;

                        case 3:
                        {
                            SIDE_FT;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = -X(emitter)->velgen_scale;
                        }
                        break;

                        case 4:
                        {
                            SIDE_RT;

                            vel[0] = +X(emitter)->velgen_scale;
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 5:
                        {
                            SIDE_LF;

                            vel[0] = -X(emitter)->velgen_scale;
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        default:
                        {
                            assert(0);
                        }
                        break;
                    }
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    switch (ae_random_bounded_u32(6))
                    {
                        case 0:
                        {
                            SIDE_UP;

                            vel[0] = 0.0f;
                            vel[1] = +X(emitter)->velgen_scale * ae_random_flt();
                            vel[2] = 0.0f;
                        }
                        break;

                        case 1:
                        {
                            SIDE_DN;

                            vel[0] = 0.0f;
                            vel[1] = -X(emitter)->velgen_scale * ae_random_flt();
                            vel[2] = 0.0f;
                        }
                        break;

                        case 2:
                        {
                            SIDE_BK;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = +X(emitter)->velgen_scale * ae_random_flt();
                        }
                        break;

                        case 3:
                        {
                            SIDE_FT;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = -X(emitter)->velgen_scale * ae_random_flt();
                        }
                        break;

                        case 4:
                        {
                            SIDE_RT;

                            vel[0] = +X(emitter)->velgen_scale * ae_random_flt();
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 5:
                        {
                            SIDE_LF;

                            vel[0] = -X(emitter)->velgen_scale * ae_random_flt();
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        default:
                        {
                            assert(0);
                        }
                        break;
                    }
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_BI_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_BI_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    switch (ae_random_bounded_u32(6))
                    {
                        case 0:
                        {
                            SIDE_UP;

                            vel[0] = 0.0f;
                            vel[1] = X(emitter)->velgen_scale * ae_random_bi_flt();
                            vel[2] = 0.0f;
                        }
                        break;

                        case 1:
                        {
                            SIDE_DN;

                            vel[0] = 0.0f;
                            vel[1] = X(emitter)->velgen_scale * ae_random_bi_flt();
                            vel[2] = 0.0f;
                        }
                        break;

                        case 2:
                        {
                            SIDE_BK;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = X(emitter)->velgen_scale * ae_random_bi_flt();
                        }
                        break;

                        case 3:
                        {
                            SIDE_FT;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = X(emitter)->velgen_scale * ae_random_bi_flt();
                        }
                        break;

                        case 4:
                        {
                            SIDE_RT;

                            vel[0] = X(emitter)->velgen_scale * ae_random_bi_flt();
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 5:
                        {
                            SIDE_LF;

                            vel[0] = X(emitter)->velgen_scale * ae_random_bi_flt();
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        default:
                        {
                            assert(0);
                        }
                        break;
                    }
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_NONE:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO:
            {
                for (; pos < end; pos += 3)
                {
                    switch (ae_random_bounded_u32(6))
                    {
                        case 0: { SIDE_UP; } break;
                        case 1: { SIDE_DN; } break;
                        case 2: { SIDE_BK; } break;
                        case 3: { SIDE_FT; } break;
                        case 4: { SIDE_RT; } break;
                        case 5: { SIDE_LF; } break;
                    }
                }

                if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM)
                {
                    end = vel + X(emitter)->num_particles * 3;

                    for (; vel < end; vel += 3)
                    {
                        vel[0] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[1] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[2] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                    }
                }
                else if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO)
                {
                    memset(vel, 0, X(emitter)->num_particles * sizeof(float[3]));
                }
            }
            break;

            default:
            {
                assert(0);
            }
            break;
        }

        #undef SIDE_UP
        #undef SIDE_DN
        #undef SIDE_BK
        #undef SIDE_FT
        #undef SIDE_RT
        #undef SIDE_LF

        // X(emitter)->frozen = 1;
    }
}

void gl_particle_emitter_sphere_limit(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        // TODO: move this function to ae_particle/pfx_sphere_limit
        const float radius2 = radius * radius;

        float* ptr = static_cast<float*>(X(emitter)->particles.data);
        float* end = ptr + X(emitter)->num_particles * 3;

        for (; ptr < end; ptr += 3)
        {
            float dir[3]; // vector from center
            vec3sub_vec(dir, ptr, center);

            // TODO: if emitter pos is inside the sphere, copy that
            if (vec3mag2(dir) >= radius2) { vec3copy(ptr, center); }
        }
    }
}

void gl_particle_emitter_sphere_clamp(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        // TODO: move this function to ae_particle/pfx_sphere_clamp
        const float radius2 = radius * radius;

        float* ptr = static_cast<float*>(X(emitter)->particles.data);
        float* end = ptr + X(emitter)->num_particles * 3;

        /* NOTE: using the fast inverse square root or _mm_rsqrt_ss
         * here loses too much precision and makes particles jitter
         */
        for (; ptr < end; ptr += 3)
        {
            float dir[3]; // vector from center
            vec3sub_vec(dir, ptr, center);

            if (vec3mag2(dir) >= radius2)
            {
                vec3normalize(dir, dir);

                vec3mul_flt(ptr, dir, radius);
                vec3add_vec(ptr, ptr, center);
            }
        }
    }
}

void gl_particle_emitter_sphere_reset(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        X(emitter)->num_particles = X(emitter)->max_particles; // fill emitter

        float* pos = (float*)X(emitter)->particles.data;
        float* end = pos + X(emitter)->num_particles * 3;
        float* vel = (float*)X(emitter)->vel_array.data;

        #define PICK_POS()                  \
            ae_random_unit_vec3(pos);       \
                                            \
            vec3mul_flt(pos, pos, radius);  \
            vec3add_vec(pos, pos, center);  \

        #define SPHERE_V(s)                 \
            vec3sub_vec(vel, pos, center);  \
            vec3normalize(vel, vel);        \
            vec3mul_flt(vel, vel, X(emitter)->velgen_scale * (s));

        ae_switch (X(emitter)->velgen_mode, gl_particle_emitter_velgen_mode,
                                    GL_PARTICLE_EMITTER_VELGEN_MODE, suffix)
        {
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    PICK_POS(); // TODO: SSE normals (for velocity only)
                    SPHERE_V(1.0f);
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    PICK_POS(); // TODO: SSE normals (for velocity only)
                    SPHERE_V(ae_random_flt());
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_BI_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_BI_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    PICK_POS(); // TODO: SSE normals (for velocity only)
                    SPHERE_V(ae_random_bi_flt());
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_NONE:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO:
            {
                for (; pos < end; pos += 3) { PICK_POS(); }

                if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM)
                {
                    end = vel + X(emitter)->num_particles * 3;

                    for (; vel < end; vel += 3)
                    {
                        vel[0] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[1] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[2] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                    }
                }
                else if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO)
                {
                    memset(vel, 0, X(emitter)->num_particles * sizeof(float[3]));
                }
            }
            break;

            default:
            {
                assert(0);
            }
            break;
        }

        #undef PICK_POS
        #undef SPHERE_V

        // X(emitter)->frozen = 1;
    }
}

void gl_particle_emitter_radius_limit(gl_particle_emitter_t* emitter, float radius)
{
    GL_PROFILE_SCOPE();

    float* v = gl_particle_emitter_get_position(emitter);
    gl_particle_emitter_sphere_limit(emitter, v, radius);
}

void gl_particle_emitter_radius_clamp(gl_particle_emitter_t* emitter, float radius)
{
    GL_PROFILE_SCOPE();

    float* v = gl_particle_emitter_get_position(emitter);
    gl_particle_emitter_sphere_clamp(emitter, v, radius);
}

void gl_particle_emitter_radius_reset(gl_particle_emitter_t* emitter, float radius)
{
    GL_PROFILE_SCOPE();

    float* v = gl_particle_emitter_get_position(emitter);
    gl_particle_emitter_sphere_reset(emitter, v, radius);
}

void gl_particle_emitter_line_reset(gl_particle_emitter_t* emitter,
                                    float* min_val, float* max_val)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        X(emitter)->num_particles = X(emitter)->max_particles;

        float* pos = (float*)X(emitter)->particles.data;
        float* end = pos + X(emitter)->num_particles * 3;
        float* vel = (float*)X(emitter)->vel_array.data;

        /* these don't really have to point up or right, as long as they're orthogonal.
         */
        const float up_dir[3] = { 0.0f, 1.0f, 0.0f };
        const float rt_dir[3] = { 1.0f, 0.0f, 0.0f };

        /* we use these min and max values to store the beginning and endpoints of the
         * line; this may look like extent calculation, but we're calculating line dir.
         */
        float dir[3];
        float adj[3];

        vec3sub_vec(dir, max_val, min_val);
        vec3normalize(dir, dir);
        vec3cross(adj, dir, up_dir);

        /* since each vector has an infinite number of perpendicular vectors, we just
         * pick a random one; this tries to ensure that our axis of rotation is always
         * perpendicular to the direction of the line, but it fails in certain cases.
         * we mitigate this by just renormalizing the perpendicular vector if need be.
         */
        ae_if (vec3mag2(adj) < 0.01f) vec3cross(adj, dir, rt_dir);
        ae_if (vec3mag2(adj) < 1.00f) vec3normalize(adj, adj);

        ae_switch (X(emitter)->velgen_mode, gl_particle_emitter_velgen_mode,
                                    GL_PARTICLE_EMITTER_VELGEN_MODE, suffix)
        {
            /*  TODO: if they're common, axial rotation cases could be sped up
                by just using sin and cos instead of arbitrary-axis rotations!
                should also see if matrix creation + multiplication is cheaper.
             */
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    vec3lerp(pos, min_val, max_val, ae_random_flt());
                    vec3rotate(vel, adj, dir, 2.0 * ae_random_flt() * M_PI);

                    vec3mul_flt(vel, vel, X(emitter)->velgen_scale);
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_BI_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_BI_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    vec3lerp(pos, min_val, max_val, ae_random_flt()); // random scale
                    vec3rotate(vel, adj, dir, 2.0 * ae_random_flt() * M_PI);

                    vec3mul_flt(vel, vel, X(emitter)->velgen_scale * ae_random_flt());
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_NONE:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO:
            {
                for (; pos < end; pos += 3)
                {
                    vec3lerp(pos, min_val, max_val, ae_random_flt());
                }

                if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM)
                {
                    end = vel + X(emitter)->num_particles * 3;

                    for (; vel < end; vel += 3)
                    {
                        vel[0] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[1] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[2] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                    }
                }
                else if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO)
                {
                    memset(vel, 0, X(emitter)->num_particles * sizeof(float[3]));
                }
            }
            break;

            default:
            {
                assert(0);
            }
            break;
        }

        // X(emitter)->frozen = 1;
    }
}

// TODO: emitters with attached vertices and indices, which will generate new
// particles on a random point on a random polygon with optional velocity gen.
// TODO: SSE velgen normals (_mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(sqrlen))))
// TODO: get tri normal in vertex assignment and offset particles by some val

#define GL_PARTICLE_EMITTER_MESH_FUNC_ENTER()                               \
                                                                            \
    GL_PROFILE_SCOPE();                                                     \
                                                                            \
    if (!gl_particle_emitter_get_open(emitter) ||                           \
        !gl_buffer_get_open(buffer)) { return; }                            \
                                                                            \
    /* FIXME: mega hack around triangle point picking bias/cluster issues!  \
     */                                                                     \
    ae_assert(!(particles_per_triangle % 3), "particle count per triangle " \
        "must be a multiple of 3 (got %u)!", (u32) particles_per_triangle); \
                                                                            \
    /* buffer_data (gl_internal_buffer_t*)                                  \
     * vertex_format (ae_vertex_format_t)                                   \
     * vertex_size (in floats)                                              \
     * float_count (vertex array floats)                                    \
     * vertex_count                                                         \
     * (tex/col/nrm/pos)_(offset/size)                                      \
     */                                                                     \
    GL_BUFFER_TOTAL_VERTEX_FORMAT_INFO();                                   \
                                                                            \
    size_t index_bytes;                                                     \
    u32* indices;                                                           \
    size_t index_count;                                                     \
                                                                            \
    ae_if (buffer_data->indices.size == 0) /* raw verts; gen id indices */  \
    {                                                                       \
        index_bytes = vertex_count * sizeof(u32);                           \
        index_count = vertex_count;                                         \
                                                                            \
        indices = (u32*)ae_stack_malloc(ae_global_stack(), index_bytes);    \
        idx_identity(indices, AE_INDEX_TYPE_U32, index_count);              \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        ae_index_type_t t = buffer_data->index_type; /* convert to u32 */   \
                                                                            \
        index_count = buffer_data->indices.size / ae_index_type_bytes[t];   \
        index_bytes = index_count * sizeof(u32);                            \
        indices = (u32*)ae_stack_malloc(ae_global_stack(), index_bytes);    \
                                                                            \
        idx_type_converted_ex(buffer_data->indices.data,                    \
                        buffer_data->index_type, indices,                   \
                        AE_INDEX_TYPE_U32, index_count);                    \
    }                                                                       \
                                                                            \
    const size_t num_triangles = index_count / 3; /* calculate resize */    \
    const size_t old_particles = X(emitter)->num_particles;                 \
                                                                            \
    const size_t new_particles = num_triangles * particles_per_triangle;    \
    const size_t new_total_num = old_particles + new_particles;             \
                                                                            \
    X(emitter)->num_particles = new_total_num;                              \
    X(emitter)->max_particles = new_total_num;                              \
                                                                            \
    ae_array_t vel_array;                                                   \
    ae_array_t particles;                                                   \
    gl_particle_emitter_lock(emitter, &vel_array, &particles);              \
                                                                            \
    float* pos = ((float*)particles.data) + old_particles * 3;              \
    float* p_e = ((float*)particles.data) + new_total_num * 3;              \
                                                                            \
    float* vel = ((float*)vel_array.data) + old_particles * 3;              \
    float* v_e = ((float*)vel_array.data) + new_total_num * 3;              \

#define GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS()                             \
    /*                                                                      \
     * TODO: store buffer vertices in a local to avoid slow pointer derefs  \
     */                                                                     \
    float* v0 = &((float*)buffer_data->vertices.data)[                      \
            indices[i + 0] * vertex_size + pos_offset];                     \
                                                                            \
    float* v1 = &((float*)buffer_data->vertices.data)[                      \
            indices[i + 1] * vertex_size + pos_offset];                     \
                                                                            \
    float* v2 = &((float*)buffer_data->vertices.data)[                      \
            indices[i + 2] * vertex_size + pos_offset];                     \

#define GL_PARTICLE_EMITTER_MESH_FUNC_LEAVE()                               \
                                                                            \
    X(emitter)->frozen = /* freeze (if we don't generate any velocity) */   \
    X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_NONE;        \
                                                                            \
    /* TODO: move velocity generation into its own func, for code size */   \
    float n[3], u[3], v[3], velgen_scale = X(emitter)->velgen_scale;        \
                                                                            \
    ae_switch (X(emitter)->velgen_mode, gl_particle_emitter_velgen_mode,    \
                                GL_PARTICLE_EMITTER_VELGEN_MODE, suffix)    \
    {                                                                       \
        case GL_PARTICLE_EMITTER_VELGEN_MODE_NONE:                          \
        {                                                                   \
            vel = v_e; /* set to sentinel for the end-of-buffer assert */   \
        }                                                                   \
        break;                                                              \
                                                                            \
        case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL:                   \
        {                                                                   \
            for (size_t i = 0; i < index_count; i += 3)                     \
            {                                                               \
                GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();                    \
                                                                            \
                tri3face_normal(n, v0, v1, v2);                             \
                vec3scale(n, n, velgen_scale);                              \
                                                                            \
                for (size_t j = 0; j < particles_per_triangle;              \
                                            j += 3, vel += 9)               \
                {                                                           \
                    vec3copy(vel + 0, n);                                   \
                    vec3copy(vel + 3, n);                                   \
                    vec3copy(vel + 6, n);                                   \
                }                                                           \
            }                                                               \
        }                                                                   \
        break;                                                              \
                                                                            \
        case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_RANDOM:            \
        {                                                                   \
            for (size_t i = 0; i < index_count; i += 3)                     \
            {                                                               \
                GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();                    \
                                                                            \
                tri3face_normal(n, v0, v1, v2);                             \
                vec3scale(n, n, velgen_scale);                              \
                                                                            \
                for (size_t j = 0; j < particles_per_triangle;              \
                                            j += 3, vel += 9)               \
                {                                                           \
                    vec3scale(vel + 0, n, ae_random_flt());                 \
                    vec3scale(vel + 3, n, ae_random_flt());                 \
                    vec3scale(vel + 6, n, ae_random_flt());                 \
                }                                                           \
            }                                                               \
        }                                                                   \
        break;                                                              \
                                                                            \
        case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_BI_RANDOM:         \
        {                                                                   \
            for (size_t i = 0; i < index_count; i += 3)                     \
            {                                                               \
                GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();                    \
                                                                            \
                tri3face_normal(n, v0, v1, v2);                             \
                vec3scale(n, n, velgen_scale);                              \
                                                                            \
                for (size_t j = 0; j < particles_per_triangle;              \
                                            j += 3, vel += 9)               \
                {                                                           \
                    vec3scale(vel + 0, n, ae_random_bi_flt());              \
                    vec3scale(vel + 3, n, ae_random_bi_flt());              \
                    vec3scale(vel + 6, n, ae_random_bi_flt());              \
                }                                                           \
            }                                                               \
        }                                                                   \
        break;                                                              \
                                                                            \
        case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_RANDOM:   \
        {                                                                   \
            for (size_t i = 0; i < index_count; i += 3)                     \
            {                                                               \
                GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();                    \
                                                                            \
                tri3face_normal(n, v0, v1, v2);                             \
                vec3scale(n, n, velgen_scale * ae_random_flt());            \
                                                                            \
                for (size_t j = 0; j < particles_per_triangle;              \
                                            j += 3, vel += 9)               \
                {                                                           \
                    vec3copy(vel + 0, n);                                   \
                    vec3copy(vel + 3, n);                                   \
                    vec3copy(vel + 6, n);                                   \
                }                                                           \
            }                                                               \
        }                                                                   \
        break;                                                              \
                                                                            \
        case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_BI_RANDOM:\
        {                                                                   \
            for (size_t i = 0; i < index_count; i += 3)                     \
            {                                                               \
                GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();                    \
                                                                            \
                tri3face_normal(n, v0, v1, v2);                             \
                vec3scale(n, n, velgen_scale * ae_random_bi_flt());         \
                                                                            \
                for (size_t j = 0; j < particles_per_triangle;              \
                                            j += 3, vel += 9)               \
                {                                                           \
                    vec3copy(vel + 0, n);                                   \
                    vec3copy(vel + 3, n);                                   \
                    vec3copy(vel + 6, n);                                   \
                }                                                           \
            }                                                               \
        }                                                                   \
        break;                                                              \
                                                                            \
        case GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM:                        \
        {                                                                   \
            for (size_t i = 0; i < index_count; i += 3)                     \
            {                                                               \
                GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();                    \
                                                                            \
                for (size_t j = 0; j < particles_per_triangle;              \
                                            j += 3, vel += 9)               \
                {                                                           \
                    vel[0] = ae_random_bi_flt();                            \
                    vel[1] = ae_random_bi_flt();                            \
                    vel[2] = ae_random_bi_flt();                            \
                    vec3scale(vel + 0, vel + 0, velgen_scale);              \
                                                                            \
                    vel[3] = ae_random_bi_flt();                            \
                    vel[4] = ae_random_bi_flt();                            \
                    vel[5] = ae_random_bi_flt();                            \
                    vec3scale(vel + 3, vel + 3, velgen_scale);              \
                                                                            \
                    vel[6] = ae_random_bi_flt();                            \
                    vel[7] = ae_random_bi_flt();                            \
                    vel[8] = ae_random_bi_flt();                            \
                    vec3scale(vel + 6, vel + 6, velgen_scale);              \
                }                                                           \
            }                                                               \
        }                                                                   \
        break;                                                              \
                                                                            \
        case GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO:                          \
        {                                                                   \
            memset(vel, 0, particles_per_triangle * index_count *           \
                                                sizeof( float ));           \
                                                                            \
            /* increment for end-of-buffer assertion */                     \
            vel += particles_per_triangle * index_count;                    \
        }                                                                   \
        break;                                                              \
                                                                            \
        default:                                                            \
        {                                                                   \
            ae_assert(0, "velgen mode: %u", (u32) X(emitter)->velgen_mode); \
        }                                                                   \
        break;                                                              \
    }                                                                       \
                                                                            \
    /* restore the position and velocity arrays to the emitter */           \
    gl_particle_emitter_unlock(emitter, &vel_array, &particles);            \
                                                                            \
    /* free temporary identity or converted index buffer */                 \
    ae_stack_free(ae_global_stack(), indices, index_bytes);                 \
                                                                            \
    ae_assert(pos == p_e, "position %p end pointer %p", pos, p_e);          \
    ae_assert(vel == v_e, "velocity %p end pointer %p", vel, v_e);          \

void gl_particle_emitter_mesh_cover_face(gl_particle_emitter_t* emitter,
                    gl_buffer_t* buffer, size_t particles_per_triangle)
{
    GL_PARTICLE_EMITTER_MESH_FUNC_ENTER();

    for (size_t i = 0; i < index_count; i += 3)
    {
        GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();

        for (size_t j = 0; j < particles_per_triangle; j += 3, pos += 9)
        {
            ae_random_tri3_point(pos + 0, v0, v1, v2);
            ae_random_tri3_point(pos + 3, v2, v0, v1);
            ae_random_tri3_point(pos + 6, v1, v2, v0);
        }
    }

    GL_PARTICLE_EMITTER_MESH_FUNC_LEAVE();
}

void gl_particle_emitter_mesh_same_edges(gl_particle_emitter_t* emitter,
                    gl_buffer_t* buffer, size_t particles_per_triangle)
{
    GL_PARTICLE_EMITTER_MESH_FUNC_ENTER();

    for (size_t i = 0; i < index_count; i += 3)
    {
        GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();

        for (size_t j = 0; j < particles_per_triangle; j += 3, pos += 9)
        {
            ae_random_tri3_edge_index(pos + 0, v0, v1, v2, 0);
            ae_random_tri3_edge_index(pos + 3, v0, v1, v2, 1);
            ae_random_tri3_edge_index(pos + 6, v0, v1, v2, 2);
        }
    }

    GL_PARTICLE_EMITTER_MESH_FUNC_LEAVE();
}

void gl_particle_emitter_mesh_rand_edges(gl_particle_emitter_t* emitter,
                    gl_buffer_t* buffer, size_t particles_per_triangle)
{
    GL_PARTICLE_EMITTER_MESH_FUNC_ENTER();

    for (size_t i = 0; i < index_count; i += 3)
    {
        GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();

        for (size_t j = 0; j < particles_per_triangle; j += 3, pos += 9)
        {
            ae_random_tri3_edge(pos + 0, v0, v1, v2);
            ae_random_tri3_edge(pos + 3, v2, v0, v1);
            ae_random_tri3_edge(pos + 6, v1, v2, v0);
        }
    }

    GL_PARTICLE_EMITTER_MESH_FUNC_LEAVE();
}

void gl_particle_emitter_mesh_given_edge(gl_particle_emitter_t * emitter,
        gl_buffer_t* buffer, size_t particles_per_triangle, size_t index)
{
    GL_PARTICLE_EMITTER_MESH_FUNC_ENTER();

    for (size_t i = 0; i < index_count; i += 3)
    {
        GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();

        for (size_t j = 0; j < particles_per_triangle; j += 3, pos += 9)
        {
            ae_random_tri3_edge_index(pos + 0, v0, v1, v2, index);
            ae_random_tri3_edge_index(pos + 3, v0, v1, v2, index);
            ae_random_tri3_edge_index(pos + 6, v0, v1, v2, index);
        }
    }

    GL_PARTICLE_EMITTER_MESH_FUNC_LEAVE();
}

void gl_particle_emitter_mesh_rad_vertex(gl_particle_emitter_t * emitter,
        gl_buffer_t* buffer, size_t particles_per_triangle, float radius)
{
    GL_PARTICLE_EMITTER_MESH_FUNC_ENTER();

    if (particles_per_triangle == 3 && ae_fabsf(radius) < GL_DEFAULT_EPSILON)
    {
        for (size_t i = 0; i < index_count; i += 3, pos += 9)
        {
            GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();

            vec3copy(pos + 0, v0);
            vec3copy(pos + 3, v1);
            vec3copy(pos + 6, v2);
        }
    }
    else if (ae_fabsf(radius) < GL_DEFAULT_EPSILON)
    {
        for (size_t i = 0; i < index_count; i += 3)
        {
            GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();

            for (size_t j = 0; j < particles_per_triangle; j += 3, pos += 9)
            {
                vec3copy(pos + 0, v0);
                vec3copy(pos + 3, v1);
                vec3copy(pos + 6, v2);
            }
        }
    }
    else if (ae_fabsf(radius - 1.0f) < GL_DEFAULT_EPSILON)
    {
        for (size_t i = 0; i < index_count; i += 3)
        {
            GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();

            for (size_t j = 0; j < particles_per_triangle; j += 3, pos += 9)
            {
                pos[0] = v0[0] + ae_random_bi_flt();
                pos[1] = v0[1] + ae_random_bi_flt();
                pos[2] = v0[2] + ae_random_bi_flt();

                pos[3] = v1[0] + ae_random_bi_flt();
                pos[4] = v1[1] + ae_random_bi_flt();
                pos[5] = v1[2] + ae_random_bi_flt();

                pos[6] = v2[0] + ae_random_bi_flt();
                pos[7] = v2[1] + ae_random_bi_flt();
                pos[8] = v2[2] + ae_random_bi_flt();
            }
        }
    }
    else
    {
        for (size_t i = 0; i < index_count; i += 3)
        {
            GL_PARTICLE_EMITTER_MESH_ASSIGN_VERTS();

            for (size_t j = 0; j < particles_per_triangle; j += 3, pos += 9)
            {
                pos[0] = v0[0] + ae_random_bi_flt() * radius;
                pos[1] = v0[1] + ae_random_bi_flt() * radius;
                pos[2] = v0[2] + ae_random_bi_flt() * radius;

                pos[3] = v1[0] + ae_random_bi_flt() * radius;
                pos[4] = v1[1] + ae_random_bi_flt() * radius;
                pos[5] = v1[2] + ae_random_bi_flt() * radius;

                pos[6] = v2[0] + ae_random_bi_flt() * radius;
                pos[7] = v2[1] + ae_random_bi_flt() * radius;
                pos[8] = v2[2] + ae_random_bi_flt() * radius;
            }
        }
    }

    GL_PARTICLE_EMITTER_MESH_FUNC_LEAVE();
}

void gl_particle_emitter_reset_all(void)
{
    GL_PROFILE_SCOPE();

    aeTempArray<gl_particle_emitter_t*> emitters(gl_particle_emitter_count_all());
    gl_particle_emitter_list_all(emitters.data());

    for (size_t i = 0, n = gl_particle_emitter_count_all(); i < n; i++)
    {
        gl_particle_emitter_reset(emitters[i]);
    }
}

void gl_particle_emitter_reset(gl_particle_emitter_t* emitter)
{
    if (gl_particle_emitter_get_open(emitter))
    {
        assert(!X(emitter)->locked);

        X(emitter)->num_particles = 0;
        X(emitter)->current_index = 0;

        X(emitter)->time = 0.0;
    }
}

void gl_particle_emitter_fill_all(void)
{
    GL_PROFILE_SCOPE();

    aeTempArray<gl_particle_emitter_t*> emitters(gl_particle_emitter_count_all());
    gl_particle_emitter_list_all(emitters.data());

    for (size_t i = 0, n = gl_particle_emitter_count_all(); i < n; i++)
    {
        gl_particle_emitter_fill(emitters[i]);
    }
}

void gl_particle_emitter_fill(gl_particle_emitter_t* emitter)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        // TODO: while this should protect us from an infinite loop,
        // we should still add a timeout counter to this function...

        if (X(emitter)->frozen || X(emitter)->max_particles == 0)
        {
            return;
        }

        while (X(emitter)->num_particles < X(emitter)->max_particles)
        {
            gl_particle_emitter_update(emitter, X(emitter)->period);
        }
    }
}

void gl_particle_emitter_update_all(double dt)
{
    GL_PROFILE_SCOPE();

    aeTempArray<gl_particle_emitter_t*> emitters(gl_particle_emitter_count_all());
    gl_particle_emitter_list_all(emitters.data());

    for (size_t i = 0, n = gl_particle_emitter_count_all(); i < n; i++)
    {
        gl_particle_emitter_update(emitters[i], dt);
    }
}

static void
gl_particle_emitter_generate_particles(gl_internal_particle_emitter_t* data, double dt)
{
    GL_PROFILE_SCOPE();

    /* "cache" these as locals so we only have pay to to dereference the structure once
     */
    const float* const pos_sigma = const_cast<const float* const>(data->position_sigma);
    const float* const pos = const_cast<const float* const>(data->position);

    const float* const vel_sigma = const_cast<const float* const>(data->velocity_sigma);
    const float* const vel = const_cast<const float* const>(data->velocity);

    while (data->time > data->period)
    {
        data->time -= data->period;

        for (int i = 0; i < data->burst_size; i++)
        {
            if (data->num_particles != data->max_particles) data->num_particles += 1;
            if (data->current_index >= data->max_particles) data->current_index  = 0;

            float* curr_p = ((float*)data->particles.data) + data->current_index * 3;
            float* curr_v = ((float*)data->vel_array.data) + data->current_index * 3;

            /* TODO: allow selection of different random particle generation algorithms
             */
            curr_p[0] = ae_random_gauss_flt(pos[0], pos_sigma[0]);
            curr_p[1] = ae_random_gauss_flt(pos[1], pos_sigma[1]);
            curr_p[2] = ae_random_gauss_flt(pos[2], pos_sigma[2]);

            curr_v[0] = ae_random_gauss_flt(vel[0], vel_sigma[0]);
            curr_v[1] = ae_random_gauss_flt(vel[1], vel_sigma[1]);
            curr_v[2] = ae_random_gauss_flt(vel[2], vel_sigma[2]);

            data->current_index++;
        }
    }
}

static void
gl_particle_emitter_move_particles(gl_internal_particle_emitter_t* data, double dt)
{
    GL_PROFILE_SCOPE();

    ae_if (!vec3is_zero_e(data->gravity, GL_DEFAULT_EPSILON))
    {
        float scaled[3]; // scale gravity for time
        vec3mul_flt(scaled, data->gravity, dt);

        vtx_vec3add((float*)data->vel_array.data, scaled, data->num_particles * 3);
    }

    vtx_madd( // TODO: fold add + scale into one loop
            (float*)data->particles.data,

            (const float* const)data->particles.data,
            (const float* const)data->vel_array.data,

            dt, data->num_particles * 3);
}

void gl_particle_emitter_update(gl_particle_emitter_t* emitter, double dt)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        // TODO: assert that particles and vel_array sizes match
        // TODO: assert that num_particles <= max_particles

        if (X(emitter)->frozen || X(emitter)->max_particles == 0)
        {
            return;
        }

        X(emitter)->time += dt;

        gl_particle_emitter_generate_particles(X(emitter), dt);
        gl_particle_emitter_move_particles(X(emitter), dt);
    }
}

void gl_particle_emitter_draw_all(void)
{
    GL_PROFILE_SCOPE();

    aeTempArray<gl_particle_emitter_t*> emitters(gl_particle_emitter_count_all());
    gl_particle_emitter_list_all(emitters.data());

    for (size_t i = 0, n = gl_particle_emitter_count_all(); i < n; i++)
    {
        gl_particle_emitter_draw(emitters[i]);
    }
}

void gl_particle_emitter_draw(gl_particle_emitter_t* emitter)
{
    GL_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked); // don't issue GL commands on empty emitters
        ae_if (X(emitter)->num_particles == 0) { return; }

        /* FIXME: point sprites are rendered upside-down, so we'll need to push a
         * texture matrix to set the origin to the upper-left corner temporarily.
         * in future implementations, this will of course be done inside a shader.
         *
         * FIXME: on some drivers i've tested this on, particles look super wonky,
         * growing and shrinking to ridiculous sizes in seemingly random ways...
         * we should probably check for some missing extension and try a fallback.
         * my initial suspicion is something involving PointParameter* functions.
         *
         * after some further testing on my linux machine, this bizarre flickering
         * behavior also seems to be affecting line rendering mode... it appears
         * that any opengl usage beyond the beaten path is subject to glitchiness.
         */
        GL_DepthMask(GL_FALSE);
        {
            GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
            {
                GL_Color4fv(X(emitter)->color);
                GL_Disable(GL_LIGHTING);

                GL_Enable(GL_BLEND); // TODO: premultiplied alpha?
                GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                GL_PointSize(X(emitter)->max_point_size);
                gl_texture_bind(X(emitter)->texture);

                GL_PointParameterf(GL_POINT_SIZE_MIN, X(emitter)->min_point_size);

                float attenuation[3] = { 0.0f, 0.0f, 0.01f }; // TODO: property
                GL_PointParameterfv(GL_POINT_DISTANCE_ATTENUATION, attenuation);

                ae_if (gl_texture_get_open(X(emitter)->texture))
                {
                    GL_TexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
                    GL_Enable(GL_POINT_SPRITE);
                }

                GL_PushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
                {
                    GL_VertexPointer(3, GL_FLOAT, 0, X(emitter)->particles.data);
                    GL_EnableClientState(GL_VERTEX_ARRAY);
                    GL_DrawArrays(GL_POINTS, 0, X(emitter)->num_particles);
                }
                GL_PopClientAttrib();
            }
            GL_PopAttrib();
        }
        GL_DepthMask(GL_TRUE);

        // TODO: GL_DEBUG, draw only if it's #defined, color/enabled props
        #if 0
        {
            static const float * c = ae_color_flt[ae_random_bounded_u32(
                                                        AE_COLOR_COUNT)];
            gl_particle_emitter_draw_aabbox(emitter, c);
        }
        #endif

        // TODO: GL_DEBUG, draw only if it's #defined, color/enabled props
        #if 0
        {
            static const u32 s = ae_random_bounded_u32(AE_COLOR_COUNT);
            static const u32 e = ae_random_bounded_u32(AE_COLOR_COUNT);

            gl_particle_emitter_draw_velocity_ex(emitter,
                                ae_color_flt[s], ae_color_flt[e], 1.0f);
        }
        #endif

        gl_debug_log_error_state();
    }
}

void gl_particle_emitter_draw_aabbox_ex(gl_particle_emitter_t * emitter,
                                    const float* rgba, float line_width)
{
    GL_PROFILE_SCOPE();

    if (gl_particle_emitter_get_num_particles(emitter) > 0)
    {
        float minv[3];
        float maxv[3];

        gl_particle_emitter_get_aabbox(emitter, minv, maxv);
        gl_aabbox_ex(minv, maxv, rgba, line_width);
    }
}

void gl_particle_emitter_draw_aabbox(gl_particle_emitter_t* emitter,
                                                const float * rgba)
{
    gl_particle_emitter_draw_aabbox_ex(emitter, rgba, 1.0f);
}

void gl_particle_emitter_draw_velocity_ex( gl_particle_emitter_t * emitter,
        const float* start_color, const float* end_color, float line_width)
{
    GL_PROFILE_SCOPE();

    if (gl_particle_emitter_get_open(emitter))
    {
        assert(!X(emitter)->locked); // don't issue GL commands on empty emitters
        ae_if (X(emitter)->num_particles == 0) { return; }

        GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT);
        {
            GL_Disable(GL_TEXTURE_2D);
            GL_Disable(GL_LIGHTING);

            GL_LineWidth(line_width);

            GL_Enable(GL_BLEND); // default alpha blending mode
            GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            GL_Begin(GL_LINES);
            {
                for (int i = 0; i < X(emitter)->num_particles * 3; i += 3)
                {
                    float* p = ((float*)X(emitter)->particles.data) + i;
                    float* v = ((float*)X(emitter)->vel_array.data) + i;

                    GL_Color4fv(start_color);
                    GL_Vertex3fv(p);

                    GL_Color4fv(end_color);
                    GL_Vertex3f(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
                }
            }
            GL_End();
        }
        GL_PopAttrib();

        gl_debug_log_error_state();
    }
}

void gl_particle_emitter_draw_velocity(gl_particle_emitter_t* emitter,
                                                    const float* rgba)
{
    gl_particle_emitter_draw_velocity_ex(emitter, rgba, rgba, 1.0f);
}

#undef X

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_objects_init(void)
{
    #define N(cap, low) /* create the lists we use to track objects */  \
                                                                        \
        ae_assert(memiszero(&gl_ ## low ## _lst, sizeof(ae_ptrset_t)),  \
                            "the %s set is already initialized",#low);  \
                                                                        \
        ae_ptrset_init(&gl_ ## low ## _lst, 16);                        \

    GL_OBJECT_TYPE_N
    #undef N

    /* log a few different structure sizes for easier heap tracking.
     */
    #define N(cap, low) ae_log(MISC, "gl_%s_t:\t%u bytes", \
            #low, (u32)sizeof(gl_internal_ ## low ## _t));

    GL_OBJECT_TYPE_N
    #undef N
}

void gl_objects_quit(void)
{
    gl_object_close_all();

    #define N(cap, low) /* free and zero out object lists */            \
                                                                        \
        ae_assert(gl_ ## low ## _count_all() == 0, "%s leaked!", #low); \
                                                                        \
        ae_ptrset_free(&gl_ ## low ## _lst);                            \
        memset(&gl_ ## low ## _lst, 0, sizeof(ae_ptrset_t));            \

    GL_OBJECT_TYPE_N
    #undef N

    gl_internal_texture_t::default_copy = 0;
}

#undef GL_PROFILE_SCOPE
#undef GL_PROFILE_NAMED
