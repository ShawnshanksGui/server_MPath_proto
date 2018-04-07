
#gcc -std=gnu99  -g -o test sender.c utility.c mySocket.c thread_core_affinity_set.c -lpthread
#g++ -std=c++11 -g -o test data_manager.cpp server.cpp mySocket.cpp utility.cpp thread_core_affinity_set.cpp -lpthread

g++ -std=c++11 -g -o test_reader main.cpp video_reader.cpp  data_manager.cpp codeStreaming_parser.cpp bitrate_select.cpp  fec_param_adjustor.cpp path_selector.cpp myUtility.cpp encoder.cpp mySocket.cpp timer.cpp libRS_FFT.a thread_core_affinity_set.cpp -lpthread
