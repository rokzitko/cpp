#include <iostream>
#include "prettytime.hh"

int main() {
  using namespace std::chrono;
  std::cout << durfmt::format_human( 3*24h + 1min + 3s ) << "\n"; // "3d 1min 3s"
  std::cout << durfmt::format_human( 60s ) << "\n";              // "1min"
  std::cout << durfmt::format_human( 1500ms ) << "\n";           // "1s 500ms"
  std::cout << durfmt::format_human( duration<long long, std::pico>(12) ) << "\n"; // "12ps"
  std::cout << durfmt::format_human( 547289ns ) << "\n";         
  std::cout << durfmt::format_human( 547289s ) << "\n";         
  std::cout << durfmt::format_human( 3600s ) << "\n";         
  std::cout << durfmt::format_human( 3601s ) << "\n";         
  std::cout << durfmt::format_human( 3660s ) << "\n";
  std::cout << durfmt::format_human( 7200s ) << "\n";
  std::cout << durfmt::format_human( 1800s ) << "\n";
  std::cout << durfmt::format_human( 900s ) << "\n";
  std::cout << durfmt::format_human( 86400s ) << "\n";
  std::cout << durfmt::format_human( 100000s ) << "\n";
  std::cout << durfmt::format_human( 10000s ) << "\n";
  std::cout << durfmt::format_human( 1000s ) << "\n";
  std::cout << durfmt::format_human( 100s ) << "\n";
  std::cout << durfmt::format_human( 10s ) << "\n";
  std::cout << durfmt::format_human( 120s ) << "\n";
  std::cout << durfmt::format_human( 240s ) << "\n";
  std::cout << durfmt::format_human( 300s ) << "\n";
  std::cout << durfmt::format_human(100000s) << "\n";     // "100000s" (wins over decomposition)
  std::cout << durfmt::format_human(60s) << "\n";         // "1min"   (readable canonical, also single-unit exists)
  std::cout << durfmt::format_human(1500ms) << "\n";      // "1s 500ms" (canonical)
  std::cout << durfmt::format_human(duration<long long, std::pico>(12)) << "\n"; // "12ps"
}
