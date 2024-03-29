#include <iostream>
#include <tbb/flow_graph.h>

using namespace tbb::flow;

struct body {
  std::string my_name;
  body(std::string name) : my_name(name) {}
  void operator()(continue_msg) const {
    std::cout << my_name << std::endl;
  }
};


int main()
{
  graph g;

  broadcast_node< continue_msg > start(g);
  continue_node<continue_msg> a(g, body("A"));
  continue_node<continue_msg> b(g, body("B"));
  continue_node<continue_msg> c(g, body("C"));
  continue_node<continue_msg> d(g, body("D"));
  continue_node<continue_msg> e(g, body("E"));
  
  make_edge(start, a);
  make_edge(start, b);
  make_edge(a, c);
  make_edge(b, c);
  make_edge(c, d);
  make_edge(a, e);
  
  for (int i = 0; i < 3; ++i) {
    start.try_put(continue_msg());
    g.wait_for_all();
  }
  
  return 0;
}
