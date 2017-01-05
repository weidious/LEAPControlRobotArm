OS := $(shell uname)
ARCH := $(shell uname -m)

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY := ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64
  else
    LEAP_LIBRARY := ../lib/x86/libLeap.so -Wl,-rpath,../lib/x86
  endif
else
  # OS X
  LEAP_LIBRARY := ../lib/libLeap.dylib
endif
all: Sample Sample2
   
Sample: Sample.cpp
	$(CXX) -Wall -g -I../include Sample.cpp -o Sample $(LEAP_LIBRARY)

Sample2: Sample2.cpp
	$(CXX) -Wall -g -I../include Sample2.cpp -o Sample2 ../lib/libLeap.dylib
ifeq ($(OS), Darwin)
	install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib Sample
	install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib Sample2
endif

clean:
	rm -rf *.o Sample Sample.dSYM
