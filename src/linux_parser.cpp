#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  std::vector<float> memo_amounts;

  string memo_type, memo_amount, memo_unit;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> memo_type >> memo_amount >> memo_unit;
      memo_amounts.push_back(stof(memo_amount));

      if(memo_type == "SwapCached:")
      {
        break;
      }


    }
  }

  float total_memo = memo_amounts[0];
  float free_memo = memo_amounts[1];

  return (total_memo - free_memo) / total_memo;

 }

// Done: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string val;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> val;
    return stol(val);
  }

  return 0;

 }

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total = 0;
  vector<string> cpu_utilization;
  cpu_utilization = LinuxParser::CpuUtilization();

  for(int state = kUser_; state <= kSteal_; state++)
  {
    total += std::stol(cpu_utilization[state]);
  }

  return total;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 

  long total = 0;
  string info;
  string line;
  int i = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while(std::getline(linestream, info, ' '))
    {

      if((i == 13) || (i == 14) || (i == 15))
      {

        total += stol(info);

        i++;

        continue;
      }
      else if(i == 16)
      {
        total += stol(info);

        break;


      }

      i++;
    }
  }

  return total;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
 { 
   long active = LinuxParser::Jiffies() - LinuxParser::IdleJiffies();

   return active;
 }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 

  long total = 0;
  vector<string> cpu_utilization = LinuxParser::CpuUtilization();
  for(int state = kIdle_; state <= kIOwait_; state++)
  {
    total += stol(cpu_utilization[state]);
  }
  return total;

 }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_util;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string token;
    while(std::getline(linestream, token, ' '))
    {
      if((token != "cpu") && (token != ""))
      {
        cpu_util.push_back(token);
      }
    }
    return cpu_util;

  }

  return cpu_util;

}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

int num = 0;
string line;
std::ifstream stream(kProcDirectory + kStatFilename);

if(stream.is_open())
{
  while(std::getline(stream, line))
  {
    std::istringstream linestream(line);
    string val;

    linestream >> val;
    if(val == "processes")
    {
      linestream >> val;
      num = stoi(val);
      return num;
    }
  }
}
  return 0;

}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 

  int num = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      string val;

      linestream>> val;
      if(val == "procs_running")
      {
        linestream >> val;
        num = stoi(val);
        return num;
      }
    }
  }

  return 0;

 }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open())
  {
    std::getline(stream,line);
    return line;
  }
  
  return string(); 
  }

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 

  string key, val;
  string line;
  std::stringstream ram;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> val;;

      if(key == "VmSize:")
      {
        //Convert to MB:
        ram << std::fixed << std::setprecision(2) << stof(val) / 1000;
        return ram.str();
      }
    }
  }

  return string("0");
 }

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  
  string uid;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      string val;
      linestream >> val;
      if(val == "Uid:")
      {
        linestream >> uid;
        return uid;
      }
    }
  }
  
  
  return string();
  
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  
  string uid = Uid(pid);
  string line;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      vector<string> vals;
      string token;
      while(std::getline(linestream, token, ':'))
      {
        vals.push_back(token);

      }

      if(vals[2] == uid) return vals[0];

    }
  }
  
  
  return string(); 
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      vector<string> vals;
      string token;
      while(std::getline(linestream, token, ' '))
      {
        vals.push_back(token);
      }

      return stol(vals[21]) / sysconf(_SC_CLK_TCK);

    }
  }
  
  return 0; 
  
  }

  vector<string> LinuxParser::Cpu(int pid)
  {
    vector<string> cpu_util;
    string line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if(stream.is_open())
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      vector<string> vals;
      string token;
      while(std::getline(linestream,token, ' '))
      {
        if((token != "cpu") && (token != ""))
        {
          vals.push_back(token);

        }

      }

      cpu_util.push_back(vals[13]);
      cpu_util.push_back(vals[14]);
      cpu_util.push_back(vals[15]);
      cpu_util.push_back(vals[16]);
      cpu_util.push_back(vals[21]);
      return cpu_util;

    }

    return cpu_util;
  }