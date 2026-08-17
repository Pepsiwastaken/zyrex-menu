#pragma once

#include <windows.h>
#include <string>

namespace AutoUpdater
{
    // Downloads and applies offsets from https://offsets.imtheo.lol/
    // If `version` is non-empty, tries the version-specific dump first
    // (https://offsets.imtheo.lol/offsets/<version>/Offsets.hpp), then falls
    // back to the latest dump. Returns number of offsets updated.
    int update_offsets(const std::string& version = "");

    // Determines the running Roblox version from the process handle (if given)
    // and fetches matching offsets (with latest fallback).
    void update_all(HANDLE process_handle = nullptr);

    // Compares the running Roblox version against the offsets' version and logs a warning on mismatch
    void validate_version(HANDLE process_handle);
}
