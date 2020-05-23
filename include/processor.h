#ifndef PROCESSOR_H
#define PROCESSOR_H


#include "linux_parser.h"

using std::stof;
using std::vector;
using std::string;


class Processor 
{
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 vector<float> prev_cpu_utils;
 float IdleTime(vector<float>&);
 float NonIdleTime(vector<float>&);
};

#endif