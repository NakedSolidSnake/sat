/**
 * @file sat_plugin.h
 * @brief Dynamic library loading and symbol resolution interface
 * @author Cristiano Silva de Souza
 * @date 2025
 * 
 * This module provides a cross-platform interface for dynamically loading
 * shared libraries (plugins) and resolving symbols (functions, variables)
 * at runtime. It wraps the underlying platform-specific dynamic linking
 * mechanisms (dlopen/dlsym on Unix-like systems).
 */

#ifndef SAT_PLUGIN_H_
#define SAT_PLUGIN_H_

#include <sat_status.h>

/**
 * @brief Plugin handle structure
 * 
 * Opaque structure representing a loaded dynamic library.
 * Users should not access its fields directly.
 */
typedef struct 
{
    void *handle;  /**< Platform-specific library handle */
} sat_plugin_t;

/**
 * @brief Plugin configuration structure
 * 
 * Contains parameters for loading a dynamic library.
 */
typedef struct 
{
    char *library_name;  /**< Path to the shared library file */
} sat_plugin_args_t;

/**
 * @brief Opens and loads a dynamic library
 * 
 * Loads the specified shared library into the process's address space,
 * making its symbols available for resolution. The library path can be
 * absolute or relative, and the system's library search path is used
 * if needed.
 * 
 * @param[out] object Pointer to the plugin structure to initialize
 * @param[in] args Pointer to configuration containing the library path
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note On Linux, the library name typically has a .so extension
 * @note The library remains loaded until sat_plugin_close() is called
 * @warning The library must exist and be compatible with the current platform
 * @see sat_plugin_close()
 */
sat_status_t sat_plugin_open (sat_plugin_t *const object, const sat_plugin_args_t *const args);

/**
 * @brief Loads a symbol (function or variable) from the plugin
 * 
 * Resolves the address of a named symbol from the loaded library and
 * stores it in the provided pointer. This is typically used to obtain
 * function pointers from the plugin.
 * 
 * @param[in] object Pointer to the loaded plugin structure
 * @param[in] name Name of the symbol to resolve (e.g., function name)
 * @param[out] method Pointer to store the resolved symbol address
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @note The symbol name must exactly match the exported symbol in the library
 * @note For C++ libraries, symbols may be mangled; use extern "C" to avoid this
 * @warning The method pointer is only valid while the plugin remains loaded
 * @warning Casting the resolved pointer to the correct type is the caller's responsibility
 * @see sat_plugin_open()
 */
sat_status_t sat_plugin_load_method (sat_plugin_t *const object, const char *const name, void **const method);

/**
 * @brief Closes and unloads a dynamic library
 * 
 * Unloads the library from the process's address space and releases
 * associated resources. After calling this function, all symbols obtained
 * from the library become invalid.
 * 
 * @param[in,out] object Pointer to the plugin structure to close
 * @return sat_status_t indicating success or failure of the operation
 * 
 * @warning All function pointers and addresses obtained from the library
 *          must not be used after calling this function
 * @warning Calling this function multiple times on the same plugin may cause errors
 * @see sat_plugin_open()
 */
sat_status_t sat_plugin_close (sat_plugin_t *const object);

#endif/* SAT_PLUGIN_H_ */
