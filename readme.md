This is a logger, because you can't have enough of it.

The idea is to be lightweight, and do only what I personally need it to do - nothing more and nothing less.

One can request two kind of loogers currently: one that logs to console (serial), and one that logs to a file.
The file logger logs into separate files, based on the name of the logger.

The communication with the logger happens through UNIX socket. Currently hardcoded to `/tmp/log_sock`.

Two kind of requests are available. The below structs illustrate the data formats:

```
struct register_logger {
  int requestType; // 0 always, meaning register new logger
  int nameLength; // the length of the next member, which is the name of the logger. Include null terminator also.
  char name[nameLength]; // the name of the logger
  int loggerType; // 0 for console logging, 1 for logging into a file
};
```

and the other is

```
struct log_message {
  int requestType; // 1 always, meaning log a new message
  int nameLength; // the length of the next member, which is the name of the logger. Include null terminator also.
  char name[nameLength]; // the name of the logger
  int msgLength; // the length of the next member, which is the name of the message to log. Include null terminator also.
  char msg[msgLength]; // the message to log
  int logLevel; // 0 - DEBUG, 1 - INFO, 2 - WARNING, 3 - ERROR, 4 - FATAL
};
```

The log files are currently created as `/tmp/logs/$name.log`. No rotation for now. The log folder must exist already.
