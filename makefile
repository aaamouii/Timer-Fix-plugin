OUTFILE = "timerfix.so"

BOOST_INCLUDE_DIRECTORY=-I/mnt/d/boost_1_67_0
BOOST_LIBRARY_DIRECTORY=-L/mnt/d/boost_1_67_0/stage/lib

DEFINES=-DLINUX
STATIC=-static-libstdc++
LIBRARIES=-lboost_system -lboost_chrono

all:
	g++ -m32 $(BOOST_INCLUDE_DIRECTORY) $(DEFINES) -c -O3 -w -Ilib -std=c++11 ./*.cpp
	g++ -m32 $(BOOST_INCLUDE_DIRECTORY) $(DEFINES) -c -O3 -w -Ilib -std=c++11 ./lib/*.cpp
	g++ -m32 $(BOOST_LIBRARY_DIRECTORY) -shared -O3 $(STATIC) -o $(OUTFILE) *.o $(LIBRARIES)
	rm ./*.o
	strip -s $(OUTFILE)
