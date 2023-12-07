mpicxx -o output time_life.cpp mpi_life.cpp naive.cpp openmp.cpp concurrency.cpp concurrency_openmp.cpp concurrencyTestAll.cpp

mpicxx -o serial.out serial.cpp

