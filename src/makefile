GPP = g++ -m32 -std=c++11 -Ilib -fno-stack-protector -nodefaultlibs
GCC = gcc -m32 -Ilib -fno-stack-protector -nodefaultlibs
LINK = $(GCC)
TIMER_OUTFILE = "./timerfix.so"

COMPILE_FLAGS = -c -O3 -fpack-struct=1 -fPIC -w -DLINUX

TIMER = -D TIMER $(COMPILE_FLAGS)

all: TIMER

clean:
	-rm -f *~ *.o *.so
  
static: GPP = g++ -m32 -std=c++11 -Ilib -fno-stack-protector -static-libgcc -static-libstdc++
static: GCC = gcc -m32 -Ilib -fno-stack-protector -static-libgcc -static-libstdc++
static: LINK = $(GPP)
static: all

TIMER: clean
	$(GPP) $(TIMER) ./*.cpp	
	$(LINK) -fshort-wchar -shared -o $(TIMER_OUTFILE) *.o