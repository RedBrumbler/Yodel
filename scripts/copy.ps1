

$root = "${PSScriptRoot}/.."
$build = "${root}/build"
$yodel = "${yodel}/libyodel.so"

& ${PSScriptRoot}/build.ps1

& adb push ${yodel} /sdcard/ModData/com.readyatdawn.r15/Modloader/libyodel.so
