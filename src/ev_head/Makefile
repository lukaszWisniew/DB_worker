FILES = Context

OBJECTS = $(FILES:%=%.o)

all: ev_head.a

$(OBJECTS) : %.o : %.cpp %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS) 


ev_head.a: $(OBJECTS)
	ar sr $@ $(OBJECTS)

clean:
	rm -f *.o *.a
