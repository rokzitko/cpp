#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <Eigen/Dense>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace mpi = boost::mpi;

namespace boost { namespace serialization {

template<   class Archive,
            class S,
            int Rows_,
            int Cols_,
            int Ops_,
            int MaxRows_,
            int MaxCols_>
inline void serialize(Archive & ar,
                      Eigen::Matrix<S, Rows_, Cols_, Ops_, MaxRows_, MaxCols_> & matrix,
                      const unsigned int version)
{
  int rows = matrix.rows();
  int cols = matrix.cols();
  ar & make_nvp("rows", rows);
  ar & make_nvp("cols", cols);    
  matrix.resize(rows, cols); // no-op if size does not change!

  // always save/load row-major
  for(int r = 0; r < rows; ++r)
    for(int c = 0; c < cols; ++c)
      ar & make_nvp("val", matrix(r,c));
}    

}} // namespace boost::serialization

int main() {
	mpi::environment env;
	mpi::communicator world;

  using Eigen::VectorXd;
  std::vector<VectorXd> localVectors;

  // Eigen uses std::rand for its randomness, so this ensures different processes generate different vectors
  std::srand((unsigned int) (world.rank() + 1)*std::time(0) );

  for(int idx = 0; idx < world.rank() + 1; idx++) 
    localVectors.push_back(VectorXd::Random(idx+1));

  std::vector<VectorXd> allPoints;
  allPoints.insert(allPoints.end(), localVectors.begin(), localVectors.end());

  for(int procid = 1; procid < world.size(); procid++ ) {
    if( world.rank() == 0) {
      std::vector<VectorXd> receivedVecs;
      world.recv(procid, mpi::any_tag, receivedVecs);
      allPoints.insert(allPoints.end(), receivedVecs.begin(), receivedVecs.end());
      std::cout << "received " << receivedVecs.size() << " vectors from process " << procid << std::endl;
      world.barrier();
    } else if (world.rank() == procid) {
      world.send(0, 0, localVectors);
      world.barrier();
    } else {
      world.barrier();
    }
  }

  if(world.rank() == 0) {
    for(int idx=0; idx < allPoints.size(); idx++) 
      std::cout << allPoints[idx].transpose() << std::endl;
  }
	return 0;
}
