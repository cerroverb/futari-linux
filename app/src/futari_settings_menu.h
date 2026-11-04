// futari-linux (2026-11-04): ported from cerroverb/espgaluda2-recomp src/settings_menu.h (same owner, commit 2284d7b).
// The F4 settings window lists every setting compiled into the program, most of them from the code this port reuses
// (the SDK's emulated GPU, banjo-nx's Switch renderer tuning, futari-nx). Apply() moves the settings that apply to
// futari-linux into the "Futari" category and leaves every other one out of the window (HideSettingsInMenu). Names do
// not change: config files and launch options work as before.

#pragma once

namespace futari::settings_menu {

// Call once every setting and key binding is registered (OnPostSetup).
void Apply();

}  // namespace futari::settings_menu
