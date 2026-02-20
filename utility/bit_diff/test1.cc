#include <iostream>
#include <iomanip>
#include <string>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>
#include <sstream>

#include "format_with_dispatch.hh"

template <class UInt>
std::string bit_diff_bitmap(UInt oldv, UInt newv,
                            bool msb_first = true,
                            std::size_t group = 0,   // e.g. 4 for nibble grouping, 0 disables
                            char sep = '\'')         // separator between groups
{
  static_assert(std::is_integral<UInt>::value, "UInt must be an integral type");
  static_assert(std::is_unsigned<UInt>::value, "UInt must be an unsigned type");
  constexpr std::size_t W = std::numeric_limits<UInt>::digits;
  // Bits that changed from 0->1 and 1->0
  const UInt up   = static_cast<UInt>(~oldv) & newv;          // 0 -> 1
  const UInt down = oldv & static_cast<UInt>(~newv);          // 1 -> 0 (not strictly needed below)
  const UInt chg  = oldv ^ newv;                              // changed (either direction)
  std::string out;
  out.reserve(W + (group ? (W - 1) / group : 0));
  auto emit_bit = [&](std::size_t i) -> char {
    const UInt mask = static_cast<UInt>(UInt(1) << i);
    if ((chg & mask) == 0) return '.';
    return (up & mask) ? '1' : '0';
  };
  if (msb_first) {
    for (std::size_t k = 0; k < W; ++k) {
      const std::size_t i = W - 1 - k; // MSB -> LSB
      out.push_back(emit_bit(i));
      if (group && (k + 1) < W && ((k + 1) % group == 0)) out.push_back(sep);
    }
  } else {
    for (std::size_t i = 0; i < W; ++i) { // LSB -> MSB
      out.push_back(emit_bit(i));
      if (group && (i + 1) < W && ((i + 1) % group == 0)) out.push_back(sep);
    }
  }
  return out;
}

// Format set-bit indices (LSB=0) as comma-separated ranges, e.g. "0-2,5,7-9".
inline std::string format_index_ranges(const std::size_t* idx, std::size_t n,
                                       const char* empty_token = "-")
{
    if (n == 0) return empty_token ? std::string(empty_token) : std::string{};

    std::string out;
    // Rough reserve: each index maybe up to 3-4 chars + punctuation; cheap heuristic
    out.reserve(n * 6);

    std::size_t i = 0;
    while (i < n) {
        std::size_t start = idx[i];
        std::size_t end   = start;

        while (i + 1 < n && idx[i + 1] == end + 1) {
            ++i;
            end = idx[i];
        }

        if (!out.empty()) out.push_back(',');

        out += std::to_string(start);
        if (end != start) {
            out.push_back('-');
            out += std::to_string(end);
        }

        ++i;
    }
    return out;
}

template <class UInt>
std::pair<std::string, std::string>
bit_flip_index_ranges(UInt oldv, UInt newv,
                      const char* empty_token = "-") // what to print if no flips
{
    static_assert(std::is_integral<UInt>::value, "UInt must be an integral type");
    static_assert(std::is_unsigned<UInt>::value, "UInt must be an unsigned type");

    constexpr std::size_t W = std::numeric_limits<UInt>::digits;

    const UInt up   = static_cast<UInt>(~oldv) & newv;          // 0 -> 1
    const UInt down = oldv & static_cast<UInt>(~newv);          // 1 -> 0

    // Collect indices (LSB=0). Use fixed-size arrays (no allocations).
    std::size_t up_idx[W];
    std::size_t dn_idx[W];
    std::size_t up_n = 0, dn_n = 0;

    for (std::size_t i = 0; i < W; ++i) {
        const UInt mask = static_cast<UInt>(UInt(1) << i);
        if (up & mask)   up_idx[up_n++] = i;
        if (down & mask) dn_idx[dn_n++] = i;
    }

    return {
        format_index_ranges(up_idx, up_n, empty_token),
        format_index_ranges(dn_idx, dn_n, empty_token)
    };
}
int main()
{
  std::cout << "Hello world!" << std::endl;
  FormatDispatch d;

  using value_t = uint32_t;
  constexpr int WIDTH_DATA = 32;

  value_t c = 0x64231122; // current
  value_t p = 0x54231233; // previous

  d['b'] = [&c, &p](std::string_view f) { // bitset view
    return bit_diff_bitmap(p, c, true, 8);
  };
  d['c'] = [&c, &p](std::string_view f) { // change list view
    auto [up, down] = bit_flip_index_ranges(p, c);
    return "0:" + down + " 1:" + up;
  };
  d['h'] = [&c, &p](std::string_view f) { // hew view
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0') << std::setw(WIDTH_DATA/4) << p;
    ss << " -> 0x"<< std::hex << std::setfill('0') << std::setw(WIDTH_DATA/4) << c; 
    return ss.str();
  };

  std::cout << format_with_dispatch("%b %c %h", d) << std::endl;
}
