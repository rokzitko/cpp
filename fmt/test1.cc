#include <iostream>
#include <string>
#include <fmt/format.h>

class event {
 public:
  int count;
  int value;
};

template <>
struct fmt::formatter<event> {
 constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

 template <typename FormatContext>
 auto format(const event &e, FormatContext &ctx) {
   return format_to(ctx.out(), "count={count} {count:#08x} value={value} {value:#08x}", 
     fmt::arg("count", e.count), fmt::arg("value", e.value));
 }
};

void vlog(const char* format, fmt::format_args args)
{
   fmt::vprint(format, args);
}

std::string log_letters = "abcd";

bool log_enabled(char c)
{
  return log_letters.find(c) != std::string::npos;
}

template <typename... Args>
void log(char c, const char* format, const Args & ... args) {
  if (log_enabled(c)) 
    vlog(format, fmt::make_format_args(args...));
}

using namespace fmt::literals;

void cpp11()
{
  fmt::print("Hello, {name}!\n", "name"_a="Rok Zitko");
}

//#include <chrono>
#include <ctime>
#include <fmt/chrono.h>

void cas()
{
//  auto tp = std::chrono::system_clock::now();
//  fmt::print("{:%Y-%m-%d}", tp);
  std::time_t t = std::time(nullptr);
  fmt::print("{:%Y-%m-%d}\n", *std::localtime(&t));
}

#include <vector>
#include <list>
#include <tuple>
#include <fmt/ranges.h>

// https://stackoverflow.com/questions/4850473/pretty-print-c-stl-containers?rq=1

void rng()
{
  std::vector<int> a{1,2,3,4};
  fmt::print("{0} {1}\n", a, fmt::join(a, " "));
  std::list<double> b{1.2,3.4,5.0};
  fmt::print("{}\n", b);
  std::tuple<char, bool, int> c{'a', true, -42};
  fmt::print("{}\n", c);
}


#include <fmt/color.h>

int main()
{
  fmt::print(fmt::emphasis::bold, "hello {}\n", "Rok");
  fmt::print(fmt::fg(fmt::color::red), "{2} hello {0} {1}\n", "Rok", "Zitko", "abc");
  std::cout << fmt::format("{:10}", 123.456) << std::endl;
  std::cout << fmt::format(FMT_STRING("{:10}"), 123.456) << std::endl; // compile time check
  fmt::memory_buffer buf;
  format_to(buf, "{} {}", 123, true);
  std::string s = to_string(buf);
  std::cout << s << std::endl;
  event e {341, 11};
  fmt::print("{}\n", e);
  log('a', "{}\n", 12.33);
  fmt::print(stderr, "This is an error {}!\n", 42);
  cpp11();
  cas();
  rng();
}

