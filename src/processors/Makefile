OUTFILE = processors.a

FILES = ManagerProcessor \
DBDataProcessor

OBJECTS = $(FILES:%=%.o)

all: $(OUTFILE)

$(OBJECTS) : %.o : %.cpp %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS) 


$(OUTFILE): $(OBJECTS)
	ar sr $@ $(OBJECTS)

clean:
	rm -f *.o *.a
