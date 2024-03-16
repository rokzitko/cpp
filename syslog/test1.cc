#include <iostream>
#include "log.h"

int main()
{
   std::clog.rdbuf(new Log("foo", LOG_LOCAL0));
   std::clog << kLogNotice << "test log message" << std::endl;
   std::clog << "the default is debug level" << std::endl;
}
