import java.io.IOException;
import java.net.Socket;


public class MsgHandler 
{
	public static void HandleMsg( Socket mySocket ) throws IOException
	{
		// --读包长度
		int iMsgLen = ReadPackHead( mySocket );
		byte[] byMsgBytes = new byte[MsgBase.MAX_PACK_LEN];
		
		// --读出消息头
		mySocket.getInputStream().read( byMsgBytes, 0, iMsgLen );
		MsgBase MsgHead = new MsgBase();
		MsgHead._ReadPack( byMsgBytes );
		
		// --读包
		switch( MsgHead.byMsgCategory )
		{
		case MsgBase.CS_LOGON:
			{
				switch( MsgHead.byMsgProtocol )
				{
					case MsgBase.S2C_SVR_READY_CMD:
					{
						Handler_S2C_SVR_READY_CMD( mySocket, byMsgBytes );
					}
					break;
					
					case MsgBase.S2C_USER_AUTH_ACK:
					{
						Handler_S2C_USER_AUTH_ACK( byMsgBytes );
					}
					break;
				}
			}
			break;
		}
	}
	
	// --读取消息长度函数 
	private static int ReadPackHead( Socket mySocket )throws IOException
	{
		int str0 = mySocket.getInputStream().read();
		int str1 = mySocket.getInputStream().read();
		int str2 = mySocket.getInputStream().read();
		int str3 = mySocket.getInputStream().read();
		int intStr =  str0&0xff | (str1&0xff) << 8 | (str2&0xff) << 16 | (str3&0xff) << 24;
		return intStr;
	}
	
	// --pushsvr->client: 版本验证消息处理函数
	private static void Handler_S2C_SVR_READY_CMD( Socket mySocket, byte[] byMsgBytes )throws IOException
	{
		// --读包
		MSG_S2C_SVR_READY_CMD rcvMsg = new MSG_S2C_SVR_READY_CMD();
		rcvMsg._ReadPack( byMsgBytes, 2 );
		System.out.println( "[OUT] 认证版本成功! 当前最新版本: "+rcvMsg.sHighVer+"."+rcvMsg.sLowVer );
		System.out.println( "[OUT] 认证码:"+rcvMsg.dwEncKey + "  校验串:" + rcvMsg.strAuth );
		
		// --发送
		MSG_C2S_USER_AUTH_SYN sendMsg = new MSG_C2S_USER_AUTH_SYN();
		sendMsg.szUserID = "jakesun";
		sendMsg.szPasswd = "123456";
		sendMsg._WritePack( mySocket );
	}
	
	// --pushsvr->client: 登录反馈消息处理函数
	private static void Handler_S2C_USER_AUTH_ACK( byte[] byMsgBytes  )throws IOException
	{
		MSG_S2C_USER_AUTH_ACK rcvMsg = new MSG_S2C_USER_AUTH_ACK();
		rcvMsg._ReadPack( byMsgBytes, 2 );
		if( 1 == rcvMsg.byRes )
		{
			System.out.println( "[OUT] 用户登陆成功！" );
		}
		else
		{
			System.out.println( "[OUT] 用户登陆失败！" );
		}
	}
}
