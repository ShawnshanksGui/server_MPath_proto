#aux_source_directory(. DIR_LIB_SRCS)
#add_library(RS_FFT &{DIR_LIB_SRCS})


SET(CMAKE_C_COMPILER gcc)
add_library(RS_FFT rs_fft.c)