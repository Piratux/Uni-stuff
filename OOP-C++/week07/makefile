CPP      = g++.exe
OBJ      = main.o Fridge.o
EXE      = Project.exe
CPPFLAGS = -std=c++17
RM       = rm.exe -f

DOCGEN   = doxygen
CONFIG   = config

TESTS    = UnitTests.cpp
TESTSOBJ = UnitTests.o
TESTSEXE = tests.exe

.PHONY: all all-before all-after clean clean-custom

run: build
	$(EXE)

build: $(EXE)

rebuild: clean build

clean:
	$(RM) $(OBJ) $(EXE) $(TESTSOBJ) $(TESTSEXE)

$(EXE): main.o Fridge.o 
	$(CPP) $(OBJ) -o $(EXE)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CPPFLAGS)

Fridge.o: Fridge.cpp Fridge.h
	$(CPP) -c Fridge.cpp -o Fridge.o $(CPPFLAGS)

doc: $(EXE)
	$(DOCGEN) $(CONFIG)


tests: $(TESTSEXE)

$(TESTSEXE): UnitTests.o Fridge.o
	$(CPP) UnitTests.o Fridge.o -o $(TESTSEXE) $(CPPFLAGS)

UnitTests.o: UnitTests.cpp
	$(CPP) -c UnitTests.cpp -o UnitTests.o $(CPPFLAGS)