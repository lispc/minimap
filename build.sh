clang++ -fPIC -O2 -std=c++11 -c map.cpp -o map.o
clang++ map.o -shared -o map.so
