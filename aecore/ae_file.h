/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_FILE_H__
#define __AE_FILE_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_MEMORY_H__
#include <ae_memory.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_file_init(int argc, char** argv);
void ae_file_quit(void); // private init

/*
================================================================================
 * ~~ [ filesystem utilities ] ~~ *
--------------------------------------------------------------------------------
*/

// returns SIZE_MAX (0xFFFF...) if fatal=0 and the file doesn't exist.
AE_DECL size_t AE_CALL ae_file_bytes(const char* filename, int fatal);

// returns true if the file is present on the filesystem.
AE_DECL int AE_CALL ae_file_exists(const char* filename);

AE_DECL void AE_CALL ae_file_move(const char* oldname,
                    const char* newname, int fatal );

AE_DECL void AE_CALL ae_file_copy(const char* oldname,
                    const char* newname, int fatal );

AE_DECL void AE_CALL ae_file_remove(const char* filename, int fatal);

/* Get the only writeable application path, used for config files and savegames.
 * Note that this function creates the directory if it does not already exist.
 * Org is the name of the organization, and app is the name of the application.
 */
AE_DECL const char* AE_CALL ae_conf_path(const char* org, const char* app);

/* Get the read-only path where the main application resides.
 */
AE_DECL const char* AE_CALL ae_base_path(void);

/* Enforce a base path for cases where C main() is running Python or something
 * similar, so our code's entry point is most likely in a different location.
 */
AE_DECL void AE_CALL ae_base_path_override(const char* name);

/*
================================================================================
 * ~~ [ path string utilities ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL const char* AE_CALL // "C:\User\x.y" => "x.y", optional strlen
ae_filename_from_path_ex(const char* path, size_t* path_length);

AE_DECL const char* AE_CALL ae_filename_from_path(const char* path);

// "C:\User\x.y" -> "C:\User\" is copied to path, "x.y" is copied to name.
// path and name must be big enough to hold path_and_name, & can be NULL.
AE_DECL void AE_CALL
ae_split_file_path(const char* path_and_name, char* path, char* name);

AE_DECL const char* AE_CALL // "C:\User\x.y" => "y", optional strlen
ae_extension_from_path_ex(const char* path, size_t* path_length);

AE_DECL const char* AE_CALL ae_extension_from_path(const char* path);

// "C:\User\x.y" -> "C:\User\x" copied into name, "y" copied into extn.
// extn and name must be big enough to hold the path, and can be NULL.
AE_DECL void AE_CALL
ae_split_file_extension(const char* path, char* name, char* extn);

/*
================================================================================
 * ~~ [ reading whole binary files ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL ae_file_read_array(ae_array_t* array,
                        const char* filename, int fatal);

AE_DECL void* AE_CALL ae_file_read_stack(ae_memory_stack_t* stack,
                    size_t* size, const char* filename, int fatal);

AE_DECL void* AE_CALL ae_file_read_block(ae_memory_chunk_t* chunk,
                    size_t* size, const char* filename, int fatal);

AE_DECL void* AE_CALL ae_file_read_heap(ae_memory_heap_t* heap,
                size_t* size, const char* filename, int fatal);

AE_DECL void* AE_CALL ae_file_read_string(size_t * size,
                        const char* filename, int fatal);

AE_DECL void* AE_CALL ae_file_read(size_t* size,
                const char* filename, int fatal);

/*
================================================================================
 * ~~ [ writing whole binary files ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL ae_file_write(const char* filename,
                void* contents, size_t size, int fatal);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_FILE_H__ */
