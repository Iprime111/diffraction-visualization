#pragma once

#include <fmt/format.h>

#define DIFFRACTION_CRITICAL(...) throw std::runtime_error(fmt::format(__VA_ARGS__))

#define DIFFRACTION_NODISCARD [[nodiscard]]

namespace diffraction {
struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
}  // namespace diffraction
