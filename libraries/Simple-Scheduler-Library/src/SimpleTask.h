// Copyright  : Dennis Buis (2017,2020)
// License    : MIT
// Platform   : Arduino
// Library    : Simple Scheduler Library
// File       : SimpleScheluder.h
// Purpose    : Simple library for task management
// Repository : https://github.com/DennisB66/Simple-Scheduler-Library-for-Arduino

#ifndef _SIMPLE_TASK_H
#define _SIMPLE_TASK_H

typedef void (*TaskFunc)();

class SimpleTask
{
public:
  SimpleTask(TaskFunc); // create task (function, frequency)

  TaskFunc func();    // return task callback function
  SimpleTask *next(); // return task frequency

  void setFunc(TaskFunc);     // set task calllback (function)
  void setNext(SimpleTask *); // set next task (task)

protected:
  TaskFunc _func;    // task callback function
  SimpleTask *_next; // next task
};

class SimpleTaskList
{
public:
  SimpleTaskList(); // create (empty) task list

  virtual void attachHandler(TaskFunc); // insert callback function
  virtual void detachHandler(TaskFunc); // delete calback function

  virtual void print(); // print task list

protected:
  static SimpleTask *_rootTask; // first task in task list
  static SimpleTask *_lastTask; // last  task in task list

  void _attach(SimpleTask *); // insert task into task list
  void _detach(SimpleTask *); // delete task into task list

  static void _handle(); // execute all tasks in task list
};

#endif
