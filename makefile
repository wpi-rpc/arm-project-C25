# clean workspace
all: 
	@echo error: no make argument given.  
	@echo allowable args: clean, build, uf2, create
# removes CMake build folder; should be cleaned before pushing code
clean: 
	rm -rf build
# generates the CMake build folder and contents with running GNU Make for the codebase
build: 
	rm -rf build && mkdir build && cd build && cmake .. && cd ..
# generates uf2 executable for 
uf2: 
	cd build && make -j`nproc` && cd ..
create: build uf2


