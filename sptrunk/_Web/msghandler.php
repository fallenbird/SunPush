<?php

require_once( "msgbase.php" );


// --消息总处理接口
function msg_handler( $socketmsg )
{
	$msgLen  = readMsgInt($socketmsg);
	$msgbase = new MsgBase();
	$msgbase->_ReadPack( $socketmsg );
	
	switch( $msgbase->byMsgCategory )
	{
	   case WS_DATA:
	   {
	       switch( $msgbase->byMsgProtocol )
		   {
		       case S2W_PUSHSVRAUTH_SYN:
			   {
					Handler_S2W_PUSHSVRAUTH_SYN( $socketmsg );
			   }
			   break;
			   
			   case S2W_TEST_ORDER_SYN:
			   {
					Handler_S2W_TEST_ORDER_SYN( $socketmsg );
			   }
			   break;
		   }
	   }
	   break;
     
	}
}

// --确认连接成功
function ConfirmConnect( $mysocket )
{
	$msgclass = new MSG_W2S_WSCONNECTED_CMD();
	$msg = $msgclass->_WritePack();
	socket_write( $mysocket, $msg, strlen($msg) );
}


// --S2W_PUSHSVRAUTH_SYN处理函数
function Handler_S2W_PUSHSVRAUTH_SYN( $msgdata )
{
    $psmsg = new MSG_S2W_PUSHSVRAUTH_SYN( $msgdata ); 
	echo "[OUT] 收到消息"."  消息大类: ".$psmsg->byMsgCategory ."  消息协议号:".$psmsg->byMsgProtocol."S验证串: ".$psmsg->szAuthKey.'<br />';
	$msgclass = new MSG_W2S_AUTH_RESULT_ACK();
	if( $psmsg->szAuthKey == "DHF100FSKJF324XDA23K34QPZ6XVO190" )
	{
		echo "[OUT] 认证PushServer成功!".'<br />';
		$msgclass->byResult=1;
	}
	else
	{
		echo "[ERR] 认证PushServer失败!".'<br />';
		$msgclass->byResult=0;
	}
	$msg = $msgclass->_WritePack();
	//$_SESSION["s_sock"]
	socket_write( $msgdata, $msg, strlen($msg) ) or die(" [ERR] socket_write() 出错: " . socket_strerror(socket_last_error()).'<br />' );
	
	// --验证失败 关闭Socket
	if( 0 == $msgclass->byResult )
	{
		socket_close( $msgdata );
	}
}


// ------------------测试订单----------------
function Handler_S2W_TEST_ORDER_SYN( $msgdata )
{
	$recvmsg = new MSG_S2W_TEST_ORDER_SYN( $msgdata ); 
	$msgclass = new MSG_W2S_NEWORDERBRD_CMD();
	$msgclass->m_strPhone = $recvmsg->strPhone;
	$msgclass->iUsrConID  = $recvmsg->iUsrConID;
	$msgclass->iOrderNo   = $recvmsg->iOrderNo;
	$sendmsg = $msgclass->_WritePack();
	socket_write( $msgdata, $sendmsg, strlen($sendmsg) );
}

?>