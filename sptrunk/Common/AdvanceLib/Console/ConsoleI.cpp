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

#include "stdafx.h"
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include "ConsoleO.h"
#include "ConsoleI.h"

//bool Rehash();
extern boost::program_options::variables_map	g_options_map;			//	global program-options map

void LogonConsole::TranslateRehash(char* str)
{
	//sLog.outString("rehashing config file...");
	//Rehash();
}

void LogonConsole::Kill()
{
	_thread->kill=true;
#ifdef WIN32
	/* write the return keydown/keyup event */
	DWORD dwTmp;
	INPUT_RECORD ir[2];
	ir[0].EventType = KEY_EVENT;
	ir[0].Event.KeyEvent.bKeyDown = TRUE;
	ir[0].Event.KeyEvent.dwControlKeyState = 288;
	ir[0].Event.KeyEvent.uChar.AsciiChar = 13;
	ir[0].Event.KeyEvent.wRepeatCount = 1;
	ir[0].Event.KeyEvent.wVirtualKeyCode = 13;
	ir[0].Event.KeyEvent.wVirtualScanCode = 28;
	ir[1].EventType = KEY_EVENT;
	ir[1].Event.KeyEvent.bKeyDown = FALSE;
	ir[1].Event.KeyEvent.dwControlKeyState = 288;
	ir[1].Event.KeyEvent.uChar.AsciiChar = 13;
	ir[1].Event.KeyEvent.wRepeatCount = 1;
	ir[1].Event.KeyEvent.wVirtualKeyCode = 13;
	ir[1].Event.KeyEvent.wVirtualScanCode = 28;
	WriteConsoleInput (GetStdHandle(STD_INPUT_HANDLE), ir, 2, & dwTmp);
#endif
	printf("Waiting for console thread to terminate....\n");
	while(_thread != NULL)
	{
		Sleep(100);
	}
	printf("Console shut down.\n");
}

bool LogonConsoleThread::run()
{
	new LogonConsole;

	//SetThreadName("Console Interpreter");
	//LogonConsole::getInstance()._thread = this;
	size_t i = 0, len = 0;
	char cmd[96];

#ifndef WIN32
	fd_set fds;
	struct timeval tv;
#endif

	while (!kill)
	{
#ifndef WIN32
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		FD_ZERO( &fds );
		FD_SET( STDIN_FILENO, &fds );
		if ( select( 1, &fds, NULL, NULL, &tv ) <= 0 )
		{
			if(!kill) // timeout
				continue;
			else
				break;
		}
#endif
		// Make sure our buffer is clean to avoid Array bounds overflow
		memset( cmd,0,sizeof( cmd ) );
		// Read in single line from "stdin"
		fgets( cmd, 80, stdin );

		if ( kill )
			break;

		len = strlen( cmd );
		for ( i = 0; i < len; ++i )
		{
			if ( cmd[i] == '\n' || cmd[i] == '\r' )
				cmd[i] = '\0';
		}
		LogonConsole::getInstance().ProcessCmd( cmd );
	}

	LogonConsole::getInstance()._thread=NULL;
	return true;
}

//------------------------------------------------------------------------------
// Protected methods:
//------------------------------------------------------------------------------
// Process one command
void LogonConsole::ProcessCmd(char *cmd)
{
	typedef void (LogonConsole::*PTranslater)(char *str);
	struct SCmd
	{
		const char *name;
		PTranslater tr;
	};

	SCmd cmds[] =
	{

		{	"?", &LogonConsole::TranslateHelp}, {"help", &LogonConsole::TranslateHelp},
		{	"rehash", &LogonConsole::TranslateRehash},
		{	"reload", &LogonConsole::ReloadAccts},
		{	"netstatus", &LogonConsole::NetworkStatus},
		{	"shutdown", &LogonConsole::TranslateQuit}, 
		{	"exit", &LogonConsole::TranslateQuit}, 
		{	"OutputLevel", &LogonConsole::SetOutputLevel}, 
		{	"ol", &LogonConsole::SetOutputLevel}, 
		{	"ShowConfig", &LogonConsole::ShowConfig}, 
		{	"sc", &LogonConsole::ShowConfig}, 
		{	"ShowDetail", &LogonConsole::ShowDetail}, 
		{	"sd", &LogonConsole::ShowDetail}, 
	};

	char cmd2[80];
	strcpy(cmd2, cmd);
	for(size_t i = 0; i < strlen(cmd); ++i)
		cmd2[i] = tolower(cmd[i]);

	for (size_t i = 0; i < sizeof(cmds)/sizeof(SCmd); i++)
		if (strncmp(cmd2, cmds[i].name, strlen(cmds[i].name)) == 0)
		{
			(this->*(cmds[i].tr)) (cmd + strlen(cmds[i].name));
			return;
		}

	printf("无法解析的控制台命令！ (使用 \"help\" 命令获得帮助！).\n");
}

void LogonConsole::SetOutputLevel(char *str)
{
	int level = atoi(str);
	theConsole.SetLevel(level);
	theConsole.Notice("重新配置", "已更改控制台信息输出级别，级别=[%d]", level);
}

void LogonConsole::ShowConfig(char *str)
{
	theConsole.Print("当前服务器环境变量:\n");
	theConsole.Print("----------------------------------------------------------------------\n");
	theConsole.Print("    帐号数据库地址    ：%s\n", g_options_map["db_user_address"].as<std::string>().c_str());
	theConsole.Print("    帐号数据库端口    ：%s\n", g_options_map["db_user_port"].as<std::string>().c_str());
	theConsole.Print("    帐号数据库帐号    ：%s\n", g_options_map["db_user_username"].as<std::string>().c_str());
	theConsole.Print("    帐号数据库密码    ：%s\n", g_options_map["db_user_password"].as<std::string>().c_str());
	theConsole.Print("    游戏数据库地址    ：%s\n", g_options_map["db_game_address"].as<std::string>().c_str());
	theConsole.Print("    游戏数据库端口    ：%s\n", g_options_map["db_game_port"].as<std::string>().c_str());
	theConsole.Print("    游戏数据库帐号    ：%s\n", g_options_map["db_game_username"].as<std::string>().c_str());
	theConsole.Print("    游戏数据库密码    ：%s\n", g_options_map["db_game_password"].as<std::string>().c_str());
	theConsole.Print("    数据库线程数量    ：%d\n", g_options_map["db_thread_num"].as<unsigned int>());
	theConsole.Print("\n");
	theConsole.Print("    连接WS服务器的地址：%s\n", g_options_map["address"].as<std::string>().c_str());
	theConsole.Print("    连接WS服务器的端口：%d\n", g_options_map["port"].as<unsigned int>());
	theConsole.Print("\n");
	theConsole.Print("    控制台输出级别    ：%d\n", theConsole.GetLevel());
	theConsole.Print("    文件日志输出级别  ：%d\n", 0);
	theConsole.Print("    背包物品缓冲区大小：%d\n", omp::app::DP_ITEM_DATA_SIZE);
	theConsole.Print("    MYSQL记录行数限制 ：%d\n", MAX_ROWS_LIMIT);
	theConsole.Print("    MYSQL记录长度限制 ：%d\n", MAX_ROW_SIZE_LIMIT);
	theConsole.Print("\n");
}

//显示详细信息
void LogonConsole::ShowDetail(char *str)
{
	
	theConsole.Print("当前服务器运行状态:\n");
	theConsole.Print("----------------------------------------------------------------------\n");
	SYSTEMTIME time = theCountStat.m_server_start_time;
	theConsole.Print("    服务器启动时间    ：%04d-%02d-%02d %02d:%02d:%02d\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	GetLocalTime(&time);
	theConsole.Print("    当前服务器时间    ：%04d-%02d-%02d %02d:%02d:%02d\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	theConsole.Print("    缓存玩家总数      ：%d/%d\n", memcache_mgr::getInstance().m_char_count, memcache_mgr::getInstance().m_char_count);	//不需要LOCK
	theConsole.Print("    缓存玩家对象总数  ：%d/%d\n", memcache_mgr::getInstance().m_object_count, memcache_mgr::getInstance().m_object_count);
	const SYSTEMTIME& time1 = memcache_mgr::getInstance().m_prev_commit_time;
	theConsole.Print("    上次保存玩家时间  ：%04d-%02d-%02d %02d:%02d:%02d\n", time1.wYear, time1.wMonth, time1.wDay, time1.wHour, time1.wMinute, time1.wSecond);
	const SYSTEMTIME& time2 = memcache_mgr::getInstance().m_prev_commited_time;
	theConsole.Print("    上次成功提交时间  ：%04d-%02d-%02d %02d:%02d:%02d\n", time2.wYear, time2.wMonth, time2.wDay, time2.wHour, time2.wMinute, time2.wSecond);
	theConsole.Print("    上次成功提交数量  ：%d\n", memcache_mgr::getInstance().m_commit_save_count);
	theConsole.Print("    下次保存玩家间隔  ：%d ms\n", g_options_map["interval_save_to_db"].as<unsigned int>());

	if(strstr(str, "-a"))
	{
		memcache_mgr::MemcacheMap cache_map;
		memcache_mgr::getInstance().get_memcache_map(cache_map);
		memcache_mgr::MemcacheMapIt itea = cache_map.begin();
		theConsole.Line();
		for(itea; itea!=cache_map.end(); ++itea)
		{
			std::map<const char*, memcache_obj* >& object_map = itea->second;
			std::map<const char*, memcache_obj* >::iterator iteo = object_map.begin();
			for(iteo; iteo!=object_map.end(); ++iteo)
			{
				theConsole.Print("  玩家:[%d],", iteo->second->get_char_dbid());
				SYSTEMTIME& time = iteo->second->m_last_commited;
				theConsole.Print("对象[%s],标志:[%04X],更新:[%04d-%02d-%02d %02d:%02d:%02d]\n", iteo->first,iteo->second->get_update_flag(),
					time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
			}
		}

		if(cache_map.empty())
		{
			theConsole.Print("\n    当前玩家对象池中未发现任何对象!\n");
		}
	}

	theConsole.Print("\n");
}

void LogonConsole::ReloadAccts(char *str)
{
	theConsole.Error("重载配置", "该功能尚未实现！");
	//AccountMgr::getSingleton().ReloadAccounts(false);
	//IPBanner::getSingleton().Reload();
}

void LogonConsole::NetworkStatus(char *str)
{
	//sSocketMgr.ShowStatus();
	theConsole.Error("流量统计", "该功能尚未实现！");
}

// quit | exit
void LogonConsole::TranslateQuit(char *str)
{
	int delay = str != NULL ? atoi(str) : 5000;
	if(!delay)
		delay = 5000;
	else
		delay *= 1000;

	ProcessQuit(delay);
}
void LogonConsole::ProcessQuit(int delay)
{
	//mrunning = false;
}
//------------------------------------------------------------------------------
// help | ?
void LogonConsole::TranslateHelp(char *str)
{
	ProcessHelp(NULL);
}
void LogonConsole::ProcessHelp(char *command)
{
	if (command == NULL)
	{
		printf("Console:--------help--------\n");
		printf("	Help, ?       : 显示所有可使用的控制台命令.\n");
		//printf("	Reload        : 重新加载并应用DP服务器的配置项目(有些需要重启).\n");
		printf("	OutputLevel,ol: 设置控制台输出的日志级别！参数1=输出级别\n");
		printf("	LogLevel,   ll: 设置输出到文件的日志级别！参数1=记录级别\n");
		printf("	ShowConfig, sc: 显示当前DP服务器所有环境变量及配置信息\n");
		printf("	ShowDetail, sd: 显示当前DP服务器所有运行时状态信息\n");
		printf("\n");


		//printf("	Netstatus: Shows network status.\n");
		//printf("	Exit:      Closes the logonserver.\n\n");
	}
}
//------------------------------------------------------------------------------
bool LogonConsoleThread::kill = false;
CThreadState LogonConsoleThread::ThreadState;
time_t LogonConsoleThread::start_time;
int LogonConsoleThread::ThreadId;

LogonConsoleThread::LogonConsoleThread()
{
	kill=false;
}

LogonConsoleThread::~LogonConsoleThread()
{
}
