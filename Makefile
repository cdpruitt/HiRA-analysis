SOURCES = src/ADCEvent.cpp src/correl.cpp src/det.cpp src/doppler.cpp src/einstein.cpp src/elist.cpp src/hira.cpp src/histo_read.cpp src/histo_sort.cpp src/loss.cpp src/newton.cpp src/pid.cpp src/pixels.cpp src/readData.cpp src/solution.cpp src/sort.cpp src/text.cpp src/telescope.cpp src/ZApar.cpp 
OBJECTS = $(SOURCES:.c=.o)
CFLAGS= -g -std=c++11 -O0 -W -I$(shell root-config --incdir) 
COMPILER= clang++
LINKOPTION = $(shell root-config --libs)
HEADERS = $(shell echo include/*.h)

all: sort readtree

sort: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o sort $(OBJECTS) $(LINKOPTION)

readtree: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o readtree $(OBJECTS) $(LINKOPTION)

try: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o try $(OBJECTS) $(LINKOPTION)
 
testPid: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o testPid $(OBJECTS) $(LINKOPTION)

modCal: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o modCal $(OBJECTS) $(LINKOPTION)
 
ThFront: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o ThFront $(OBJECTS) $(LINKOPTION)
 
ThBack: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o ThBack $(OBJECTS) $(LINKOPTION)
 
modCal2: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o modCal2 $(OBJECTS) $(LINKOPTION)

modCal3: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o modCal3 $(OBJECTS) $(LINKOPTION)
 
testHist: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o testHist $(LINKOPTION)

O12_sim: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o O12_sim $(OBJECTS) $(LINKOPTION)

O12_et: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o O12_et $(OBJECTS) $(LINKOPTION)

O12_sim2: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o O12_sim2 $(OBJECTS) $(LINKOPTION)

O12_sim2_center: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o O12_sim2_center $(OBJECTS) $(LINKOPTION)

O12_sim2_good: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o O12_sim2_good $(OBJECTS) $(LINKOPTION)

N12_sim: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o N12_sim $(OBJECTS) $(LINKOPTION)

N12_sim2: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o N12_sim2 $(OBJECTS) $(LINKOPTION)

N12_sim3: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o N12_sim3 $(OBJECTS) $(LINKOPTION)

N12_et: $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o N12_et $(OBJECTS) $(LINKOPTION)

%.o : %.cpp $(HEADERS)
	$(COMPILER) $(CFLAGS) $< -o $@

clean:
	rm -f *.o

