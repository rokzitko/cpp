#include <iostream>
#include <string>

class Cell
{
  public:
    void setValue(this Cell& self, double value) {
      self.m_value = value;
    }
    double getValue(this const Cell& self) {
      return self.m_value;
    }
  private:
    double m_value {};
};

int main()
{
  std::cout << "Hello world!" << std::endl;
  Cell c;
  c.setValue(1.1);
  std::cout << "c=" << c.getValue() << std::endl;
}

