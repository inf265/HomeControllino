// Copyright  : Dennis Buis (2017,2020)
// License    : MIT
// Platform   : Arduino
// Library    : Simple Scheduler Library for Arduino
// File       : SimpleScheluder.h
// Purpose    : Setting up a simple task scheduler
// Repository : https://github.com/DennisB66/Simple-Scheduler-Library-for-Arduino

#ifndef _SIMPLE_SCHEDULER_H
#define _SIMPLE_SCHEDULER_H

#include "SimpleTask.h"

class SimpleSchedulerTask : public SimpleTask
{
public:
  SimpleSchedulerTask( TaskFunc);
};

class SimpleScheduler : public SimpleTaskList
{
public:
  SimpleScheduler( unsigned long = 1);

  virtual void attachHandler( TaskFunc);

  void start();
  void stop();

protected:
  unsigned long _msec;
};

#endif
