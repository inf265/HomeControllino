# Simple Scheduler Library for Arduino and ESP8266

Simple Scheduler is a small but flexible library for Arduino and ESP8266 that implements a basic time based task scheduler.

Multiple tasks can be attached to a single scheduler that will execute these tasks on a fixed msec interval. Multiple schedulers with each their own interval can be defined in parallel.

## Functionality

The functions in the class SimpleScheduler include:
```
SimpleScheduler     // constructor + set interval (msec)
attachHandler()     // attach new task handler function
start()             // start scheduler
stop()              // stop scheduler
```

## Library Dependencies

- https://github.com/PaulStoffregen/TimerOne (for arduino / not required for esp8266)

Version history:
```
0.1                 // initial version
0.11                // some cleanup
```
