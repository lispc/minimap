#clang++ -O2 -std=c++11 -stdlib=libc++ map.cpp -o exe
#./exe
clang++ -std=c++11 -stdlib=libc++ -shared -O2 -o map.so -fPIC map.cpp 
