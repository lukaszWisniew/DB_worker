FILES = Configuration MainConfig

OBJECTS = $(FILES:%=%.o)

all: config.a

$(OBJECTS) : %.o : %.cpp %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS) 


config.a: $(OBJECTS)
	ar sr $@ $(OBJECTS)

clean:
	rm -f *.o *.a
