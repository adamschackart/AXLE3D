/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

/* TODO: ae_stream - opaque file-like objects similar to SDL's RWops, that follow
 * the conventions of XL objects (see pyxl/xl_core.h). Could also contain the
 * functionality of stdout and dynamic arrays, providing a unified safe interface
 * for all I/O. We could use this to implement ae_stdio.h functions (AE_FILE*).
 *
 * TODO: override the base path with something parsed from argv (-basepath="foo").
 * TODO: read and write text files for proper cross-platform newline handling.
 * TODO: functional equivalent to python's os.path.join (use variable arguments?)
 * TODO: enumerate files in a directory, with and without nested subdirectories.
 */
void ae_file_init(int argc, char** argv) {}
void ae_file_quit(void) {}

/*
================================================================================
 * ~~ [ filesystem utilities ] ~~ *
--------------------------------------------------------------------------------
*/

void ae_file_move(const char* oldname, const char* newname, int fatal) // mv
{
    if (rename(oldname, newname) == 0)
    {
        #if defined(AE_FILE_TRACE)
        ae_log(FILE, "successfully moved file \"%s\" to \"%s\"",
                                        oldname, newname );
        #endif
    }
    else
    {
        ae_log(FILE, "failed to move file \"%s\" to \"%s\"",
                                        oldname, newname );

        if (fatal)
        {
            ae_error("failed to move file \"%s\" to \"%s\"",
                                        oldname, newname );
        }
    }
}

void ae_file_copy(const char* oldname, const char* newname, int fatal) // cp
{
    size_t size;
    void * data = ae_file_read_stack(ae_global_stack(),
                                &size, oldname, fatal);

    ae_file_write(newname, data, size, fatal);
    ae_stack_free(ae_global_stack(), data, size);
}

void ae_file_remove(const char* filename, int fatal) // rm
{
    if (remove(filename) == 0)
    {
        #if defined(AE_FILE_TRACE)
        ae_log(FILE, "successfully removed file \"%s\"", filename);
        #endif
    }
    else
    {
        ae_log(FILE, "failed to remove file \"%s\"", filename);

        if (fatal)
        {
            ae_error("failed to remove file \"%s\"", filename);
        }
    }
}

/* ===== [ application paths ] ============================================== */

const char* ae_conf_path(const char* org, const char* app)
{
    // NOTE: we still create a new config path every time this is called
    AE_PROFILE_ENTER();

    // reserve 1k for the conf path, which can apparently be expensive to find
    static char ae_conf_path_string[1024 * 1];

    #if defined(_WIN32) // XXX TODO FIXME: this is a majorly bad hack
        void* SDL = ae_library_open("SDL2.dll");
    #else
        void* SDL = ae_library_open("libSDL2.so");
    #endif

    if (SDL)
    {
        typedef char* (AE_CALL* SDL_GetPrefPathFunc)(const char*, const char*);
        typedef void  (AE_CALL* SDL_FreeFunc)(void * mPtr);

        SDL_GetPrefPathFunc SDL2_GetPrefPath = (SDL_GetPrefPathFunc)
                            ae_library_proc(SDL, "SDL_GetPrefPath");

        SDL_FreeFunc SDL2_Free = (SDL_FreeFunc) \
                ae_library_proc(SDL, "SDL_free");

        if (SDL2_GetPrefPath && SDL2_Free)
        {
            char* conf_path = SDL2_GetPrefPath(org, app);

            if (conf_path)
            {
                strcpy(ae_conf_path_string, conf_path);
                SDL2_Free(conf_path);
            }
            else
            {
                AE_WARN("SDL failed to get application conf path!");
                // strcpy(ae_conf_path_string, ".");
            }
        }
        else
        {
            AE_WARN("linked with SDL lib, but not with functions!");
            // strcpy(ae_conf_path_string, ".");
        }

        ae_library_close(SDL);
    }
    else
    {
        AE_WARN("failed to find the application conf path string!");
        // strcpy(ae_conf_path_string, ".");
    }

    AE_PROFILE_LEAVE(); return ae_conf_path_string;
}

// reserve 1k for the base path, which can apparently be expensive to find
static char ae_base_path_string[1024 * 1];

const char* ae_base_path(void)
{
    if (ae_unlikely(ae_base_path_string[0] == '\0'))
    {
        AE_PROFILE_ENTER();

        #if defined(_WIN32) // XXX TODO FIXME: this is a majorly bad hack
            void* SDL = ae_library_open("SDL2.dll");
        #else
            void* SDL = ae_library_open("libSDL2.so");
        #endif

        if (SDL)
        {
            typedef char* (AE_CALL* SDL_GetBasePathFunc)(void);
            typedef void  (AE_CALL* SDL_FreeFunc)(void * mPtr);

            SDL_GetBasePathFunc SDL2_GetBasePath = (SDL_GetBasePathFunc)
                                ae_library_proc(SDL, "SDL_GetBasePath");

            SDL_FreeFunc SDL2_Free = (SDL_FreeFunc) \
                    ae_library_proc(SDL, "SDL_free");

            if (SDL2_GetBasePath && SDL2_Free)
            {
                char* base_path = SDL2_GetBasePath();

                if (base_path)
                {
                    ae_base_path_override(base_path);
                    SDL2_Free(base_path);
                }
                else
                {
                    AE_WARN("SDL failed to get application base path!");
                    // ae_base_path_override(".");
                }
            }
            else
            {
                AE_WARN("linked with SDL lib, but not with functions!");
                // ae_base_path_override(".");
            }

            ae_library_close(SDL);
        }
        else
        {
            AE_WARN("failed to find the application base path string!");
            // ae_base_path_override(".");
        }

        AE_PROFILE_LEAVE();
    }

    return ae_base_path_string;
}

void ae_base_path_override(const char* name)
{
    ae_log(FILE, "application base path set to \"%s\"", name);
    strcpy(ae_base_path_string, name);
}

/*
================================================================================
 * ~~ [ path string utilities ] ~~ *
--------------------------------------------------------------------------------
*/

// toggle profiling these, as their average time is somewhere in the attoseconds
#if 0
    #define FN_PROFILE_ENTER AE_PROFILE_ENTER
    #define FN_PROFILE_LEAVE AE_PROFILE_LEAVE
#else
    static c_inline void* fn_profile_enter(void) { return NULL; }
    static c_inline void  fn_profile_leave(void* p) { /* nop */ }

    #define FN_PROFILE_ENTER() void* _fn_profile_context = fn_profile_enter()
    #define FN_PROFILE_LEAVE() fn_profile_leave(_fn_profile_context)
#endif

const char* ae_filename_from_path_ex(const char* path, size_t* path_length)
{
    FN_PROFILE_ENTER(); // "C:\User\x.y" => "x.y"
    char *s = (char*)path, *match = (char*)path;

    for (; *s; s++)
    {
        switch (*s)
        {
            case '\\': // win32 or unix path
            case '/' : match = s + 1; break;

            default: break;
        }
    }

    if ( path_length ) // write out length
        *path_length = (size_t)(s - path);

    FN_PROFILE_LEAVE(); return match;
}

const char* ae_filename_from_path(const char* path)
{
    return ae_filename_from_path_ex(path, NULL);
}

void ae_split_file_path(const char* path_and_name, char* path, char* name)
{
    size_t path_and_name_length;
    FN_PROFILE_ENTER();

    const char* name_and_extension = ae_filename_from_path_ex(
                        path_and_name, &path_and_name_length);

    if (name) strcpy(name, name_and_extension);
    if (path)
    {
        const // copy out the path characters and stamp the null terminator
        size_t path_length = (size_t)(name_and_extension - path_and_name);

        memcpy(path, path_and_name, path_length);
        path[path_length] = '\0';
    }

    FN_PROFILE_LEAVE();
}

const char* ae_extension_from_path_ex(const char* path, size_t* path_length)
{
    FN_PROFILE_ENTER(); // track calls made within frames

    const char* s = strchr(path, '.');
    s = s ? s + 1 : (const char*)path;

    if ( path_length ) // write out the full path length
    {
        *path_length = (size_t)( s - path ) + strlen(s);

        ae_assert(*path_length == strlen(path), "%u %u",
                (u32)(*path_length), (u32)strlen(path));
    }

    FN_PROFILE_LEAVE(); return s;
}

const char* ae_extension_from_path(const char* path)
{
    return ae_extension_from_path_ex(path, NULL);
}

void ae_split_file_extension(const char* path, char* name, char* extn)
{
    FN_PROFILE_ENTER();

    size_t path_length, name_length; // input size, and split size
    const char* e = ae_extension_from_path_ex(path, &path_length);

    if (extn) strcpy(extn, e);
    if (name)
    {
        name_length = (size_t)(e - path) - 1; // ignore ext period
        if (name_length == (size_t)-1) name_length = 0; // no ext?

        memcpy(name, path, name_length);
        name[name_length] = '\0'; // append null terminator at len
    }

    FN_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ reading whole binary files ] ~~ *
--------------------------------------------------------------------------------
*/

static void* ae_file_read_begin(const char* filename, int fatal, size_t* size)
{
    AE_PROFILE_ENTER();

    FILE* filehandle = fopen(filename, "rb");
    if (filehandle)
    {
        if (fseek(filehandle, 0, SEEK_END)) // seek for file size
        {
            ae_error("failed to seek file \"%s\"", filename);
        }

        *size = ftell(filehandle);
        rewind(filehandle);

        #if defined(AE_FILE_TRACE)
        ae_log(FILE, "successfully opened file \"%s\" (%u bytes)",
                                filename, (unsigned int)(*size));
        #endif
    }
    else
    {
        ae_log(FILE, "failed to open file \"%s\"", filename);

        if (fatal)
        {
            ae_error("failed to open file \"%s\"", filename);
        }

        *size = 0;
    }

    AE_PROFILE_LEAVE();
    return (void*)filehandle;
}

static void ae_file_read_end(void* filehandle, const char * filename, int fatal,
                            void * filememory, const size_t filesize)
{
    // keep track of how many times this is called on files that don't exist
    void* outer = ae_profile_enter(__FILE__, "ae_file_read_end_outer");

    ae_if (filehandle != NULL)
    {
        void* inner = ae_profile_enter(__FILE__, "ae_file_read_end_inner");

        if (fread(filememory, filesize, 1, (FILE*)filehandle) != 1)
        {
            ae_error("failed to read file \"%s\"!", filename);
        }

        if (fclose((FILE*)filehandle))
        {
            ae_error("failed to close file \"%s\"", filename);
        }

        #if defined(AE_FILE_TRACE)
        ae_log(FILE, "successfully closed file \"%s\" (%u bytes)",
                                filename, (unsigned int)filesize);
        #endif
        ae_profile_leave(inner);
    }

    ae_profile_leave(outer);
}

/* ===== [ public API ] ===================================================== */

size_t ae_file_bytes(const char* filename, int fatal)
{
    size_t size; // beginning the read just grabs the file's size
    void * filep = ae_file_read_begin(filename, fatal, &size);

    if (filep)
    {
        if (fclose((FILE *)filep)) // TODO: this is ugly and weak!
        {
            ae_error("failed to close file \"%s\"", filename);
        }

        #if defined(AE_FILE_TRACE)
        ae_log(FILE, "successfully closed file \"%s\" (%u bytes)",
                                    filename, (unsigned int)size);
        #endif
    }
    else
    {
        size = (size_t)-1;
    }

    return size;
}

int ae_file_exists(const char* filename)
{
    return ae_file_bytes(filename, 0) != (size_t)-1;
}

int ae_file_read_array(ae_array_t* array, const char* filename, int fatal)
{
    AE_PROFILE_ENTER(); // append the entire file contents to the array
    size_t filesize, oldsize = array->size;

    void * filehandle = ae_file_read_begin(filename, fatal, &filesize);
    ae_array_resize(array, array->size + filesize);

    ae_file_read_end(filehandle,
                filename, fatal, (u8*)array->data + oldsize, filesize);

    AE_PROFILE_LEAVE();
    return filehandle != NULL;
}

/* these are written to prevent zero-sized blocks from being allocated
 * when optional files aren't found (return a NULL pointer instead).
 */
#define AE_ENTER                                                    \
    /* AE_PROFILE_ENTER(); */                                       \
                                                                    \
    void* filehandle = ae_file_read_begin(filename, fatal, size);   \
    void* memory = NULL;                                            \
                                                                    \
    ae_if (filehandle != NULL)

#define AE_LEAVE                                                    \
    ae_file_read_end(filehandle, filename, fatal, memory, *size);   \
                                                                    \
    /* AE_PROFILE_LEAVE(); */                                       \
    return memory;

void* ae_file_read_block(ae_memory_chunk_t* chunk, size_t* size,
                                const char* filename, int fatal)
{
    AE_ENTER
    {
        if (*size <= chunk->block_size)
            memory = ae_block_malloc(chunk);
        else
            memory = ae_malloc(*size);
    }
    AE_LEAVE
}

void* ae_file_read_stack(ae_memory_stack_t* stack, size_t* size,
                                const char* filename, int fatal)
{
    AE_ENTER
    memory = ae_stack_malloc(stack, *size);
    AE_LEAVE
}

void* ae_file_read_heap(ae_memory_heap_t* heap, size_t* size,
                            const char* filename, int fatal)
{
    AE_ENTER
    memory = ae_heap_malloc(heap, *size);
    AE_LEAVE
}

void* ae_file_read_string(size_t* size, const char* filename, int fatal)
{
    AE_ENTER
    memory = ae_string_malloc(*size);
    AE_LEAVE
}

void* ae_file_read(size_t* size, const char* filename, int fatal)
{
    AE_ENTER
    memory = ae_malloc(*size);
    AE_LEAVE
}

#undef AE_ENTER
#undef AE_LEAVE

/*
================================================================================
 * ~~ [ writing whole binary files ] ~~ *
--------------------------------------------------------------------------------
*/

int ae_file_write(const char* filename, void* contents, size_t size, int fatal)
{
    void* outer = ae_profile_enter(__FILE__, "ae_file_write_outer");
    FILE* wfile = fopen(filename, "wb"); // overwrites old contents

    if (wfile)
    {
        // see how many files we open for writes actually succeed in profile
        void* inner = ae_profile_enter(__FILE__, "ae_file_write_inner");

        #if defined(AE_FILE_TRACE)
        ae_log(FILE, "successfully opened file \"%s\" for writing %u bytes",
                                            filename, (unsigned int) size);
        #endif

        if (contents && size) // passing no data just creates an empty file
        if (fwrite(contents, size, 1, wfile) != 1)
        {
            ae_error("failed to write file \"%s\"", filename);
        }

        if (fclose(wfile))
        {
            ae_error("failed to close file \"%s\"", filename);
        }

        #if defined(AE_FILE_TRACE)
        ae_log(FILE, "successfully closed file \"%s\" for writing %u bytes",
                                            filename, (unsigned int) size);
        #endif
        ae_profile_leave(inner);
    }
    else
    {
        ae_log(FILE, "failed to open file \"%s\" for writing %u bytes",
                                        filename, (unsigned int) size);
        if (fatal)
        {
            ae_error("failed to open file \"%s\" for writing %u bytes",
                                        filename, (unsigned int) size);
        }
    }

    ae_profile_leave(outer);
    return wfile != NULL;
}
