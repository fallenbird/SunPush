<?php
	require_once( "msghandler.php" );

	/*function asc2bin($temp) 
	{
        $len = strlen($temp);
        for($i=0; $i<$len; $i++) 
		{
            $data .= sprintf( "%08b", ord(substr($temp, $i, 1)) );
        }
        return $data;
	}
	*/
	error_reporting(E_ALL);
	
	// --超时时间
	set_time_limit(0);
	ob_implicit_flush();
	
	$address = '127.0.0.1';
	$port = 33066;

	// --创建socket
	$socket = socket_create( AF_INET, SOCK_STREAM, SOL_TCP );
	if( $socket < 0 )
	{
	    echo "socket_create() failed: reason: " . socket_strerror($sock) . "\n";
	}
	echo "[OUT] Socket创建成功!".'<BR />';

	// --邦定端口
	$ret = socket_bind($socket, $address, $port );
	if( $ret < 0 )
	{
	    echo "socket_bind() failed: reason: " . socket_strerror($ret) . "\n";
	}

	// --监听
	echo "[OUT] 开始监听端口: ".$port.'<BR />';
	$ret = socket_listen($socket,5);
	if( $ret < 0 )
	{
	    echo "socket_listen() failed: reason: " . socket_strerror($ret) . "\n";
	}
	
	// --循环
	while( true )
	{
		// --接受连接，另一个Socket来处理通信
		$spawn = socket_accept( $socket );
		if( $spawn )
		{
			echo "[OUT] 成功建立一个连接".'<BR />';
			//$_SESSION["s_sock"] = $spawn;
			ConfirmConnect( $spawn );
		}
		else
		{
			echo "[ERR] 成功建立一个连接".'<BR />';
		}
		
		// --消息处理
		while( true )
		{
			msg_handler( $spawn );
		}
		// --关闭socket
		//socket_close( $spawn ); 
	}
	
	socket_close( $socket ); 
?>