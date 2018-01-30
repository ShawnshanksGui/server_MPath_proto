#gcc -std=c99 -g -o test  receiver.c  _socket.c  utility.c  thread_core_affinity_set.c -lpthread

g++ -std=c++11 -g -o test mySocket.cpp utility.cpp receiver.cpp -lpthread
