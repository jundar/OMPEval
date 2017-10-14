cmake_minimum_required(VERSION 3.8)
project(OMPEval)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        libdivide/libdivide.h
        omp/CardRange.cpp
        omp/CardRange.h
        omp/CombinedRange.cpp
        omp/CombinedRange.h
        omp/Constants.h
        omp/EquityCalculator.cpp
        omp/EquityCalculator.h
        omp/Hand.h
        omp/HandEvaluator.cpp
        omp/HandEvaluator.h
        omp/OffsetTable.hxx
        omp/Random.h
        omp/Util.h
        ttest/ttest.h
        benchmark.cpp
        LICENSE-libdivide.txt
        LICENSE.txt
        Makefile
        README.md
        main.cpp)

add_executable(OMPEval ${SOURCE_FILES})