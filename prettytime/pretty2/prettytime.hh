#include <chrono>
#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>

namespace durfmt {

struct Unit { __int128 ps; const char* suffix; };

// ---- helpers ----
static inline std::string to_string_u128(__int128 v) { // v >= 0
  if (v == 0) return "0";
  std::string s;
  while (v > 0) {
    int digit = int(v % 10);
    s.push_back(char('0' + digit));
    v /= 10;
  }
  std::reverse(s.begin(), s.end());
  return s;
}

static inline std::string join_terms(const std::vector<std::string>& terms) {
  if (terms.empty()) return {};
  std::string out = terms.front();
  for (std::size_t i = 1; i < terms.size(); ++i) {
    out.push_back(' ');
    out += terms[i];
  }
  return out;
}

// Count decimal digits of non-negative __int128
static inline int digits10(__int128 x) {
  int d = 1;
  while (x >= 10) { x /= 10; ++d; }
  return d;
}

// “Round decimal” heuristic:
// true if x = m * 10^k with m in {1,2,5} (strongly round) or, if allow_any_single_digit=true, m in {1..9}.
static inline bool is_round_decimal(__int128 x, bool allow_any_single_digit = false) {
  if (x <= 0) return false;
  int tz = 0;
  __int128 t = x;
  while ((t % 10) == 0) { 
    t /= 10; ++tz;
  }
  // t is now x / 10^tz, not divisible by 10
  if (t == 1 || t == 2 || t == 5) return true;
  if (allow_any_single_digit && t >= 1 && t <= 9) return true;
  return false;
}

// Candidate with metadata for scoring
struct Candidate {
  std::string text;
  int terms = 0;
  bool first_count_round = false;
  int len = 0;
  int first_count_digits = 0;
};

// Build canonical decomposition: largest units first, skip zeros.
static inline Candidate canonical(__int128 ps, const Unit* units, int n_units) {
  Candidate c;
  __int128 rem = ps;
  std::vector<std::string> terms;
  terms.reserve(n_units);
  
  bool first = true;
  for (int i = 0; i < n_units; ++i) {
    __int128 q = rem / units[i].ps;
    rem %= units[i].ps;
    if (q != 0) {
      if (first) {
        c.first_count_round = is_round_decimal(q);
        c.first_count_digits = digits10(q);
        first = false;
      }
      terms.push_back(to_string_u128(q) + units[i].suffix);
    }
  }
  c.terms = int(terms.size());
  c.text = join_terms(terms);
  c.len = int(c.text.size());
  return c;
}

// Single-unit candidate if exact multiple.
static inline bool single_unit(__int128 ps, const Unit& u, Candidate& out) {
  if (ps % u.ps != 0) return false;
  __int128 q = ps / u.ps;
  out.text = to_string_u128(q) + u.suffix;
  out.terms = 1;
  out.first_count_round = is_round_decimal(q);
  out.first_count_digits = digits10(q);
  out.len = int(out.text.size());
  return true;
}

// Lexicographic “readability score”:
// 1) fewer terms (strong preference)
// 2) if terms==1, prefer round decimal counts (powers of 10 etc.)
// 3) shorter string
// 4) if still tied, fewer digits in the (single) count
static inline bool better(const Candidate& a, const Candidate& b) {
  if (a.terms != b.terms) return a.terms < b.terms;
  if (a.terms == 1 && b.terms == 1 && a.first_count_round != b.first_count_round)
    return a.first_count_round; // true beats false
  if (a.len != b.len) return a.len < b.len;
  return a.first_count_digits < b.first_count_digits;
}

// ---- main formatting function ----
template <class Rep, class Period>
std::string format_human(std::chrono::duration<Rep, Period> d) {
  using namespace std::chrono;

  using ps128 = duration<__int128, std::pico>;
  ps128 ps_dur = duration_cast<ps128>(d);
  __int128 ps = ps_dur.count();

  if (ps < 0) throw std::invalid_argument("format_human: negative durations not supported");
  if (ps == 0) return "0s";
  
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
        { PS_PER_US,  "us"  },
        { PS_PER_NS,  "ns"  },
        { 1,          "ps"  }
  };
  constexpr int N = int(sizeof(UNITS) / sizeof(UNITS[0]));

  // Start with canonical.
  Candidate best = canonical(ps, UNITS, N);

  // Consider single-unit exact representations, choose most readable.
  for (int i = 0; i < N; ++i) {
     Candidate c;
     if (!single_unit(ps, UNITS[i], c)) continue;
     if (better(c, best)) best = std::move(c);
  }

  // If you *only* want the “round number override” for very round values,
  // you can gate the single-unit replacements above with `if (c.first_count_round) ...`.

  return best.text;
}

} // namespace durfmt
