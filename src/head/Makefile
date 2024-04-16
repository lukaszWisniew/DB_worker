FILES = ParseMainSwitches

OBJECTS = $(FILES:%=%.o)

all: head.a

$(OBJECTS) : %.o : %.cpp %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS) 


head.a: $(OBJECTS)
	ar sr $@ $(OBJECTS)

clean:
	rm -f *.o *.a
