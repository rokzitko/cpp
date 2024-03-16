#include <iostream>
#include <gsl/gsl_linalg.h>

int main()
{
  double b_data[] = { 1.5, 3.0, 4.5 };
  gsl_vector_view b = gsl_vector_view_array(b_data, 3);
  gsl_vector_fprintf(stdout, &b.vector, "%lf");
}
