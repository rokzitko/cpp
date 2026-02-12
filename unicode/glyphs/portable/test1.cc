#include <iostream>
#include <string_view>

struct utf8 {
    std::string_view bytes;
};

inline std::ostream& operator<<(std::ostream& os, utf8 s) {
    return os.write(s.bytes.data(), static_cast<std::streamsize>(s.bytes.size()));
}

// U+2191 ↑ and U+2193 ↓ encoded as UTF-8 bytes
inline constexpr utf8 uparrow   { "\xE2\x86\x91" };
inline constexpr utf8 downarrow { "\xE2\x86\x93" };

int main() {
    std::cout << "up=" << uparrow << " down=" << downarrow << "\n";
}

