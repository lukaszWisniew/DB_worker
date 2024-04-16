OUTFILE = redisBus.a
FILES = RedisCtx \
 config/RedisBusConfig \
 config/DataRBConfig \
 config/ManagerRBConfig \
 config/QueueStatusType \
 manager/RedisManagerBus \
 manager/RedisManagerCtx \
 data/RedisDataBus \
 data/RedisDataCtx \
 frames/ToFrom_JSON \
 frames/JsonRedisMainFrame \
 frames/DataQueueStatus\
 frames/DBWorkersStatus\
 frames/DataDBRequest\
 frames/DataDBResponse\
 frames/types/QueueData\
 frames/types/WorkersStatusData\
 frames/types/DBRequestData


OBJECTS = $(FILES:%=%.o)

all: $(OUTFILE)

$(OBJECTS) : %.o : %.cpp %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS) 


$(OUTFILE): $(OBJECTS)
	ar sr $@ $(OBJECTS)

clean:
	rm -f *.o *.a
	rm -f config/*.o config/*.a
	rm -f manager/*.o manager/*.a
	rm -f frames/*.o frames/*.a
	rm -f frames/types/*.o frames/types/*.a
	rm -f data/*.o data/*.a