VERSION_FLAG=-DGIT_COMMIT=\"$(shell git describe --long --always --dirty=+dirty | sed 's/.*-g\?//' | sed 's/-/+/g')\"
BUILD_DATE_FLAG=-DBUILD_DATE=\"$(shell date +%d.%m.%Y)\"
RELEASE_DATE_FLAG=-DRELEASE_DATE=\"$(shell git log -1 --format=%cd --date=format:%d.%m.%Y | head -n 1)\"
PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

LW_LIB__SRC=$(PROJECT_ROOT)../../lw-lib/src

OBJS = DB_worker.o head/head.a ev_head/ev_head.a config/config.a db/db.a redis_bus/redisBus.a processors/processors.a
LIBS = -levent -lpqxx -lpq -lredis++ -lhiredis -pthread
CXX=g++
export CXXFLAGS=-g -Wall -pedantic -std=gnu++17 -I$(LW_LIB__SRC) $(VERSION_FLAG) $(RELEASE_DATE_FLAG) $(BUILD_DATE_FLAG)

export LIB_ADD=$(LW_LIB__SRC)/config/libconf.a\
	 $(LW_LIB__SRC)/ev_main/libevmain.a\
	 $(LW_LIB__SRC)/misc/libmisc.a\
	 $(LW_LIB__SRC)/log/liblog.a\
	 $(LW_LIB__SRC)/hiredis/libhiredis.a
	 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64/

all:
	$(MAKE) -C processors
	$(MAKE) -C redis_bus
	$(MAKE) -C db
	$(MAKE) -C config
	$(MAKE) -C head
	$(MAKE) -C ev_head
	$(MAKE) DB_worker

DB_worker:	$(OBJS)
	$(CXX) $^ $(LIB_ADD) -o $@ $(LDFLAGS) $(LIBS)
	$(EXTRA_CMDS)
	
clean:
	rm -fr DB_worker $(OBJS)
	$(MAKE) -C redis_bus clean
	$(MAKE) -C db clean
	$(MAKE) -C config clean
	$(MAKE) -C head clean
	$(MAKE) -C ev_head clean
	$(MAKE) -C processors clean
