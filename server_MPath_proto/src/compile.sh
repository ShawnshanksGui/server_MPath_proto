
#gcc -std=gnu99  -g -o test sender.c utility.c mySocket.c thread_core_affinity_set.c -lpthread
g++ -std=c++11 -g -o test sender.cpp mySocket.cpp utility.cpp -lpthread