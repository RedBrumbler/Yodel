#ifndef LINUX_TEST

#include <jni.h>
#include <string>
#include "_config.h"
#include "internal-loader.hpp"
#include "log.h"
#include "modloader.h"
#include "protect.hpp"

namespace {
std::string application_id;
std::filesystem::path modloader_path;
std::filesystem::path modloader_root_load_path;
std::filesystem::path modloader_source_path;
std::filesystem::path files_dir;
std::filesystem::path external_dir;
std::filesystem::path libr15Path;
bool failed = false;

using namespace std::literals::string_view_literals;
constexpr std::string_view libil2cppName = "libr15-original.so"sv;

}  // namespace

#pragma region modloader_api

namespace modloader {

MODLOADER_EXPORT std::filesystem::path const& get_modloader_path() noexcept {
  return modloader_path;
}

MODLOADER_EXPORT std::filesystem::path const& get_modloader_root_load_path() noexcept {
  return modloader_root_load_path;
}

MODLOADER_EXPORT std::filesystem::path const& get_files_dir() noexcept {
  return files_dir;
}

MODLOADER_EXPORT std::filesystem::path const& get_external_dir() noexcept {
  return external_dir;
}

MODLOADER_EXPORT std::string const& get_application_id() noexcept {
  return application_id;
}

MODLOADER_EXPORT std::filesystem::path const& get_modloader_source_path() noexcept {
  return modloader_source_path;
}

MODLOADER_EXPORT std::filesystem::path const& get_libr15_path() noexcept {
  return libr15Path;
}

}  // namespace modloader

// Exposed C API
MODLOADER_FUNC bool modloader_get_failed() {
  return failed;
}
MODLOADER_FUNC char const* modloader_get_path() {
  return modloader_path.c_str();
}
MODLOADER_FUNC char const* modloader_get_root_load_path() {
  return modloader_root_load_path.c_str();
}
MODLOADER_FUNC char const* modloader_get_files_dir() {
  return files_dir.c_str();
}
MODLOADER_FUNC char const* modloader_get_external_dir() {
  return external_dir.c_str();
}
MODLOADER_FUNC char const* modloader_get_application_id() {
  return application_id.c_str();
}
MODLOADER_FUNC char const* modloader_get_source_path() {
  return modloader_source_path.c_str();
}
MODLOADER_FUNC char const* modloader_get_libr15_path() {
  return libr15Path.c_str();
}

#pragma endregion  // modloader_api

// Entry points for the modloader here
MODLOADER_FUNC void modloader_preload(JNIEnv* env, char const* appId, char const* modloaderPath,
                                      char const* modloaderSourcePath, char const* filesDir,
                                      char const* externalDir) noexcept {
  LOG_INFO("Yodelaheehoo! Built from branch " GIT_BRANCH " ({:X}) modified: {} by user {}", GIT_COMMIT,
           (bool)GIT_MODIFIED, GIT_USER);

  LOG_DEBUG("modloader_preload called with:");
  LOG_DEBUG("env:                  {}", fmt::ptr(env));
  LOG_DEBUG("appId:                {}", appId);
  LOG_DEBUG("modloaderPath:        {}", modloaderPath);
  LOG_DEBUG("modloaderSourcePath:  {}", modloaderSourcePath);
  LOG_DEBUG("filesDir:             {}", filesDir);
  LOG_DEBUG("externalDir:          {}", externalDir);

  application_id = appId;
  modloader_path = modloaderPath;
  modloader_source_path = modloaderSourcePath;
  files_dir = filesDir;
  modloader_root_load_path = std::filesystem::path(modloaderSourcePath).parent_path();
  external_dir = externalDir;

  if (env->GetJavaVM(&modloader_jvm) != 0) {
    LOG_WARN("Failed to get JavaVM! Be careful when using it!");
  }

  if (!modloader::copy_all(files_dir)) {
    LOG_FATAL("Failed to copy over files! Modloading cannot continue!");
    failed = true;
  }
}

MODLOADER_FUNC void modloader_load(JNIEnv* env, char const* soDir) noexcept {
  LOG_DEBUG("modloader_load called with:");
  LOG_DEBUG("env:   {}", fmt::ptr(env));
  LOG_DEBUG("soDir: {}", soDir);

  // Copy over soDir
  libr15Path = soDir;
  libr15Path = libr15Path / libil2cppName;
  if (failed) {
    LOG_FATAL("Not loading mods because we failed!");
    return;
  }
  // dlopen all libs and dlopen early mods, call setup
  modloader::open_libs(files_dir);
  modloader::open_early_mods(files_dir);
}

MODLOADER_FUNC void modloader_accept_r15_handle(JNIEnv* env, void* r15Handle) noexcept {
  LOG_DEBUG("modloader_accept_r15_handle called with:");
  LOG_DEBUG("env:        {}", fmt::ptr(env));
  LOG_DEBUG("r15Handle:  {}", fmt::ptr(r15Handle));

  modloader_r15_handle = r15Handle;
  if (failed) {
    LOG_FATAL("Not loading mods because we failed!");
    return;
  }
  modloader::protect_all();

  modloader::load_early_mods();
  // TODO: make some kind of hook to invoke a late load call, possibly somewhere after components are loaded within
  // echovr?
}

MODLOADER_FUNC void modloader_unload(JavaVM* vm) noexcept {
  LOG_DEBUG("modloader_unload called with:");
  LOG_DEBUG("vm:         {}", fmt::ptr(vm));
  if (failed) {
    LOG_FATAL("Not unloading mods because we failed!");
    return;
  }
  // dlclose all opened mods, uninstall all hooks
  modloader::close_all();
}
#endif
