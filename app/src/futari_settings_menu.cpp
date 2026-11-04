// futari-linux (2026-11-04): ported from cerroverb/espgaluda2-recomp src/settings_menu.cpp (same owner, commit 2284d7b).

#include "futari_settings_menu.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <rex/cvar.h>
#include <rex/logging.h>
#include <rex/ui/overlay/settings_overlay.h>

namespace futari::settings_menu {

namespace {

// The settings shown in the window, by name, with their category there. Each was checked in a run on Wayland
// (2026-11-04). Left out because they change nothing here: input_rumble (the game asked for no vibration through two
// credits of play, bomb, deaths and game over) and keybind_guide (guide_button is off). video_driver stays hidden: SDL
// picks the driver.
const std::unordered_map<std::string_view, std::string_view> kShown = {
    // Display
    {"fullscreen", "Futari/Display"},
    {"window_width", "Futari/Display"},
    {"window_height", "Futari/Display"},
    {"monitor", "Futari/Display"},
    {"renut_resolution", "Futari/Display"},
    {"renut_vsync", "Futari/Display"},
    {"futari_fill_crop", "Futari/Display"},
    {"renut_anisotropic_filtering", "Futari/Display"},
    // Input lag and frame timing
    {"futari_late_input", "Futari/Latency"},
    {"futari_vblank_display_sync", "Futari/Latency"},
    {"futari_vblank_phase_auto", "Futari/Latency"},
    {"futari_vblank_phase_max_us", "Futari/Latency"},
    {"futari_vblank_delay_us", "Futari/Latency"},
    {"futari_vblank_wait_shown", "Futari/Latency"},
    {"futari_renut_double_buffer", "Futari/Latency"},
    // Game
    {"futari_dlc_dir", "Futari/Game"},
    // Audio
    {"audio_mute", "Futari/Audio"},
    {"audio_maxqframes", "Futari/Audio"},
    // Keyboard
    {"mnk_mode", "Futari/Keyboard"},
    // Logs
    {"log_level", "Futari/Logs"},
    // Program key bindings (the SDK's bind_debug_overlay, bind_console and bind_achievements stay hidden)
    {"bind_settings", "Futari/Keybinds/System"},
    {"bind_title_exit", "Futari/Keybinds/System"},
};

// Tooltips that describe the Switch build or the code's history: this build's wording.
const std::unordered_map<std::string_view, std::string_view> kDescriptions = {
    {"renut_vsync", "Show each frame at the display's refresh: no tearing, a little more input lag. Off shows frames "
                    "at once, with tearing."},
    {"futari_vblank_display_sync", "The game's vertical blank follows the display's own refresh (when the presents "
                                   "are shown, VK_KHR_present_wait) instead of a 60 Hz timer of its own."},
    {"futari_renut_double_buffer", "The swapchain with 2 images (one image less queued before the display) instead "
                                   "of 3."},
};

// The keyboard's controller bindings (keybind_*) move as a group.
constexpr std::string_view kControllerKeybinds = "Input/Keybinds/Controller";

}  // namespace

void Apply() {
  size_t shown = 0;
  std::vector<std::string> hidden;
  for (rex::cvar::FlagEntry& entry : rex::cvar::GetRegistry()) {
    if (const auto it = kShown.find(entry.name); it != kShown.end()) {
      entry.category = std::string(it->second);
      if (const auto text = kDescriptions.find(entry.name); text != kDescriptions.end()) {
        entry.description = std::string(text->second);
      }
      // Read once at startup. Init-only settings are greyed out in the window and refuse a change: a restart lets
      // the player set them there, save, and start again. audio_maxqframes (audio_system.cpp) and futari_dlc_dir
      // (futari_dlc.cpp) are read once as well, though marked live.
      if (entry.lifecycle == rex::cvar::Lifecycle::kInitOnly || entry.name == "audio_maxqframes" ||
          entry.name == "futari_dlc_dir") {
        entry.lifecycle = rex::cvar::Lifecycle::kRequiresRestart;
      }
      ++shown;
    } else if (entry.category == kControllerKeybinds && entry.name != "keybind_guide") {
      entry.category = "Futari/Keybinds/Controller";
      ++shown;
    } else {
      hidden.push_back(entry.name);
    }
  }
  REXLOG_INFO("settings window: {} settings shown, {} hidden (src/futari_settings_menu.cpp)", shown, hidden.size());
  rex::ui::HideSettingsInMenu(hidden);
}

}  // namespace futari::settings_menu
