# DB_worker

## Description
The DB Worker is responsible for communication between the database and the rest of the system.
A query from the system is transformed into a database query, and the database responds to the task. 
In response, the DB Worker sends back the status along with the query result if necessary.

## Technology

### Programming Language

The DB Worker is written in C++. To achieve maximum performance, this programming language was chosen.

### Libraries

The primary library used is libevent.

* Communication with the PostgreSQL database - pqxx
* Communication with the data bus Redis – redis-plus-plus (hiredis)
* JSON formatting - nlohmann
* Other program-related components compiled statically - lw_lib

## Architecture

The DB_worker is connected to two data buses. 
One bus is responsible for communication with the system and receiving requests to the database. 
The other bus is used to report the queue occupancy to the DB worker manager, for the purpose of managing the DB workers.
On the other end, it is connected to the PostgreSQL database.

## Configuration

### Command line switches:
| Short form of the switch | Full form of the switch | Description |
| ------------------------ | ----------------------- | ----------- |
| -h                       | --help                  | Display help |
| -V                       | --version               | Display program version |
| -v                       | --verbose               | Verbose mode |
| -c                       | --config                | Specify the configuration file |

_Example:_
```
./db_worker -v -c /etc/db_worker.conf
```
### Configuration File

The configuration file is divided into sections. 
Each section has a name enclosed in square brackets (e.g., [MAIN]) and ends with an empty pair of square brackets ([]).

The file contains the following sections:
- [MAIN](#main-section): Main program configuration.
- [LOG](#log-section): Logging configuration.
- [DB_CONNECTION](#db_connection-section): Database connection configuration. 
- [DB_MANAGER_BUS](#db_manager_bus-section): Configuration of the management bus connection.
- [DATA_BUS](#data_bus-section): Configuration of the database query bus connection.

#### MAIN Section
[MAIN] - start of the configuration section

LOG_ENABLE - enable logging.

Possible options to set:
  * yes - enable logging
  * no - logging disabled

[] - end of the section

_Example:_
```
[MAIN]
  LOG_ENABLE = yes
[]
```

#### LOG Section
Configuration of this section is taken into account only if LOG_ENABLE=yes is set in the MAIN section.

 [LOG] - start of the configuration section
 
 LEVEL - logging level. Specifies the type of notifications to be included in the log. Setting a lower logging level will include information from higher levels in the log as well.
 
Possible options to set:
   * ERROR - Information marked with this tag will appear in the log.
   * WARNING - Information marked with this tag will appear in the log, as well as those marked with ERROR.
   * INFO - Information marked with this tag will appear in the log, as well as those marked with DEBUG, ERROR, WARNING.
   * DEBUG - Information marked with this tag will appear in the log, as well as those marked with ERROR, INFO, WARNING.

FILE_PATH - path to the directory where the program log will be located

FILE_NAME - name of the log file

ADD_PID - append the process PID number to the log file name
Possible options to set:
   * yes - append the process PID number to the log file name
   * no - do not append the process PID number

ROTATE - enable log file rotation
Possible options to set:
   * on - enable rotation
   * off - disable rotation

Subsequent switches are considered only if ROTATE is set to on.

ROTATE_TYPE - set what triggers log rotation
Possible options to set:
   * size - file size
   * time - specified time
     
ROTATE_SIZE_FILE - rotate the file after reaching the specified file size in MB. Parameter considered if ROTATE_TYPE=size.

ROTATE_TIME - rotate the file at the specified time. Time format \<hour:minute\>. _Example_: ROTATE_TIME=23:59. Parameter considered if ROTATE_TYPE=time.

[] - end of the section

_Example:_
```
[LOG]
  LEVEL=error
  FILE_PATH=/var/log/DB_worker/
  FILE_NAME=db_worker
  ADD_PID=yes
  ROTATE=on
  ROTATE_TYPE=size
  ROTATE_SIZE_FILE=5
[]
```

#### DB_CONNECTION Section

Configuration of the connection to the PostgreSQL database.

ADDRESS - server address where the database is located.

DB_NAME - name of the database managed by DB_WORKER.

USER - username for the database.

PASS - password to connect to the database.


_Example:_
```
[DB_CONNECTION]
  ADDRESS=10.25.10.115
  DB_NAME=amadeus_customers
  USER=adbcuser
  PASS=123321
[]
```
#### DB_MANAGER_BUS Section

Configuration for connecting to the communication bus with the manager.

ADDRESS - server address where Redis is running.

PORT - port on which Redis is listening.

CHANNEL_NAME - name of the channel for information exchange.

PASS - password to connect to the Redis database.

_Example:_
```
[DB_MANAGER_BUS]
  ADDRESS=10.25.10.115
  PORT=6379
  CHANNEL_NAME=s_w_bus
  PASS=nsYSmKVlG
[]
```

#### DATA_BUS Section

Configuration for connecting to the query bus for data from the database.

ADDRESS - server address where Redis is running.

PORT - port on which Redis is listening.

CHANNEL_NAME - name of the channel for information exchange.

DOMAIN - Domain describing the group of queries: customer, rooms, ...

PASS - password to connect to the Redis database.

_Example:_
```
[DATA_BUS]
ADDRESS=10.25.10.115
PORT=6379
CHANNEL_NAME=c_db_bus
DOMAIN=customer
PASS=nsYSmKVlG
[]
```
