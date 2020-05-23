#include <string>

#include "format.h"

using std::string;

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string elapsed = "";
  long hh, mm, ss;
  hh = seconds / 3600;
  mm = (seconds - hh*3600) / 60;
  ss = seconds - hh*3600 - mm*60;
  return std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss);
}