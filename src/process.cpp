#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid)
{
    user_ = LinuxParser::User(pid_);
    command_ = LinuxParser::Command(pid_);
}


// DONE: Return this process's ID
int Process::Pid() { 
    return pid_;
 }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
    vector<string> cpu_utils = LinuxParser::Cpu(pid_);
    long total_time = stol(cpu_utils[0])
                    + stol(cpu_utils[1])
                    + stol(cpu_utils[2])
                    + stol(cpu_utils[3]);

    float seconds = float(UpTime());

    float cpu_utilization_ = ((total_time / sysconf(_SC_CLK_TCK)) / seconds );
    return cpu_utilization_;

 }

// DONE: Return the command that generated this process
string Process::Command() { 

    return command_;


 }

// DONE: Return this process's memory utilization
string Process::Ram() { 
    
    if(ram_ == "") ram_ = LinuxParser::Ram(pid_);
    return ram_;
 }

// DONE: Return the user (name) that generated this process
string Process::User() {

    return user_;
 }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_);
 }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    
    long ram_o = stol(LinuxParser::Ram(pid_));
    long ram_a = stol(LinuxParser::Ram(a.pid_));

    return ram_o > ram_a;


 }