#pragma once

#include <jni.h>
#include <stdint.h>
#include "_config.h"

#ifdef __cplusplus
#include <filesystem>
#include <string>

namespace modloader {

// For all functions here, see their corresponding C API functions for documentation
// These serve as a C++ friendly interface to avoid conversions and copies

MODLOADER_EXPORT std::filesystem::path const& get_modloader_path() noexcept;
MODLOADER_EXPORT std::filesystem::path const& get_modloader_root_load_path() noexcept;
MODLOADER_EXPORT std::filesystem::path const& get_files_dir() noexcept;
MODLOADER_EXPORT std::filesystem::path const& get_external_dir() noexcept;
MODLOADER_EXPORT std::string const& get_application_id() noexcept;
MODLOADER_EXPORT std::filesystem::path const& get_modloader_source_path() noexcept;
MODLOADER_EXPORT std::filesystem::path const& get_libr15_path() noexcept;

}  // namespace modloader

extern "C" {
#endif

typedef struct {
  char const* id;
  char const* version;
  uint64_t version_long;
} CModInfo;

typedef enum {
  MatchType_Strict,
  MatchType_IdOnly,
  MatchType_IdVersion,
  MatchType_IdVersionLong,
} CMatchType;

typedef struct {
  CModInfo info;
  char const* path;
  void* handle;
} CModResult;

typedef struct {
  CModResult* array;
  size_t size;
} CModResults;

#ifdef __cplusplus
}
#endif

/// @brief Returns true if the modloader failed to copy over the libs/mods to load, false otherwise
MODLOADER_FUNC bool modloader_get_failed();
/// @brief The captured pointer to the JavaVM
MODLOADER_EXPORT extern JavaVM* modloader_jvm;
/// @brief The captured dlopen-d libil2cpp.so handle
MODLOADER_EXPORT extern void* modloader_r15_handle;
/// @brief Returns the path of the modloader.
/// Example output: /data/user/0/com.readyatdawn.r15/files/libyodel.so
MODLOADER_FUNC char const* modloader_get_path();
/// @brief Returns the root load path for searches.
/// Example output: /sdcard/ModData/com.readyatdawn.r15/Modloader
MODLOADER_FUNC char const* modloader_get_root_load_path();
/// @brief Returns the path to the files directory
/// Example output: /data/user/0/com.readyatdawn.r15/files
MODLOADER_FUNC char const* modloader_get_files_dir();
/// @brief Returns the path to the external folder
/// Example output: /storage/emulated/0/Android/data/com.readyatdawn.r15/files
MODLOADER_FUNC char const* modloader_get_external_dir();
/// @brief Returns the application ID
/// Example output: com.readyatdawn.r15
MODLOADER_FUNC char const* modloader_get_application_id();
/// @brief Returns the path where the modloader was found from
/// Example output: /sdcard/ModData/com.readyatdawn.r15/Modloader/libyodel.so
MODLOADER_FUNC char const* modloader_get_source_path();
/// @brief Returns the path where libr15.so is located and dlopened from
/// Example output:
MODLOADER_FUNC char const* modloader_get_libr15_path();
/// @brief Triggers an unload of the specified mod, which will in turn call the unload() method of it.
/// It will also be removed from any collections. It is UB if the mod to be unloaded is the currently executing mod.
/// @return False if the mod failed to be unloaded in any way, true if it either did not exist or was successfully
/// unloaded.
// TODO: Make this not work for mods that have no unload method, instead returning a different response
// TODO: Also make sure we travel up our dependents and unload all of them
MODLOADER_FUNC bool modloader_force_unload(CModInfo info, CMatchType match_type);
/// @brief Returns an allocated array of CModResults for all successfully loaded objects.
MODLOADER_FUNC CModResults modloader_get_all();
/// @brief Frees a CModResults object
MODLOADER_FUNC void modloader_free_results(CModResults* results);
