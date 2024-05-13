#ifndef _parser_h_
#define _parser_h_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Simple command line parser
// https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
class InputParser {
 public:
   InputParser (int argc, char *argv[]) {
     for (int i = 1; i < argc; ++i)
       this->tokens.push_back(std::string(argv[i]));
   }
   std::string get(const std::string &option) const {
     auto itr = std::find(this->tokens.begin(), this->tokens.end(), option);
     if (itr != this->tokens.end() && ++itr != this->tokens.end())
       return *itr;
     return "";
   }
   bool exists(const std::string &option) const {
     return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
   }
//   template <typename T>
//     T get(const std::string &option, const T def) const {
//       return exists(option) ? T(get(option)) : def;
//     }
   auto get_double(const std::string &option, const double def) const {
       return exists(option) ? std::stod(get(option)) : def;
     }
 private:
   std::vector<std::string> tokens;
};

#endif
