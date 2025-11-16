/**
 * @file mytoml.h
 * @brief The Header of the Toml Parser/Emitter .
 * @details This header provides all public API, types, macros, and
 * configuration for the mytoml TOML parser library. It is compliant with [TOML
 * v1.0.0](https://toml.io/en/v1.0.0) and supports both C and C++ usage.
 * @author Sackey Ezekiel Etrue (djoezeke)
 * @date Mon 29 09:06:15 Sept GMT 2025
 * @version 0.1.0
 * @see https://www.github.com/djoezeke/mytoml
 * @copyright Copyright (c) 2025 Sackey Ezekiel Etrue
 *
 * MYTOML: What this Is
 *
 * DOCUMENTATION:
 *
 * FEATURES:
 *
 * UNICODE:
 *
 * FUTURE:
 *
 * ISSUES:
 *
 * NOTES:
 *
 * USAGE:
 *
 *    @code
 *      #include "mytoml.h"
 *    @endcode
 *
 * FAQS:
 *
 * TODO:
 *
 * HELP:
 *    - See links below.
 *    - Read top of mytoml.c for more details and comments.
 *
 *  Has only had a few tests run, may have issues.
 *
 *  If having issues compiling/linking/running raise an issue (https://github.com/djoezeke/mytoml/issues).
 *  Please post in https://github.com/djoezeke/mytoml/discussions if you cannot find a solution in resources above.
 *
 * RESOURCES:
 * - Homepage ................... https://github.com/djoezeke/mytoml
 * - Releases & changelog ....... https://github.com/djoezeke/mytoml/releases
 * - Issues & support ........... https://github.com/djoezeke/mytoml/issues
 *
 */

#ifndef DJOEZEKE_MYTOML_H

/**
 * SECTIONS: Index of this file
 *  [SECTION] Header mess
 *  [SECTION] Configurable macros
 *  [SECTION] Function Macros
 *  [SECTION] Platform Defines
 *  [SECTION] Compiler Defines
 *  [SECTION] Compiler Warnings
 *  [SECTION] Imports/Exports
 *  [SECTION] Data Structures
 *  [SECTION] C Only Functions
 *  [SECTION] C++ Only Classes
 */

#define DJOEZEKE_MYTOML_H

#ifndef MYTOML_SKIP_VERSION_CHECK
#if defined(MYTOML_VERSION_MAJOR) && defined(MYTOML_VERSION_MINOR) && \
    defined(MYTOML_VERSION_PATCH)
#if MYTOML_VERSION_MAJOR != 0 || MYTOML_VERSION_MINOR != 1 || \
    MYTOML_VERSION_PATCH != 0
#warning "Already included a different version of the library!"
#endif
#endif
#endif // MYTOML_SKIP_VERSION_CHECK

/**
 * @defgroup version Version Information
 * @brief Macros for library versioning.
 * @{
 */

/**
 * @def MYTOML_VERSION_MAJOR
 * @brief Major version number of the library.
 */
#define MYTOML_VERSION_MAJOR 0

/**
 * @def MYTOML_VERSION_MINOR
 * @brief Minor version number of the library.
 */
#define MYTOML_VERSION_MINOR 1

/**
 * @def MYTOML_VERSION_PATCH
 * @brief Patch version number of the library.
 */
#define MYTOML_VERSION_PATCH 0

/**
 * @def MYTOML_VERSION
 * @brief Library version string in the format @c "X.Y.Z",
 * where @c X is the major version number, @c Y is a minor version
 * number, and @c Z is the patch version number.
 */
#define MYTOML_VERSION "0.1.0"

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] Header mess
//-----------------------------------------------------------------------------

#include <stdbool.h> //
#include <stdio.h>   // for FILE

#include "../khash.h"

#ifdef __cplusplus

/** C++ Exclusive headers. */
#include <exception>
#include <iostream>

#endif //__cplusplus

#ifdef MYTOML_TESTS
#endif // MYTOML_TESTS

//-----------------------------------------------------------------------------
// [SECTION] Configurable Macros
//-----------------------------------------------------------------------------

/**
 * @def MYTOML_MAX_DATE_FORMAT
 * @brief Maximum length for date format strings.
 * @note Default is 64 [`2^6`].
 */
#define MYTOML_MAX_DATE_FORMAT 64

/**
 * @def MYTOML_MAX_ID_LENGTH
 * @brief Maximum length for TOML key identifiers.
 * @note Default is 256 [`2^8`].
 */
#define MYTOML_MAX_ID_LENGTH 256

/**
 * @def MYTOML_MAX_STRING_LENGTH
 * @brief Maximum length for TOML string values.
 * @note Default is 4096 [`2^12`].
 */
#define MYTOML_MAX_STRING_LENGTH 4096

/**
 * @def MYTOML_MAX_FILE_SIZE
 * @brief Maximum TOML file size in bytes.
 * @note Default is 1073741824 [`2^30`] (1GB).
 */
#define MYTOML_MAX_FILE_SIZE 1073741824

/**
 * @def MYTOML_MAX_NUM_LINES
 * @brief Maximum number of lines in a TOML file.
 * @note Default is 16777216 [`2^24`].
 */
#define MYTOML_MAX_NUM_LINES 16777216

/**
 * @def MYTOML_MAX_SUBKEYS
 * @brief Maximum number of subkeys per TOML key.
 * @note Default is 131072 [`2^17`].
 */
#define MYTOML_MAX_SUBKEYS 131072

/**
 * @def MYTOML_MAX_ARRAY_LENGTH
 * @brief Maximum length of TOML arrays.
 * @note Default is 131072 [`2^17`].
 */
#define MYTOML_MAX_ARRAY_LENGTH 131072

//-----------------------------------------------------------------------------
// [SECTION] Function Macros
//-----------------------------------------------------------------------------

/**
 * @brief Load and parse a TOML file or stdin.
 * @param[in] file Path to TOML file or NULL for stdin.
 * @return Pointer to root TomlKey object, or NULL on failure.
 * @note Frees memory with toml_free().
 * @see toml_free
 * Example usage:
 * @code
 * TomlKey *toml = toml_load("basic.toml");
 * if (toml == NULL) return 1;
 * toml_free(toml);
 * @endcode
 * @note This macro uses C11 _Generic to select the appropriate function based
 * on the type of `file`. If `file` is a `char *`, it calls
 * `toml_load_file_name()`. If `file` is a `FILE *`, it calls
 * `toml_load_file()`. This allows for flexible loading of TOML data from either
 * a file path or an open file stream using a single macro.
 * @warning The user must ensure that the types passed to the macro match the
 * expected types to avoid compilation errors. Improper usage may lead to
 * undefined behavior.
 * @warning The file must exist and be readable. If using a `FILE *`, the caller
 * is responsible for managing the lifetime of the FILE stream. Failure to do so
 * may result in resource leaks or crashes.
 *
 */
#define toml_load(file)                  \
  _Generic((file),                       \
      char *: toml_load_file_name,       \
      const char *: toml_load_file_name, \
      FILE *: toml_load_file)(file)

/**
 * @brief Dump TOML value or key to a file or stream.
 * @param[in] object TOML key or value to dump.
 * @param[in] file Output filename or FILE stream.
 * @see toml_key_dump_file_name, toml_key_dump_file, toml_value_dump_file_name,
 * toml_value_dump_file Example usage:
 * @code
 * TomlKey *toml = toml_load("config.toml");
 * if (toml == NULL) {
 *   fprintf(stderr, "Failed to load TOML file\n");
 *   return 1;
 * }
 * // Dump to stdout
 * toml_dump(toml, stdout);
 * // Dump to file
 * toml_dump(toml, "output.toml");
 * toml_free(toml);
 * @endcode
 * @note This macro uses C11 _Generic to select the appropriate function based
 * on the types of `object` and `file`. For `TomlKey *` objects, it calls either
 * `toml_key_dump_file_name()` or `toml_key_dump_file()` depending on whether
 * `file` is a `char *` filename or a `FILE *` stream. For `TomlValue *`
 * objects, it calls either `toml_value_dump_file_name()` or
 * `toml_value_dump_file()` similarly. This allows for flexible dumping of both
 * keys and values to either files or streams using a single macro.
 * @warning The user must ensure that the types passed to the macro match the
 * expected types to avoid compilation errors. Improper usage may lead to
 * undefined behavior.
 * @warning The file or stream must be valid and writable. The caller is
 * responsible for managing the lifetime of the FILE stream if used. Failure to
 * do so may result in resource leaks or crashes.
 */
#define toml_dump(object, file)                        \
  _Generic((object),                                   \
      TomlKey *: _Generic((file),                      \
              char *: toml_key_dump_file_name,         \
              const char *: toml_key_dump_file_name,   \
              FILE *: toml_key_dump_file),             \
      TomlValue *: _Generic((file),                    \
              char *: toml_value_dump_file_name,       \
              const char *: toml_value_dump_file_name, \
              FILE *: toml_value_dump_file, ))(object, file)

/**
 * @brief Serialize TOML value or key to a string.
 * @param[in] object TOML key or value to serialize.
 * @return Pointer to string buffer (must be freed by caller).
 * @see toml_key_dumps, toml_value_dumps
 * Example usage:
 * @code
 * TomlKey *toml = toml_load("basic.toml");
 * if (toml == NULL) return 1;
 * const char *Toml = toml_dumps(toml);
 * printf("%s\n", Toml);
 * free((void *)Toml);
 * toml_free(toml);
 * @endcode
 * @note This macro uses C11 _Generic to select the appropriate function based
 * on the type of `object`. For `TomlKey *`, it calls `toml_key_dumps()`, and
 * for `TomlValue *`, it calls `toml_value_dumps()`.
 * @warning The returned string must be freed by the caller to avoid memory
 * leaks.
 *
 */
#define toml_dumps(object)       \
  _Generic((object),             \
      TomlKey *: toml_key_dumps, \
      TomlValue *: toml_value_dumps)(object)

/**
 * @brief Dump TOML value or key to a buffer.
 * @param[in] object TOML key or value to dump.
 * @param[out] buffer Pointer to output buffer.
 * @param[out] size Size of output buffer.
 * @see toml_key_dump_buffer, toml_value_dump_buffer
 * Example usage:
 * @code
 * TomlKey *toml = toml_load("basic.toml");
 * if (toml == NULL) return 1;
 * char *buffer = "";
 * size_t size = 0;
 * toml_dump_buffer(toml, &buffer, &size);
 * printf("%s\n", buffer);
 * free(buffer);
 * toml_free(toml);
 * @endcode
 * @note This macro uses C11 _Generic to select the appropriate function based
 * on the type of `object`. For `TomlKey *`, it calls `toml_key_dump_buffer()`,
 * and for `TomlValue *`, it calls `toml_value_dump_buffer()`.
 * @warning The user must ensure that the types passed to the macro match the
 * expected types to avoid compilation errors. Improper usage may lead to
 * undefined behavior.
 * @warning The buffer must be managed by the caller. The caller is responsible
 * for freeing the buffer to avoid memory leaks.
 *
 */
#define toml_dump_buffer(object, buffer, size) \
  _Generic((object),                           \
      TomlKey *: toml_key_dump_buffer,         \
      TomlValue *: toml_value_dump_buffer)(object, buffer, size)

//-----------------------------------------------------------------------------
// [SECTION] Platform
//-----------------------------------------------------------------------------

/**
 * @defgroup platform Platform Definitions
 * @{
 */

/**
 * @brief   Checks if the compiler is of given brand.
 * @param   name Platform, like `APPLE`.
 * @retval  true   It is.
 * @retval  false  It isn't.
 */
#define MYTOML_PLATFORM_IS(name) MYTOML_PLATFORM_IS_##name

/**
 * @brief  Returns the current platform name.
 * @return  platform name.
 */

#ifdef __APPLE__
/**
 * A preprocessor macro that is only defined if compiling for MacOS.
 */
#define MYTOML_PLATFORM_IS_APPLE 1
/**
 * @brief  Returns the current platform name.
 * @return  platform name.
 */
#define MYTOML_PLATFORM_NAME_IS "Apple"
#elif defined(linux) || defined(__linux) || defined(__linux__)
/**
 * A preprocessor macro that is only defined if compiling for Linux.
 */
#define MYTOML_PLATFORM_IS_LINUX 1
/**
 * @brief  Returns the current platform name.
 * @return  platform name.
 */
#define MYTOML_PLATFORM_NAME_IS "Linux"
#elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || \
    defined(_MSC_VER) || defined(__MINGW32__)
/**
 * A preprocessor macro that is only defined if compiling for Windows.
 */
#define MYTOML_PLATFORM_IS_WINDOWS 1
/**
 * @brief  Returns the current platform name.
 * @return  platform name.
 */
#define MYTOML_PLATFORM_NAME_IS "Windows"
#else
/**
 * A preprocessor macro that is only defined if compiling for others.
 */
#define MYTOML_PLATFORM_IS_OTHERS 1
/**
 * @brief  Returns the current platform name.
 * @return  platform name.
 */
#define MYTOML_PLATFORM_NAME_IS "Others"
#endif

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] Compiler
//-----------------------------------------------------------------------------

/**
 * @defgroup compiler Compiler Definitions
 * @{
 */

/**
 * @brief   Checks if the compiler is of given brand.
 * @param   name  Compiler brand, like `MSVC`.
 * @retval  true   It is.
 * @retval  false  It isn't.
 */
#define MYTOML_COMPILER_IS(name) MYTOML_COMPILER_IS_##name

/// Compiler is apple
#if !defined(__clang__)
#define MYTOML_COMPILER_IS_APPLE 0
#elif !defined(__apple_build_version__)
#define MYTOML_COMPILER_IS_APPLE 0
#else
#define MYTOML_COMPILER_IS_APPLE 1
#define MYTOML_COMPILER_VERSION_MAJOR __clang_major__
#define MYTOML_COMPILER_VERSION_MINOR __clang_minor__
#define MYTOML_COMPILER_VERSION_PATCH __clang_patchlevel__
#endif

/// Compiler is clang
#if !defined(__clang__)
#define MYTOML_COMPILER_IS_CLANG 0
#elif MYTOML_COMPILER_IS(APPLE)
#define MYTOML_COMPILER_IS_CLANG 0
#else
#define MYTOML_COMPILER_IS_CLANG 1
#define MYTOML_COMPILER_VERSION_MAJOR __clang_major__
#define MYTOML_COMPILER_VERSION_MINOR __clang_minor__
#define MYTOML_COMPILER_VERSION_PATCH __clang_patchlevel__
#endif

/// Compiler is intel
#if !defined(__INTEL_COMPILER)
#define MYTOML_COMPILER_IS_INTEL 0
#elif !defined(__INTEL_COMPILER_UPDATE)
#define MYTOML_COMPILER_IS_INTEL 1
/* __INTEL_COMPILER = XXYZ */
#define MYTOML_COMPILER_VERSION_MAJOR (__INTEL_COMPILER / 100)
#define MYTOML_COMPILER_VERSION_MINOR (__INTEL_COMPILER % 100 / 10)
#define MYTOML_COMPILER_VERSION_PATCH (__INTEL_COMPILER % 10)
#else
#define MYTOML_COMPILER_IS_INTEL 1
/* __INTEL_COMPILER = XXYZ */
#define MYTOML_COMPILER_VERSION_MAJOR (__INTEL_COMPILER / 100)
#define MYTOML_COMPILER_VERSION_MINOR (__INTEL_COMPILER % 100 / 10)
#define MYTOML_COMPILER_VERSION_PATCH __INTEL_COMPILER_UPDATE
#endif

/// Compiler is msc
#if !defined(_MSC_VER)
#define MYTOML_COMPILER_IS_MSVC 0
#elif MYTOML_COMPILER_IS(CLANG)
#define MYTOML_COMPILER_IS_MSVC 0
#elif MYTOML_COMPILER_IS(INTEL)
#define MYTOML_COMPILER_IS_MSVC 0
#elif _MSC_VER >= 1400
#define MYTOML_COMPILER_IS_MSVC 1
/* _MSC_FULL_VER = XXYYZZZZZ */
#define MYTOML_COMPILER_VERSION_MAJOR (_MSC_FULL_VER / 10000000)
#define MYTOML_COMPILER_VERSION_MINOR (_MSC_FULL_VER % 10000000 / 100000)
#define MYTOML_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
#elif defined(_MSC_FULL_VER)
#define MYTOML_COMPILER_IS_MSVC 1
/* _MSC_FULL_VER = XXYYZZZZ */
#define MYTOML_COMPILER_VERSION_MAJOR (_MSC_FULL_VER / 1000000)
#define MYTOML_COMPILER_VERSION_MINOR (_MSC_FULL_VER % 1000000 / 10000)
#define MYTOML_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 10000)
#else
#define MYTOML_COMPILER_IS_MSVC 1
/* _MSC_VER = XXYY */
#define MYTOML_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
#define MYTOML_COMPILER_VERSION_MINOR (_MSC_VER % 100)
#define MYTOML_COMPILER_VERSION_PATCH 0
#endif

/// Compiler is gcc
#if !defined(__GNUC__)
#define MYTOML_COMPILER_IS_GCC 0
#elif MYTOML_COMPILER_IS(APPLE)
#define MYTOML_COMPILER_IS_GCC 0
#elif MYTOML_COMPILER_IS(CLANG)
#define MYTOML_COMPILER_IS_GCC 0
#elif MYTOML_COMPILER_IS(INTEL)
#define MYTOML_COMPILER_IS_GCC 0
#else
#define MYTOML_COMPILER_IS_GCC 1
#define MYTOML_COMPILER_VERSION_MAJOR __GNUC__
#define MYTOML_COMPILER_VERSION_MINOR __GNUC_MINOR__
#define MYTOML_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
#endif

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] Warnings
//-----------------------------------------------------------------------------

/**
 * @defgroup compiler Compiler Warnings
 * @{
 */

#if MYTOML_COMPILER_IS(CLANG)
#define MYTOML_PRAGMA_TO_STR(x) _Pragma(#x)
#define MYTOML_CLANG_SUPPRESS_WARNING_PUSH _Pragma("clang diagnostic push")
#define MYTOML_CLANG_SUPPRESS_WARNING(w) \
  MYTOML_PRAGMA_TO_STR(clang diagnostic ignored w)
#define MYTOML_CLANG_SUPPRESS_WARNING_POP _Pragma("clang diagnostic pop")
#define MYTOML_CLANG_SUPPRESS_WARNING_WITH_PUSH(w) \
  MYTOML_CLANG_SUPPRESS_WARNING_PUSH MYTOML_CLANG_SUPPRESS_WARNING(w)
#else // MYTOML_CLANG
#define MYTOML_CLANG_SUPPRESS_WARNING_PUSH
#define MYTOML_CLANG_SUPPRESS_WARNING(w)
#define MYTOML_CLANG_SUPPRESS_WARNING_POP
#define MYTOML_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // MYTOML_CLANG

#if MYTOML_COMPILER_IS(GCC)
#define MYTOML_PRAGMA_TO_STR(x) _Pragma(#x)
#define MYTOML_GCC_SUPPRESS_WARNING_PUSH _Pragma("GCC diagnostic push")
#define MYTOML_GCC_SUPPRESS_WARNING(w) \
  MYTOML_PRAGMA_TO_STR(GCC diagnostic ignored w)
#define MYTOML_GCC_SUPPRESS_WARNING_POP _Pragma("GCC diagnostic pop")
#define MYTOML_GCC_SUPPRESS_WARNING_WITH_PUSH(w) \
  MYTOML_GCC_SUPPRESS_WARNING_PUSH MYTOML_GCC_SUPPRESS_WARNING(w)
#else // MYTOML_GCC
#define MYTOML_GCC_SUPPRESS_WARNING_PUSH
#define MYTOML_GCC_SUPPRESS_WARNING(w)
#define MYTOML_GCC_SUPPRESS_WARNING_POP
#define MYTOML_GCC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // MYTOML_GCC

#if MYTOML_COMPILER_IS(MSVC)
#define MYTOML_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
#define MYTOML_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
#define MYTOML_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
#define MYTOML_MSVC_SUPPRESS_WARNING_WITH_PUSH(w) \
  MYTOML_MSVC_SUPPRESS_WARNING_PUSH MYTOML_MSVC_SUPPRESS_WARNING(w)
#else // MYTOML_MSVC
#define MYTOML_MSVC_SUPPRESS_WARNING_PUSH
#define MYTOML_MSVC_SUPPRESS_WARNING(w)
#define MYTOML_MSVC_SUPPRESS_WARNING_POP
#define MYTOML_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // MYTOML_MSVC

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] Import/Export
//-----------------------------------------------------------------------------

/**
 * @defgroup export Export Definitions
 * @{
 */

#if defined(_WIN32)
#define MYTOML_NO_EXPORT_ATTR
#define MYTOML_API_EXPORT __declspec(dllexport)
#define MYTOML_API_IMPORT __declspec(dllimport)
#define MYTOML_DEPRECATED_ATTR __declspec(deprecated)
#else // _WIN32
#define MYTOML_API_EXPORT __attribute__((visibility("default")))
#define MYTOML_API_IMPORT __attribute__((visibility("default")))
#define MYTOML_NO_EXPORT_ATTR __attribute__((visibility("hidden")))
#define MYTOML_DEPRECATED_ATTR __attribute__((__deprecated__))
#endif // _WIN32

/**
 * @def MYTOML_API
 * @brief Macro for public API symbol export/import.
 * @details Use this macro to annotate all public API functions for correct
 * symbol visibility on all platforms.
 */

#if defined(MYTOML_BUILD_STATIC)
#define MYTOML_API
#elif defined(MYTOML_BUILD_SHARED) || defined(MYTOML_EXPORTS)
/* We are building this library */
#define MYTOML_API MYTOML_API_EXPORT
#elif defined(MYTOML_LOAD_SHARED) || defined(MYTOML_IMPORTS)
/* We are using this library */
#define MYTOML_API MYTOML_API_IMPORT
#else // MYTOML_BUILD_STATIC
#define MYTOML_API
#endif // MYTOML_BUILD_STATIC

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// [SECTION] Data Structures
//-----------------------------------------------------------------------------

/**
 * @defgroup basic Basic Types
 * @brief Core types and data structures for TOML.
 * @{
 */

/**
 * @enum TomlValueType
 * @brief Enumerates all TOML value types supported by the parser.
 * @details Used to distinguish between integer, boolean, float, array, string,
 * datetime, and table types as defined in the TOML specification.
 */
typedef enum TomlValueType_t
{
  TOML_INT,          /**< Integer value type. */
  TOML_BOOL,         /**< Boolean value type. */
  TOML_FLOAT,        /**< Floating-point value type. */
  TOML_ARRAY,        /**< Array value type. */
  TOML_STRING,       /**< String value type. */
  TOML_DATETIME,     /**< Datetime value type (RFC 3339). */
  TOML_DATELOCAL,    /**< Local date value type. */
  TOML_TIMELOCAL,    /**< Local time value type. */
  TOML_INLINETABLE,  /**< Inline table value type. */
  TOML_DATETIMELOCAL /**< Local datetime value type. */
} TomlValueType;

/**
 * @enum TomlKeyType
 * @brief Enumerates all TOML key types recognized by the parser.
 * @details Used to distinguish between root keys, tables, leaf keys, and array
 * tables for parsing and validation.
 */
typedef enum TomlKeyType_t
{
  TOML_KEY,       /**< Key in a key-value pair (e.g., j in j.k = v). */
  TOML_TABLE,     /**< Table key (e.g., a in [a.b]). */
  TOML_KEYLEAF,   /**< Leaf key in a key-value pair (e.g., k in j.k = v). */
  TOML_TABLELEAF, /**< Leaf table key (e.g., b in [a.b]). */
  TOML_ARRAYTABLE /**< Array table key (e.g., t in [[t]]). */
} TomlKeyType;

/**
 * @enum TomlErrorType
 * @brief Enumerates error types for TOML parsing.
 * @details Used to classify errors encountered during parsing or validation.
 */
typedef enum TomlErrorType
{
  /**
   * @name Toml error types
   * @{
   */

  TOML_UNKNOWN = -1, /**< An unknown error type. */
  TOML_DECODE,       /**< Cannot decode the input stream. */
  TOML_ENCODE,       /**< Cannot encode the input stream. */
  TOML_MEMORY,       /**< Cannot allocate or reallocate a block of memory. */
  TOML_READ,         /**< Cannot read from the input stream. */
  TOML_WRITE,        /**< Cannot write to the output stream. */
  TOML_CAST,         /**< Cannot cast value to type. */

  KEY_ALREADY_EXISTS,
  MISSING_SEPARATOR,
  WRONG_TYPE_CAST,
  MISSING_VALUE,
  KEY_NOT_FOUND,

  /** @} */

} TomlErrorType;

/**
 * @name TomlValue data type
 * @{
 */

/**
 * @struct TomlValue
 * @brief Represents a TOML value and its associated metadata.
 * @details Used to store any TOML value type, including arrays, numbers,
 * strings, and datetimes.
 */
typedef struct TomlValue_t TomlValue;
struct TomlValue_t
{
  TomlValueType type; /**< Type of TOML value. */
  TomlValue **arr;    /**< Array of TOML values (for TOML_ARRAY type). */
  int len;            /**< Length of array or value. */
  void *data;         /**< Pointer to value data (non-array types). */
  int precision;      /**< Numeric precision for floating-point values. */
  bool scientific;    /**< Whether to print numbers in scientific notation. */
  char
      format[MYTOML_MAX_DATE_FORMAT]; /**< Format string for datetime values. */
};

/** @} */

/**
 * @name TomlKey data type
 * @{
 */

/**
 * @struct TomlKey
 * @brief Represents a TOML key node in the parsed AST.
 * @details Each TOML key or table is represented as a TomlKey, with subkeys and
 * associated value.
 */
typedef struct TomlKey_t TomlKey;
KHASH_MAP_INIT_STR(str, TomlKey *)
struct TomlKey_t
{
  TomlKeyType type;              /**< Type of TOML key. */
  char id[MYTOML_MAX_ID_LENGTH]; /**< Key identifier string. */
  khash_t(str) * subkeys;        /**< Hash map of subkeys. */
  TomlValue *value;              /**< Value associated with this key. */
  size_t idx;                    /**< Index for array tables. */
};

/** @} */

/**
 * @name TomlError data type
 * @{
 */

/**
 * @struct TomlError
 * @brief Represents an error encountered.
 * @details Contains error type and message for diagnostics.
 */
typedef struct TomlError_t
{
  TomlErrorType type;  /**< Type of error. */
  const char *message; /**< Error message string. */
  int line;            /**< Line the error occured  */
  int column;          /**< Column the error occured  */
} TomlError_t;

/** @} */

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] C Only Functions
//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

  /**
   * @name Mytoml
   * C Functions
   * @{
   */

  /**
   * @brief Load and parse a TOML file from a filename.
   * @param[in] file Path to TOML file.
   * @return Pointer to root TomlKey object, or NULL on failure.
   * @note Frees memory with toml_free().
   * @see toml_free
   */
  MYTOML_API TomlKey *toml_load_file_name(char *file);

  /**
   * @brief Load and parse a TOML file from a FILE pointer.
   * @param[in] file FILE pointer to TOML file.
   * @return Pointer to root TomlKey object, or NULL on failure.
   * @note Frees memory with toml_free().
   * @see toml_free
   */
  MYTOML_API TomlKey *toml_load_file(FILE *file);

  /**
   * @brief Parse TOML from a string.
   * @param[in] toml TOML string to parse.
   * @return Pointer to root TomlKey object, or NULL on failure.
   * @note Frees memory with toml_free().
   * @see toml_free
   */
  MYTOML_API TomlKey *toml_loads(const char *toml);

  /**
   * @brief Dump TOML key to a FILE stream.
   * @param[in] object TOML key to dump.
   * @param[in] file Output FILE stream.
   * @warning The file must be valid and writable.
   * The caller is responsible for managing the lifetime of the FILE stream if
   * used. Failure to do so may result in resource leaks or crashes.
   */
  MYTOML_API void toml_key_dump_file(TomlKey *object, FILE *file);

  /**
   * @brief Dump TOML key to a file by filename.
   * @param[in] object TOML key to dump.
   * @param[in] file Output filename.
   */
  MYTOML_API void toml_key_dump_file_name(TomlKey *object, const char *file);

  /**
   * @brief Dump TOML value to a FILE stream.
   * @param[in] object TOML value to dump.
   * @param[in] file Output FILE stream.
   * @warning The file must be valid and writable.
   * The caller is responsible for managing the lifetime of the FILE stream if
   * used. Failure to do so may result in resource leaks or crashes.
   */
  MYTOML_API void toml_value_dump_file(TomlValue *object, FILE *file);

  /**
   * @brief Dump TOML value to a file by filename.
   * @param[in] object TOML value to dump.
   * @param[in] file Output filename.
   */
  MYTOML_API void toml_value_dump_file_name(TomlValue *object, const char *file);

  /**
   * @brief Serialize TOML key to a string.
   * @param[in] k TOML key to serialize.
   * @return Pointer to string buffer (must be freed by caller).
   * @warning The returned string must be freed by the caller to avoid memory
   * leaks.
   */
  MYTOML_API const char *toml_key_dumps(TomlKey *k);

  /**
   * @brief Serialize TOML value to a string.
   * @param[in] v TOML value to serialize.
   * @return Pointer to string buffer (must be freed by caller).
   * @warning The returned string must be freed by the caller to avoid memory
   * leaks.
   */
  MYTOML_API const char *toml_value_dumps(TomlValue *v);

  /**
   * @brief Dump TOML key to a buffer.
   * @param[in] k TOML key to dump.
   * @param[out] buffer Pointer to output buffer.
   * @param[out] size Size of output buffer.
   * @warning The buffer must be managed by the caller. The caller is responsible
   * for freeing the buffer to avoid memory leaks.
   */
  MYTOML_API void toml_key_dump_buffer(TomlKey *k, char **buffer, size_t *size);

  /**
   * @brief Dump TOML value to a buffer.
   * @param[in] v TOML value to dump.
   * @param[out] buffer Pointer to output buffer.
   * @param[out] size Size of output buffer.
   * @warning The buffer must be managed by the caller. The caller is responsible
   * for freeing the buffer to avoid memory leaks.     *
   */
  MYTOML_API void toml_value_dump_buffer(TomlValue *v, char **buffer,
                                         size_t *size);

  /**
   * @brief Dump TOML key as Toml to stdout.
   * @param[in] root Root TOML key to dump as Toml.
   */
  MYTOML_API void toml_key_dump(TomlKey *root);

  /**
   * @brief Free memory allocated for a TomlKey object and all its children.
   * @param[in] toml Pointer to TomlKey object to free.
   */
  MYTOML_API void toml_free(TomlKey *toml);

  /**
   * @brief Get integer value from TOML key.
   * @param[in] key TOML key to query.
   * @return Pointer to integer value, or NULL if not an integer.
   */
  MYTOML_API int *toml_get_int(TomlKey *key);

  /**
   * @brief Get boolean value from TOML key.
   * @param[in] key TOML key to query.
   * @return Pointer to boolean value, or NULL if not a boolean.
   */
  MYTOML_API bool *toml_get_bool(TomlKey *key);

  /**
   * @brief Get string value from TOML key.
   * @param[in] key TOML key to query.
   * @return Pointer to string value, or NULL if not a string.
   */
  MYTOML_API char *toml_get_string(TomlKey *key);

  /**
   * @brief Get floating-point value from TOML key.
   * @param[in] key TOML key to query.
   * @return Pointer to double value, or NULL if not a float.
   */
  MYTOML_API double *toml_get_float(TomlKey *key);

  /**
   * @brief Get array value from TOML key.
   * @param[in] key TOML key to query.
   * @return Pointer to TomlValue array, or NULL if not an array.
   */
  MYTOML_API TomlValue *toml_get_array(TomlKey *key);

  /**
   * @brief Get datetime value from TOML key.
   * @param[in] key TOML key to query.
   * @return Pointer to struct tm value, or NULL if not a datetime.
   */
  MYTOML_API struct tm *toml_get_datetime(TomlKey *key);

  /**
   * @brief Find a subkey by identifier.
   * @param[in] key TOML key to search.
   * @param[in] id Identifier string to match.
   * @return Pointer to matching TomlKey, or NULL if not found.
   */
  MYTOML_API TomlKey *toml_get_key(TomlKey *key, const char *id);

  /** @} */

#ifdef __cplusplus
}
#endif //__cplusplus

//-----------------------------------------------------------------------------
// [SECTION] C++ Only Classes
//-----------------------------------------------------------------------------

#ifdef __cplusplus
namespace mytoml
{

  // class Toml
  // {
  // public:
  //     Toml(TomlValue value);

  //     TomlValueType Type() const;

  //     bool IsArray() const;
  //     bool IsObject() const;
  //     bool IsString() const;
  //     bool IsNumber() const;
  //     bool IsBool() const;
  //     bool IsNone() const;

  //     Toml &operator=(const Toml &other);
  //     Toml &operator=(Toml &&other) noexcept;
  //     bool operator==(const Toml &other) const;
  //     bool operator!=(const Toml &other) const;

  //     Toml &operator[](const std::string &key);
  //     Toml &operator[](int index);

  //     std::string operator()(const std::string &key) const;
  //     std::string operator()(int index) const;
  // };

  // MYTOML_API void dump(Toml* toml, FILE * file);
  // MYTOML_API void dump(Toml* toml, char * file);
  // MYTOML_API char *dumps(Toml * toml);

  // MYTOML_API Toml load(FILE * file);
  // MYTOML_API Toml load(char * file);
  // MYTOML_API Toml loads(char* toml);

  // std::ostream &operator<<(std::ostream &os, const mytoml::Toml &toml);

  /**
   * @class TomlError
   * @brief TomlError class for Toml-related errors.
   */
  class TomlError : public std::exception
  {
  public:
    /**
     * @brief Constructs an exception with a specific error type.
     * @param type The type of the error.
     */
    TomlError(TomlError_t type);

    /**
     * @brief Gets the error message.
     * @return The error message.
     */
    const char *what() const noexcept override;

    /**
     * @brief Gets the error type.
     * @return The error type.
     */
    TomlErrorType type() const noexcept;

  private:
    TomlError_t m_Error; /**< The error type. */
  };

  /** Errors that occur during usage
   */
  class DecoderError : public TomlError
  {
  public:
    DecoderError(TomlError_t error) : TomlError(error) {};
    virtual ~DecoderError() {}
  };

} // namespace mytoml

#endif //__cplusplus

//-----------------------------------------------------------------------------
// [SECTION] Cleanup
//-----------------------------------------------------------------------------

#endif // DJOEZEKE_MYTOML_H

/**
 * LICENSE: MIT License
 *
 * Copyright (c) 2025 Sackey Ezekiel Etrue
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */