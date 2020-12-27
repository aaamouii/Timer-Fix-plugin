# Timer-Fix-plugin
This plugin makes timers more accurate. [Topic](https://forum.sa-mp.com/showthread.php?t=650736).
<br>This plugin is no longer supported.
<br>Previous versions on the Releases page might have problems and bugs, use at your own risk.

## Usage
Plugin uses default SA-MP natives so you don't have to change your code. Example:
```Pawn
#include <a_samp>

forward Function();
public Function()
{
  printf("Function called after 1 second.");
}

forward FunctionWithArguments(number);
public FunctionWithArguments(number)
{
  printf("Function called after 2 seconds. Number: %d", number);
}

main()
{
  SetTimer("Function", 1000, false);
  SetTimerEx("FunctionWithArguments", 2000, false, "d", 10);
}
```

## Natives
 Replacement for a standart native function that includes additional parameters.
 * delay - the delay before the timer starts.
 * count - count of executions.
 ```Pawn
native SetCustomTimer(funcname[], interval, delay, count);
```

The same function but with argument passing.
```Pawn
native SetCustomTimerEx(funcname[], delay, count, const format[], {Float,_}:...);
```

 Stops the timer but doesn't delete it.
 <br>Use the ContinueTimer() to start it again.
 * Note: the callback will be executed after the time remaining before the call.
 ```Pawn
native PauseTimer(timerid);
```

Continues the timer after pausing.
```Pawn
native ContinueTimer(timerid);
```

 Adds the custom callback for timer.
<br> For example:
```pawn
 new a = SetTimer("function_1");
 AddTimerHandler(a, "function_2");
```
 The timer will execute both callbacks.

* Note: arguments will also be passed to both functions.
```Pawn
native AddTimerHandler(timerid, handler[]);
```

Removes the added handler.
```Pawn
native RemoveTimerHandler(timerid, handler[]);
```

Another natives.
```Pawn
native KillAllTimers();
native IsValidTimer(timerid);
native GetTimerInterval(timerid);
native SetTimerInterval(timerid, interval);
native GetTimerRemainingTime(timerid);
native GetCountOfRunningTimers();
```

## Building
Build a shared library is provided for only two platforms. 

### Windows
1. First of all you should download [CMake](https://cmake.org) from the official site.
2. Then clone this repository or download source code from the [Releases page](https://github.com/KashCherry/Timer-Fix-plugin/releases).
3. Generate project with CMake for MSVC.
4. Build generated project (Don't forget to change configuration to Release Win32).

### Linux
1. Install everything you need to build a library (if not installed)
```
// Ubuntu, Debian, etc
apt-get install cmake
apt-get install git
apt-get install gcc-multilib g++-multilib

// CentOS
yum install cmake
yum install git
yum install gcc-multilib g++-multilib
```
2. Clone this repository
```
git clone --recursive https://github.com/KashCherry/Aisuru
```
3. Generate project with CMake in this folder or create other one
```
mkdir build && cd build && cmake ..
```
4. Build generated project with make
```
make
```

## License
Licensed under MIT License. Learn more about it [here](https://github.com/KashCherry/Timer-Fix-plugin/blob/master/LICENSE).
