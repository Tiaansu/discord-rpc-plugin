#pragma once

#include <chrono>

// taken from open.mp: https://github.com/openmultiplayer/open.mp-sdk/blob/3dc9cf854c2a002a683baf50d5845f396c634e2c/include/types.hpp#L56-L66
typedef std::chrono::steady_clock Time;
typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::system_clock WorldTime;
typedef std::chrono::system_clock::time_point WorldTimePoint;
typedef std::chrono::nanoseconds Nanoseconds;
typedef std::chrono::microseconds Microseconds;
typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::seconds Seconds;
typedef std::chrono::minutes Minutes;
typedef std::chrono::hours Hours;
typedef std::chrono::duration<float> RealSeconds;
using std::chrono::duration_cast;