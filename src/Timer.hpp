#include <unordered_map>
#include <list>
#include <deque>
#include <algorithm>
#include <chrono>
#include <stdarg.h>
#include <string.h>

#include "amx/amx.h"
#include "amx/amx2.h"
#include "plugincommon.h"

typedef void(*logprintf_t)(char* format, ...);
logprintf_t logprintf;
using namespace std;

namespace Timer
{
	class Utility
	{
	public:

		static void SetFilePath(const char *filepath)
		{
			FilePath = filepath;
		}

		static void Printf(const char *szFormat, ...)
		{
			va_list vaArgs;
			char szBuffer[256];
			va_start(vaArgs, szFormat);
			vsnprintf(szBuffer, sizeof(szBuffer), szFormat, vaArgs);
			va_end(vaArgs);

			return logprintf("[Timer Fix plugin] %s", szBuffer);
		}

		static bool Log(const char *szFormat, ...)
		{
			va_list vaArgs;
			char szBuffer[256];
			va_start(vaArgs, szFormat);
			vsnprintf(szBuffer, sizeof(szBuffer), szFormat, vaArgs);
			va_end(vaArgs);

			int hour, min, sec;
			gettime(hour, min, sec);

			FILE *file = fopen(FilePath.c_str(), "a");
			if (file != NULL) {
				fprintf(file, "[%02d:%02d:%02d] %s\n", hour, min, sec, szBuffer);
				fclose(file);
				return true;
			}
			return false;
		}

		
	private:
		static void gettime(int &hour, int &min, int &sec) {
			tm *timeinfo;
			time_t rawtime;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			
			hour = timeinfo->tm_hour;
			min = timeinfo->tm_min;
			sec = timeinfo->tm_sec;
		}

		static string FilePath;
	};
	string Utility::FilePath;

	namespace Manager
	{
		class Callback
		{
		public:

			struct AmxListItem {
				AMX *amx;
				unordered_map<string, int> _callback_map;
			};
			using AmxList = list<AmxListItem>;

			static void Initialize(AMX *amx)
			{
				AmxListItem _list_item{ amx };

				int num_publics;
				amx_NumPublics(amx, &num_publics);
				for (int i = 0; i < num_publics; i++) {
					char public_name[32];
					amx_GetPublic(amx, i, public_name);
					_list_item._callback_map.emplace(move(public_name), i);
				}
				if (IsGamemode(amx)) {
					_amx_list.push_front(_list_item);
				} else {
					_amx_list.push_back(_list_item);
				}
			}

			static cell Exec(string public_name, deque<cell> params)
			{
				cell retval = -1;
				unordered_map<string, int>::const_iterator iter;
				for (auto amx_iter : _amx_list) {
					if ((iter = amx_iter._callback_map.find(public_name)) != amx_iter._callback_map.end()) {
						for (auto param : params) amx_Push(amx_iter.amx, param);
						if (amx_Exec(amx_iter.amx, &retval, iter->second) != AMX_ERR_NONE) {
							retval = -1;
						}
					}
				}
				return retval;
			}

			static void Push(AMX *amx, const char *public_name, int index)
			{
				auto amx_iter = find_if(_amx_list.begin(), _amx_list.end(), [amx](const AmxListItem &item) {
					return item.amx == amx;
				});
				if (amx_iter != _amx_list.end()) {
					if(amx_iter->_callback_map.find(public_name) != amx_iter->_callback_map.end()) amx_iter->_callback_map.emplace(move(public_name), index);
				} else {
					Utility::Printf("Cannot find amx.");
					Utility::Log("Cannot find amx. Create issue on github [https://github.com/KashCherry/Timer-Fix-plugin]");
				}
			}

			static int Find(AMX *amx, const char *public_name)
			{
				auto amx_iter = find_if(_amx_list.begin(), _amx_list.end(), [amx](const AmxListItem &item) {
					return item.amx == amx;
				});
				if (amx_iter != _amx_list.end()) {
					auto iter = amx_iter->_callback_map.find(public_name);
					if (iter != amx_iter->_callback_map.end()) return iter->second;
				} else {
					Utility::Printf("Cannot find amx.");
					Utility::Log("Cannot find amx. Create issue on github [https://github.com/KashCherry/Timer-Fix-plugin]");
				}
				return -1;
			}

		private:


			static bool IsGamemode(AMX *amx)
			{
				int idx;
				if (amx_FindPublic(amx, "OnFilterScriptInit", &idx) != AMX_ERR_NONE) {
					return true;
				}
				return false;
			}

			static AmxList _amx_list;
		};
		Callback::AmxList Callback::_amx_list;

		class Timer
		{
		public:

			struct TimerMapItem {
				string public_name;
				int interval;
				chrono::steady_clock::time_point start_time;
				bool
					repeat, isdestroyed;
				deque<cell> params;
			};
			using TimerMap = unordered_map<int, TimerMapItem>;

			static int New(AMX *amx, const char *callback, int interval, bool repeat)
			{
				gTimerID++;

				_timer_map.insert(pair<int,TimerMapItem>(gTimerID, TimerMapItem{ callback, interval, chrono::steady_clock::now(), repeat, false }));
				return gTimerID;
			}

			static int NewEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset)
			{
				gTimerID++;

				deque<cell> _timer_params;
				for (int i = strlen(format); --i != -1;) {
					switch (format[i]) {
					case 'i':
					case 'I':
					case 'd':
					case 'D':
					case 'c':
					case 'C':
					case 'b':
					case 'B':
					case 'f':
					case 'F':
						_timer_params.push_back(*GetAddr(amx, params[offset + i]));
						break;
					case 's':
					case 'S':
						_timer_params.push_back(params[offset + i]);
						break;
					default:
						Utility::Printf("SetTimerEx: Unknown format specifer '%c'", format[i]);
						Utility::Log("SetTimerEx: Unknown format specifer '%c'", format[i]);
					}
				}
				
				_timer_map.insert(pair<int, TimerMapItem>(gTimerID, TimerMapItem{ callback, interval, chrono::steady_clock::now(), repeat, false, _timer_params }));
				return gTimerID;
			}

			static void Kill(int timerid)
			{
				auto iter = _timer_map.find(timerid);
				if (iter != _timer_map.end()) {
					iter->second.isdestroyed = true;
				} else {
					Utility::Printf("KillTimer: Cannot find timer with id %d", timerid);
					Utility::Log("KillTimer: Cannot find timer with id %d", timerid);
				}
			}
			
			static void KillAll()
			{
				_timer_map.clear();
			}

			static int GetInterval(int timerid)
			{
				auto iter = _timer_map.find(timerid);
				if (iter != _timer_map.end()) {
					return iter->second.interval;
				}
				Utility::Printf("Cannot find timer with id %d", timerid);
				Utility::Log("Cannot find timer with id %d", timerid);
				return -1;
			}

			static void SetInterval(int timerid, int interval)
			{
				auto iter = _timer_map.find(timerid);
				if (iter != _timer_map.end()) {
					iter->second.interval = interval;
				} else {
					Utility::Printf("Cannot find timer with id %d", timerid);
					Utility::Log("Cannot find timer with id %d", timerid);
				}
			}

			static bool Find(int timerid)
			{
				return _timer_map.find(timerid) != _timer_map.end();
			}

			static void Process()
			{
				for (auto iter = _timer_map.begin(); iter != _timer_map.end();) {
					if (iter->second.isdestroyed) {
						_timer_map.erase(iter++);
						continue;
					}
					if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - iter->second.start_time).count() >= iter->second.interval) {
						if (Callback::Exec(iter->second.public_name, iter->second.params) == -1) {
							Utility::Printf("Callback call failed.");
							Utility::Log("Callback call failed. Create issue on github [https://github.com/KashCherry/Timer-Fix-plugin]");
						}
						if (iter->second.repeat) {
							iter->second.start_time = chrono::steady_clock::now();
						} else {
							_timer_map.erase(iter++);
							continue;
						}
					}
				}
			}
			
		private:

			static cell *GetAddr(AMX *amx, cell param)
			{
				cell *addr;
				if (amx_GetAddr(amx, param, &addr) == AMX_ERR_NONE) {
					return addr;
				}
				return nullptr;
			}

			static TimerMap _timer_map;
			static int gTimerID;
		};
		Timer::TimerMap Timer::_timer_map;
		int Timer::gTimerID = 0;
	}

	class Natives
	{
	public:

		static void Register(AMX *amx)
		{
			vector<AMX_NATIVE_INFO> natives{
				{"IsValidTimer", n_IsValidTimer},
				{"KillAllTimers", n_KillAllTimers},
				{"GetTimerInterval", n_GetTimerInterval},
				{"SetTimerInterval", n_SetTimerInterval}
			};
			amx_Register(amx, natives.data(), natives.size());
		}

		static cell AMX_NATIVE_CALL n_SetTimer(AMX *amx, cell *params)
		{
			char *callback;
			amx_StrParam(amx, params[1], callback);
			return Manager::Timer::New(amx, callback, params[2], !!params[3]);
		}

		static cell AMX_NATIVE_CALL n_SetTimerEx(AMX *amx, cell *params)
		{
			char
				*callback, *format;
			amx_StrParam(amx, params[1], callback);
			amx_StrParam(amx, params[4], format);
			return Manager::Timer::NewEx(amx, callback, params[2], !!params[3], params, format, 5);
		}

		static cell AMX_NATIVE_CALL n_KillTimer(AMX *amx, cell *params)
		{
			Manager::Timer::Kill(params[1]);
			return 1;
		}

	private:

		// native IsValidTimer(timerid);
		static cell AMX_NATIVE_CALL n_IsValidTimer(AMX *amx, cell *params)
		{
			if (Manager::Timer::Find(params[1])) return 1;
			return 0;
		}

		// native KillAllTimers();
		static cell AMX_NATIVE_CALL n_KillAllTimers(AMX *amx, cell *params)
		{
			Manager::Timer::KillAll();
			return 1;
		}

		// native GetTimerInterval(timerid);
		static cell AMX_NATIVE_CALL n_GetTimerInterval(AMX *amx, cell *params)
		{
			return Manager::Timer::GetInterval(params[1]);
		}

		// native SetTimerInterval(timerid, interval);
		static cell AMX_NATIVE_CALL n_SetTimerInterval(AMX *amx, cell *params)
		{
			Manager::Timer::SetInterval(params[1], params[2]);
			return 1;
		}
	};
}