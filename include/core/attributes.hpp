#pragma once

#define DIFFRACTION_NODISCARD [[nodiscard]]

namespace diffraction {
struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
} // namespace diffraction
