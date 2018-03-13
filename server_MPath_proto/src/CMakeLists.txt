cmake_minimum_required(VERSION 2.8)

project (test_reader)

#aux_source_directory(video_reader.cpp data_manager.cpp codeStreaming_parser.cpp #bitrate_select.cpp fec_param_adjustor.cpp utility.cpp DIR_SRCS)

add_subdirectory(rs_fft)

#add_executable(test_reader DIR_SRCS)
add_executable(test_reader  video_reader.cpp data_manager.cpp codeStreaming_parser.cpp bitrate_select.cpp fec_param_adjustor.cpp utility.cpp)

target_link_libraries(test_reader RS_FFT)