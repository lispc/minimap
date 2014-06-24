#g++ -O2 -std=c++11 -stdlib=libc++ map.cpp -o exe
g++ -std=c++11 -stdlib=libc++ -shared -O2 -o map.so -fPIC map.cpp 
