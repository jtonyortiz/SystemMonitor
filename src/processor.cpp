#include "processor.h"

float Processor::Utilization() {
  std::vector<std::string> cpu_utils_string = LinuxParser::CpuUtilization();
  std::vector<float> cpu_utils_float(cpu_utils_string.size());
  std::transform(cpu_utils_string.begin(), cpu_utils_string.end(), cpu_utils_float.begin(), [](const std::string &str) {
      return std::stof(str);
  });


  float idle = IdleTime(cpu_utils_float);
  float non_idle = NonIdleTime(cpu_utils_float);
  float total = idle + non_idle;

  // If there is no previous information, compute the long-term value for utilization and update previous information.
  if(prev_cpu_utils.empty()){
      prev_cpu_utils = cpu_utils_float;
      return non_idle/total;
  }

  // Otherwise, you can use deltas.
  float prev_idle = IdleTime(prev_cpu_utils);
  float prev_non_idle = NonIdleTime(prev_cpu_utils);
  float prev_total = prev_idle + prev_non_idle;

  float total_delta = total - prev_total;
  float non_idle_delta = non_idle - prev_non_idle;

  prev_cpu_utils = cpu_utils_float;
  return non_idle_delta/total_delta;
}

float Processor::IdleTime(vector<float> &v){
    float idle = v[LinuxParser::CPUStates::kIdle_] + v[LinuxParser::CPUStates::kIOwait_];
    return idle;
}

float Processor::NonIdleTime(vector<float> &v){
    float non_idle = v[LinuxParser::CPUStates::kUser_]
                     + v[LinuxParser::CPUStates::kNice_]
                     + v[LinuxParser::CPUStates::kSystem_]
                     + v[LinuxParser::CPUStates::kIRQ_]
                     + v[LinuxParser::CPUStates::kSoftIRQ_]
                     + v[LinuxParser::CPUStates::kSteal_];
    return non_idle;
}

