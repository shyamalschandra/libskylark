#include <iostream>

#include <elemental.hpp>
#include <boost/mpi.hpp>
#include <boost/format.hpp>
#include <skylark.hpp>

#include <H5Cpp.h>

namespace bmpi =  boost::mpi;
namespace skybase = skylark::base;
namespace skysketch =  skylark::sketch;
namespace skynla = skylark::nla;
namespace skyalg = skylark::algorithms;
namespace skyutil = skylark::utility;

int main(int argc, char** argv) {

    elem::Initialize(argc, argv);
    skybase::context_t context(23234);

    skybase::sparse_matrix_t<double> A;
    elem::Matrix<double> b;

    boost::mpi::timer timer;

    // Load A and b from HDF5 file
    std::cout << "Reading the matrix and rhs... ";
    std::cout.flush();
    timer.restart();
    H5::H5File in(argv[1], H5F_ACC_RDONLY);
    skyutil::io::ReadHDF5(in, "A", A);
    skyutil::io::ReadHDF5(in, "b", b);
    in.close();
    std::cout <<"took " << boost::format("%.2e") % timer.elapsed() << " sec\n";

    elem::Matrix<double> x(b.Height(), 1);
    std::cout << "Using AsyRGS... ";
    timer.restart();
    elem::MakeZeros(x);
    skyalg::AsyRGS(A, b, x, 20, context);
    std::cout <<"took " << boost::format("%.2e") % timer.elapsed() << " sec\n";

    elem::Matrix<double> r(b);
    skybase::Gemv(elem::TRANSPOSE, -1.0, A, x, 1.0, r);
    double res = skybase::Nrm2(r);
    double nrmb = skybase::Nrm2(b);
    std::cout << "||A x - b||_2 / ||b||_2 = " <<
        boost::format("%.2e") % (res / nrmb) <<
        std::endl;

    return 0;
}