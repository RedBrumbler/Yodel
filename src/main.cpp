#include <jni.h>
#include "log.hpp"

extern "C" void modloader_preload(JNIEnv* env, char const* appId, char const* modloaderPath,
                                  char const* modloaderSourcePath, char const* filesDir,
                                  char const* externalDir) noexcept {
  LOG_INFO("modloader_preload called with:");
  LOG_INFO("env:                  %p", env);
  LOG_INFO("appId:                %s", appId);
  LOG_INFO("modloaderPath:        %s", modloaderPath);
  LOG_INFO("modloaderSourcePath:  %s", modloaderSourcePath);
  LOG_INFO("filesDir:             %s", filesDir);
  LOG_INFO("externalDir:          %s", externalDir);
}

extern "C" void modloader_load(JNIEnv* env, char const* soDir) noexcept {
  LOG_INFO("modloader_load called with:");
  LOG_INFO("env:   %p", env);
  LOG_INFO("soDir: %s", soDir);
}

extern "C" void modloader_accept_r15_handle(JNIEnv* env, void* r15Handle) noexcept {
  LOG_INFO("modloader_accept_r15_handle called with:");
  LOG_INFO("env:        %p", env);
  LOG_INFO("r15Handle:  %p", r15Handle);
}

extern "C" void modloader_unload(JavaVM* vm) noexcept {
  LOG_INFO("modloader_unload called with:");
  LOG_INFO("vm:         %p", vm);
}
