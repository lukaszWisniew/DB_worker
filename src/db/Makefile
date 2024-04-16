FILES = DBConfig DBDispatcher DBDataMatrix

OBJECTS = $(FILES:%=%.o)

all: db.a

$(OBJECTS) : %.o : %.cpp %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS) 


db.a: $(OBJECTS)
	ar sr $@ $(OBJECTS)

clean:
	rm -f *.o *.a
