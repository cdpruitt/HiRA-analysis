OBJECTS = det.o hira.o histo_sort.o histo_read.o caen.o calibrate.o TDC1190.o  elist.o telescope.o pixels.o doppler.o ZApar.o pid.o solution.o einstein.o newton.o correl.o loss.o forest.o
ALLOBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
CFLAGS= -c -g -std=c++11 -O2 -W -I$(shell root-config --incdir) 
COMPILER= g++
LINKOPTION = $(shell root-config --libs)


sort: sort.o $(OBJECTS)
	$(COMPILER) -o sort sort.o $(OBJECTS) $(LINKOPTION)

readtree: readtree.o $(OBJECTS)
	$(COMPILER) -o readtree readtree.o $(OBJECTS) $(LINKOPTION)

try: try.o $(OBJECTS)
	$(COMPILER) -o try try.o $(OBJECTS) $(LINKOPTION)
 
testPid: testPid.o $(OBJECTS)
	$(COMPILER) -o testPid testPid.o $(OBJECTS) $(LINKOPTION)

modCal: modCal.o $(OBJECTS)
	$(COMPILER) -o modCal modCal.o $(OBJECTS) $(LINKOPTION)
 
ThFront: ThFront.o $(OBJECTS)
	$(COMPILER) -o ThFront ThFront.o $(OBJECTS) $(LINKOPTION)
 
ThBack: ThBack.o $(OBJECTS)
	$(COMPILER) -o ThBack ThBack.o $(OBJECTS) $(LINKOPTION)
 
modCal2: modCal2.o $(OBJECTS)
	$(COMPILER) -o modCal2 modCal2.o $(OBJECTS) $(LINKOPTION)

modCal3: modCal3.o $(OBJECTS)
	$(COMPILER) -o modCal3 modCal3.o $(OBJECTS) $(LINKOPTION)
 
testHist: testHist.o hist.o
	$(COMPILER) -o testHist testHist.o hist.o $(LINKOPTION)

O12_sim: O12_sim.o $(OBJECTS)
	$(COMPILER) -o O12_sim O12_sim.o $(OBJECTS) $(LINKOPTION)

O12_et: O12_et.o $(OBJECTS)
	$(COMPILER) -o O12_et O12_et.o $(OBJECTS) $(LINKOPTION)

O12_sim2: O12_sim2.o $(OBJECTS)
	$(COMPILER) -o O12_sim2 O12_sim2.o $(OBJECTS) $(LINKOPTION)

O12_sim2_center: O12_sim2_center.o $(OBJECTS)
	$(COMPILER) -o O12_sim2_center O12_sim2_center.o $(OBJECTS) $(LINKOPTION)

O12_sim2_good: O12_sim2_good.o $(OBJECTS)
	$(COMPILER) -o O12_sim2_good O12_sim2_good.o $(OBJECTS) $(LINKOPTION)

N12_sim: N12_sim.o $(OBJECTS)
	$(COMPILER) -o N12_sim N12_sim.o $(OBJECTS) $(LINKOPTION)

N12_sim2: N12_sim2.o $(OBJECTS)
	$(COMPILER) -o N12_sim2 N12_sim2.o $(OBJECTS) $(LINKOPTION)

N12_sim3: N12_sim3.o $(OBJECTS)
	$(COMPILER) -o N12_sim3 N12_sim3.o $(OBJECTS) $(LINKOPTION)

N12_et: N12_et.o $(OBJECTS)
	$(COMPILER) -o N12_et N12_et.o $(OBJECTS) $(LINKOPTION)

$(ALLOBJECTS): %.o : %.cpp
	$(COMPILER) $(CFLAGS) $< -o $@


clean:
	rm -f *.o

