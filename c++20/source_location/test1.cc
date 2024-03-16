#include <source_location>

using namespace std;

void Log(string_view info,
         const source_location & location = source_location::current()) { // evaluation happing at call site!!
  cout << location.file_name() << ":" << location.line() << ": " << info << endl;
}

int main()
{
  Log("hello!");
}
