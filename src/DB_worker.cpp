#include <iostream>
#include <error.h>
#include <signal.h>

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>

#include "head/ParseMainSwitches.h"
#include "config/Configuration.h"
#include "ev_head/Context.h"

#include "db/DBDataMatrix.h"


int main(int argc, char **argv) {

	//signal(SIGPIPE, SIG_IGN);

	Configuration configuration;
	Context context;
	DBDataMatrix dbDataMatrix;

	//DataQueueStatus dqs;

	ParseMainSwitches *pms = new ParseMainSwitches();

	pms->parse(argc, argv, &configuration);
	configuration.readConfigFile();

	context.setConfiguration( &configuration );


	context.log.sendInfo("DB_worker started");
#if 1
	/* Event base */
	context.setBase(event_base_new ());
	if (context._base == NULL)
		error (1, 0, "failed to create libevent base");
#endif
#if 1

	context.startAutoService();

	event_base_dispatch (context._base);

	cout << "\n\nExit from DB_worker"  << endl;
#endif
	exit(EXIT_SUCCESS);
}
