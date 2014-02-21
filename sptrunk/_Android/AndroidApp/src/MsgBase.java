import java.io.IOException;
import java.net.Socket;


// --登录验证
public class MsgBase 
{
	public static final int  MAX_PACK_LEN = 4096;			// --最大消息包长度 
	public static final byte CS_LOGON = 10;					// --登陆消息大类
	public static final byte S2C_SVR_READY_CMD= 0;			// --S2C:服务器准备
	public static final byte C2S_USER_AUTH_SYN= 1;			// --C2S:认证请求
	public static final byte S2C_USER_AUTH_ACK= 2;			// --S2C:认证回复
	public static final byte S2C_KICK_USER_CMD= 4;			// --S2C:踢出账号

	public byte byMsgCategory;								// --消息类别
	public byte byMsgProtocol;								// --消息协议号
	
	MsgBase()
	{
		byMsgCategory = 0;
		byMsgProtocol = 0;
	}
	
	public void _ReadPack( byte[] byStr )
	{
		byMsgCategory = (byte)(byStr[0]&0xff);
		byMsgProtocol = (byte)(byStr[1]&0xff);
	}
	
	
	public  int GetMsgLen()
	{
		return 2;
	}
	
	public static int ReadInt( byte[] byArr, int iOff )
	{
		int intStr =  byArr[0+iOff]&0xff | (byArr[1+iOff]&0xff) << 8 | (byArr[2+iOff]&0xff) << 16 | (byArr[3+iOff]&0xff) << 24;
		return intStr;
	}
	
	public static short ReadShort( byte[] byArr, int iOff )
	{
		int shortStr = byArr[iOff+0]&0xff | (byArr[iOff+1]&0xff) << 8 ;
		return (short)shortStr;
	}
	
	public static String ReadString( byte[] byArr, int iOff, int iLen )  throws IOException 
	{
		byte[] byTemp = new byte[iLen];
		for( int i=0; i<iLen; ++i )
		{
			byTemp[i] = byArr[iOff+i];
		}
		String str	= new String( byTemp, "UTF-8" );
		return str;
	}
	
    public static void WriteInt( byte[] byArr, int iVal, int iOff )
    {
    	byArr[iOff] 	= (byte)(iVal & 0xff);
    	byArr[1+iOff] 	= (byte)(iVal >> 8  & 0xff);
    	byArr[2+iOff] 	= (byte)(iVal >> 16 & 0xff);
    	byArr[3+iOff]   = (byte)(iVal >> 24 & 0xff);
    }
};


//--PushServer --> Client 版本通知
class MSG_S2C_SVR_READY_CMD extends MsgBase
{
	private static final int MAX_AUTHSTR_LEN = 32;
	MSG_S2C_SVR_READY_CMD()
	{
		byMsgCategory = CS_LOGON;
		byMsgProtocol = S2C_SVR_READY_CMD;
	}
	
	void _ReadPack( byte[] byStr, int iOff ) throws IOException 
	{
		sHighVer = ReadShort( byStr, iOff );
		sLowVer  = ReadShort( byStr, iOff+2 );
		dwEncKey = ReadInt( byStr, iOff+4 );
		strAuth	 = ReadString( byStr, iOff+8, MAX_AUTHSTR_LEN );
	}
	short		sHighVer;					// --高版本号
	short		sLowVer;					// --低版本号
	int   	    dwEncKey;					// --密钥
	String		strAuth;					// --验证串
};


//--Client --> PushServer：认证请求
class MSG_C2S_USER_AUTH_SYN extends MsgBase
{
	public static final int ID_MAX_LEN		= 20;
	public static final int PASSWD_MAX_LEN	= 24;
	
	public MSG_C2S_USER_AUTH_SYN()
	{
		byMsgCategory = CS_LOGON;
		byMsgProtocol = C2S_USER_AUTH_SYN;
	}
	
	public  int GetMsgLen()
	{
		return 46;
	}
	
	public void  _WritePack( Socket mySocket )throws IOException 
	{
		byte[] sendbyte = new byte[GetMsgLen()];
		MsgBase.WriteInt( sendbyte, GetMsgLen(), 0  );
		sendbyte[4] = CS_LOGON;
		sendbyte[5] = C2S_USER_AUTH_SYN;
		byte[] byId = szUserID.getBytes( "utf-8" );
		byte[] byPw = szPasswd.getBytes( "utf-8" );
		
		//System.arraycopy( byId, 0, sendbyte[6], 0, byId.length ); 
		//System.arraycopy( byPw, 0, sendbyte[6+ID_MAX_LEN], 0, byPw.length ); 
		for( int i =0; i<byId.length; ++ i )
		{
			sendbyte[6+i] =  byId[i];
		}
		for( int i =0; i<byPw.length; ++ i )
		{
			sendbyte[6+ID_MAX_LEN+i] =  byPw[i];
		}
		mySocket.getOutputStream().write( sendbyte, 0, GetMsgLen() );
	}
	
	public String	szUserID;	// --帐号
	public String	szPasswd;	// --密码
}


//--PushServer --> Client：认证请求
class MSG_S2C_USER_AUTH_ACK extends MsgBase
{
	public MSG_S2C_USER_AUTH_ACK()
	{
		byMsgCategory = CS_LOGON;
		byMsgProtocol = S2C_USER_AUTH_ACK;
	}
	
	void _ReadPack( byte[] byStr, int iOff ) throws IOException 
	{
		byRes = byStr[iOff];
	}
	byte	byRes;
}