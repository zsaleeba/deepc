#ifndef DEEPTYPES_H
#define DEEPTYPES_H

#include <chrono>


namespace deepC
{

// Types from std::chrono.
using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

// Time in nanosecond resolution.
//typedef std::chrono::time_point<std::chrono::nanoseconds> TimePoint;

} // namespace deepC

#endif // DEEPTYPES_H
