/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __LOGONCONSOLE_H
#define __LOGONCONSOLE_H


enum CThreadState
{
	THREADSTATE_TERMINATE = 0,
	THREADSTATE_PAUSED	= 1,
	THREADSTATE_SLEEPING  = 2,
	THREADSTATE_BUSY	  = 3,
	THREADSTATE_AWAITING  = 4,
};
class LogonConsoleThread
{
public:
	LogonConsoleThread();
	~LogonConsoleThread();
	static bool run();
	static void OnShutdown() {}
#ifdef WIN32
	static HANDLE THREAD_HANDLE;
#else
	static pthread_t THREAD_HANDLE;
#endif

	static bool kill;
protected:
	static CThreadState ThreadState;
	static time_t start_time;
	static int ThreadId;

};

class LogonConsole
{
	friend class LogonConsoleThread;

public:						// Public methods:
	void Kill();

protected:					// Protected methods:
	LogonConsoleThread *_thread;

	// Process one command
	void ProcessCmd(char *cmd);

	// quit | exit
	void TranslateQuit(char *str);
	void ProcessQuit(int delay);
	void CancelShutdown(char *str);

	// help | ?
	void TranslateHelp(char *str);
	void ProcessHelp(char *command);

	void ReloadAccts(char *str);
	void TranslateRehash(char* str);

	void NetworkStatus(char* str);

	void SetOutputLevel(char *str);			//设置控制台输出级别
	void ShowConfig(char *str);				//显示配置信息
	void ShowDetail(char *str);				//显示详细信息

};

#define sLogonConsole LogonConsole::getInstance();

#endif
