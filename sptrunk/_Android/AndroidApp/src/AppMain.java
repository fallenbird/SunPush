import java.net.Socket;
import java.io.IOException;


/**
 * @author Administrator
 *
 */
public class AppMain 
{
	
	// -- 
	public static void main( String[] args )throws IOException, InterruptedException
	{
		System.out.println( "[OUT] 连接PushServer..." );
		
		// --创建IO Socket
		String ipaddr = "127.0.0.1";
		Socket mySocket = new Socket( ipaddr, 44405 );
		System.out.println( "[OUT] 连接成功! socket = "+ mySocket );
		
		// --处理消息
		while( true )
		{
			MsgHandler.HandleMsg( mySocket );
			Thread.sleep( 50 );
		}
		
		// --关闭socket
		//mySocket.close();
	}

}
