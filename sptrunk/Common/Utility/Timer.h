#ifndef __TIMER_H__
#define __TIMER_H__
//=======================================================================================================================
//  Timer 类别
//	@history- 2005.2.28 : Timer based time 栏肺 官操菌促
//=======================================================================================================================
#pragma once

namespace util
{
class Timer
{
public: 
	Timer():m_dwExpireTime(0),m_dwIntervalTime(0),m_bCheckTime(false) {}
	~Timer(){}

public: 

	inline bool IsAlive() const { return m_bCheckTime ; }
	inline void SetTimer( DWORD dwIntervalTime )
	{
		m_dwIntervalTime	= dwIntervalTime;
		Reset();
	}
	inline void SetIntervalTime(DWORD dwIntervalTime)
	{
		m_dwIntervalTime	= dwIntervalTime;
	}
	inline DWORD GetIntervalTime() { return m_dwIntervalTime;	}
	DWORD GetRemainTime()
	{
		if (! IsExpired(false))
		{
			DWORD dwProcess = GetProcessTime();
			CHECKZERO(m_dwIntervalTime > dwProcess);
			return m_dwIntervalTime- dwProcess;
		}
		
		return 0;
	}

	DWORD  GetProcessTime()
	{
		DWORD dwCurTick = GetTickCount();
 		CHECKZERO( m_dwExpireTime > 0 && m_dwExpireTime >= m_dwIntervalTime );
 		CHECKZERO( dwCurTick >= (m_dwExpireTime - m_dwIntervalTime) );
		return dwCurTick >= m_dwExpireTime ? m_dwIntervalTime :( dwCurTick - (m_dwExpireTime - m_dwIntervalTime) );
	}
	// m_dwIntervalTime 矫埃 埃拜阑 弊措肺 蜡瘤窍绊 促矫 定时器甫 倒赴促.
	inline void Reset(/*m_dwIntervalTime*/)
	{
		m_dwExpireTime		= GetTickCount() + m_dwIntervalTime;
		EnableCheckTime();
	}

	// 定时器甫 resume
	inline void EnableCheckTime()
	{
		m_bCheckTime = true;
	}

	// 定时器甫 吝瘤茄促.
	inline void DisableCheckTime()
	{
		m_bCheckTime = false;
	}
	inline void ResetReCalcTimer(int iValue)
	{
		CHECK(safe_cast<int>(m_dwExpireTime) >= abs(iValue));
		if(!IsExpired(FALSE))
		{
			m_dwExpireTime	= m_dwExpireTime + iValue;	
		}
		
	}
	inline void DecreaseExpireTime( DWORD dwExpireTime )
	{
		CHECK( m_dwExpireTime >= dwExpireTime );
		m_dwExpireTime -= dwExpireTime;
	}
	// 父丰鸥烙捞 登扁鳖瘤狼 当前 柳青伏
	 float GetProgressRatio()
	{
		if(m_dwIntervalTime == 0)
			return 1.0f;
		//int dwProgressTime = GetTickCount() - (m_dwExpireTime - m_dwIntervalTime);
		return min(1.0f,((float)GetProcessTime() / (float)m_dwIntervalTime));
	}

	// problem : 捞 函数甫 啊阐 龋免且 版快 IsExpired() == TRUE啊 登绰 版快啊 积变促.
	// 恐唱搁 1 Tick捞扼档 坷瞒啊 积扁瘤 臼扁 困秦 IsExpired()龋免茄 矫埃阑 利侩窍瘤 臼绰促.
	// Therefore, 捞凡版快 IsExpired() == TRUE等 饶俊 Reset()甫 茄锅 龋免秦 霖促!
	// 是否超时 [2010-3-4 10:21:49 DZY]
	inline BOOL IsExpired( BOOL bReset=TRUE )
	{
		if( m_bCheckTime && GetTickCount() >= m_dwExpireTime ) 
		{
			if( bReset )
			{
				m_dwExpireTime = GetTickCount() + m_dwIntervalTime;
			}
			return TRUE;
		}
		else 
			return FALSE;
	}

	// 茄锅父 定时器甫 倒府扁 困秦
	inline BOOL IsExpiredManual( BOOL bReset=TRUE )
	{
		DWORD dwCurTime;
		if( m_bCheckTime && ( dwCurTime = GetTickCount() ) >= m_dwExpireTime ) 
		{
			if( bReset )
			{
				m_dwExpireTime = dwCurTime + m_dwIntervalTime;
			}
			DisableCheckTime();
			return TRUE;
		}
		else 
			return FALSE;
	}

	void InitTime()
	{
		m_dwExpireTime = 0;
	}

	inline DWORD GetExpireTime()	{ return m_dwExpireTime; }

private:
	bool		m_bCheckTime;
	DWORD		m_dwExpireTime;
	DWORD		m_dwIntervalTime;

};


/************************************************************************
三选一的策略
bool TimeOut()	超时后返回TURE，以后再也不返回TURE, 比如服务器启动后，5秒后读一次拍卖信息，以后再也不读了
bool TimeOver()	超时后，永远返回TURE，反沉迷，在线30分钟后，获得0经验
bool NextTime() 超时后，同样市场后再一次返回TURE.比如中毒后每5秒掉一次血
	当NextTime的时候	如果事件已经超过了每次的最短时长，剩余的时长还累计到下一次
	当NextTime的时候	如果事件已经超过了每次的最短时长，剩余的时长不累计到下一次

二选一
获得当前时间的时候，	类自己调用bool TimeOut()
外部传入时间TimeOut( DWORD dwCurTick )

下面的都不支持
区分一天的方式	24小时一天
午夜之后算新的一天

时间单位	秒		防沉迷	SetTimerSec() SetTimerTick SetTimerDay()
毫秒	攻击时间
天		每天只能做五次的任务	
************************************************************************/
struct TimeOutBev
{
	static void ResetInterval( const DWORD & /*dwCurTick*/ , DWORD & /*dwLastTick*/ , DWORD & dwIntervalTick )
	{
		dwIntervalTick = 0;
	}
};

struct TimeOverBev
{
	static void ResetInterval( const DWORD & /*dwCurTick*/ , DWORD & /*dwLastTick*/ ,DWORD & /*dwIntervalTick*/ ){}
};

struct UseAccumulate
{
	static void CalRemainTick( const DWORD &dwCurTick , DWORD &dwLastTick ,DWORD &dwIntervalTick )
	{
		CHECK( dwCurTick > dwLastTick );
		CHECK( 0 != dwIntervalTick );
		dwLastTick = dwLastTick+dwIntervalTick;
	}
};

struct NotUseAccumulate
{
	static void CalRemainTick( const DWORD &dwCurTick , DWORD &dwLastTick ,DWORD & /*dwIntervalTick*/  )
	{
		dwLastTick = dwCurTick ;
	}
};


template< class T_CalAcount>
struct TimeNextTime
{
	static void ResetInterval( const DWORD &dwCurTick , DWORD &dwLastTick ,DWORD &dwIntervalTick )
	{
		T_CalAcount::CalRemainTick( dwCurTick , dwLastTick , dwIntervalTick );
	}
};


template<class T_ResetIntervalBev >
class RecvTickTimer
{
public:
	RecvTickTimer()
	{
		m_dwLastTick = 0;
		m_dwIntervalTick = 0;
	}
	bool Init( DWORD dwIntervalTick , DWORD dwCurTick )
	{
		CHECKF( dwIntervalTick > 0 );
		m_dwIntervalTick = dwIntervalTick;
		m_dwLastTick = dwCurTick ;
		return true;
	}
	bool TimeEvent( DWORD dwCurTick )
	{
		if( 0 == m_dwIntervalTick )
			return false;

		ASSERT( dwCurTick >= m_dwLastTick );
		ASSERT( m_dwIntervalTick > 0 );
		if( dwCurTick - m_dwLastTick < m_dwIntervalTick )
			return false;

		T_ResetIntervalBev::ResetInterval( dwCurTick ,m_dwLastTick , m_dwIntervalTick );
		return true;
	}
private:
	DWORD m_dwLastTick ;
	DWORD m_dwIntervalTick ;
};

// 毫秒
template<class T_ResetIntervalBev >
class AutoTimer
{
public:
	bool InitTicks( DWORD dwIntervalTick )
	{
		return m_TimerRecvTick.Init( dwIntervalTick , GetTickCount() ) ;
	}
	bool TimeEvent()
	{
		return m_TimerRecvTick.TimeEvent( GetTickCount() ) ;
	}
private:
	RecvTickTimer<T_ResetIntervalBev> m_TimerRecvTick ;
};

template<class T_ResetIntervalBev >
class AutoTimerSecs
{
public:
	bool InitSecs( DWORD dwSecs )
	{
		return m_TimerRecvTick.InitTicks( dwSecs * CLOCKS_PER_SEC ) ;
	}
	bool TimeEvent()
	{
		return m_TimerRecvTick.TimeEvent() ;
	}
private:
	AutoTimer<T_ResetIntervalBev>  m_TimerRecvTick ;
};

// 超时后只触发一次 [2009-12-14 14:53:54  ]
typedef AutoTimer<TimeOutBev> TimeOutTimer;//使用毫秒
typedef AutoTimerSecs<TimeOutBev> TimeOutTimerSecs;//使用秒
typedef AutoTimerSecs<TimeNextTime<UseAccumulate> > TimeNextTimeSecsAccumulate;// 积累触发次数，使用秒 [2010-3-30 11:27:52  ]
typedef AutoTimerSecs<TimeNextTime<NotUseAccumulate> > TimeNextTimeSecsNoAccumulate;// 不积累触发次数，使用秒 [2010-3-30 11:27:52  ]
}//End of namespace util





#endif //__TIMER_H__

