#include <chrono>
#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>

namespace durfmt {

struct Unit {
    __int128 ps;          // unit size in picoseconds
    const char* suffix;   // unit suffix
};

// Convert signed __int128 to decimal string (non-negative expected here).
inline std::string to_string_u128(__int128 v) {
    if (v <= 0) return "0";
    std::string s;
    while (v > 0) {
        int digit = int(v % 10);
        s.push_back(char('0' + digit));
        v /= 10;
    }
    std::reverse(s.begin(), s.end());
    return s;
}

inline std::string join_terms(const std::vector<std::string>& terms) {
    if (terms.empty()) return {};
    std::string out = terms.front();
    for (std::size_t i = 1; i < terms.size(); ++i) {
        out.push_back(' ');
        out += terms[i];
    }
    return out;
}

template <class Rep, class Period>
std::string format_human(std::chrono::duration<Rep, Period> d) {
    using namespace std::chrono;

    // Represent everything in picoseconds using a wide integer rep.
    using ps128 = duration<__int128, std::pico>;
    ps128 ps = duration_cast<ps128>(d);

    if (ps.count() < 0) {
        throw std::invalid_argument("format_human: negative durations are not supported");
    }
    if (ps.count() == 0) return "0s";

    static constexpr __int128 PS_PER_NS  = 1'000;
    static constexpr __int128 PS_PER_US  = 1'000 * PS_PER_NS;     // 1e6
    static constexpr __int128 PS_PER_MS  = 1'000 * PS_PER_US;     // 1e9
    static constexpr __int128 PS_PER_S   = 1'000 * PS_PER_MS;     // 1e12
    static constexpr __int128 PS_PER_MIN = 60   * PS_PER_S;
    static constexpr __int128 PS_PER_H   = 60   * PS_PER_MIN;
    static constexpr __int128 PS_PER_D   = 24   * PS_PER_H;

    static constexpr Unit UNITS[] = {
        { PS_PER_D,   "d"   },
        { PS_PER_H,   "h"   },
        { PS_PER_MIN, "min" },
        { PS_PER_S,   "s"   },
        { PS_PER_MS,  "ms"  },
        { PS_PER_US,  "us"  }, // ASCII; swap to "µs" if you prefer Unicode
        { PS_PER_NS,  "ns"  },
        { 1,          "ps"  }
    };

    __int128 rem = ps.count();
    std::vector<std::string> terms;
    terms.reserve(std::size(UNITS));

    for (const auto& u : UNITS) {
        __int128 q = rem / u.ps;
        rem %= u.ps;
        if (q != 0) {
            terms.push_back(to_string_u128(q) + u.suffix);
        }
    }

    // rem should be 0 because ps is the finest unit we keep.
    return join_terms(terms);
}

} // namespace durfmt

// --- Example ---
// #include <iostream>
// int main() {
//     using namespace std::chrono;
//     std::cout << durfmt::format_human( 3*24h + 1min + 3s ) << "\n"; // "3d 1min 3s"
//     std::cout << durfmt::format_human( 60s ) << "\n";              // "1min"
//     std::cout << durfmt::format_human( 1500ms ) << "\n";           // "1s 500ms"
//     std::cout << durfmt::format_human( duration<long long, std::pico>(12) ) << "\n"; // "12ps"
// }

