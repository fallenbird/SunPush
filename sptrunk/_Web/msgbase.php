<?php

require_once( "netcommon.php" );


// --消息大类常量
define( "CS_LOGON", 10 );						// --移动端<->PushSvr消息大类
define( "WS_DATA" , 50 );						// --WebSvr<->PushSvr消息大类


// --消息协议号
define ( "W2S_WSCONNECTED_CMD", 0);				// --W2S:Web服务器连接通知
define ( "S2W_PUSHSVRAUTH_SYN", 1);             // --S2W:推送服务器认证请求
define ( "W2S_AUTH_RESULT_ACK", 2);             // --W2S:推送服务器认证反馈
define ( "W2S_NEWORDERBRD_CMD", 4);             // --W2S:新增订单广播请求
define ( "S2W_TEST_ORDER_SYN", 52);             // --W2S:新增订单广播请求


// --消息基类
class MsgBase
{
   var  $byMsgCategory;							// --消息类别
   var	$byMsgProtocol;							// --消息协议号

   function _ReadPack( $msgdata )
   {
		$this->byMsgCategory = readMsgByte( $msgdata );
		$this->byMsgProtocol = readMsgByte( $msgdata ); 
   }
   
   function _WritePack( $msgpack )
   {
   }
   
    function __construct()
   {
		$this->byMsgCategory = 0;
		$this->byMsgProtocol = 0; 
   }
}


// --WebServer接收PushServer连接后发送的第一条消息
class MSG_W2S_WSCONNECTED_CMD extends MsgBase
{
	var $szUserID;	
    var $szPasswd;

	function __construct()
   {
		$this->byMsgCategory = WS_DATA;
		$this->byMsgProtocol = W2S_WSCONNECTED_CMD;
   }
   function _WritePack()
   {
		$msgpack = pack("ICC", 2, $this->byMsgCategory, $this->byMsgProtocol ); 
		return $msgpack;
   }
}


// --PushServer发送给WebServer的第一个条消息
class MSG_S2W_PUSHSVRAUTH_SYN extends MsgBase
{
    var $szAuthKey;
	function __construct( $msgdata )
   {
		$this->byMsgCategory = WS_DATA;
		$this->byMsgProtocol = S2W_PUSHSVRAUTH_SYN;
		$this->szAuthKey = readMsgStr( $msgdata, 32 );
   }
}


// --PushServer发送给WebServer的第一个条消息
class MSG_W2S_AUTH_RESULT_ACK extends MsgBase
{
    var $byResult;
	function __construct()
   {
		$this->byMsgCategory = WS_DATA;
		$this->byMsgProtocol = W2S_AUTH_RESULT_ACK;
   }
   
    function _WritePack()
   {
		$msgpack = pack("ICCC", 3, $this->byMsgCategory, $this->byMsgProtocol, $this->byResult ); 
		return $msgpack;
   }
}



// --WebServer发送给PushServer的订单广播 
class MSG_W2S_NEWORDERBRD_CMD extends MsgBase
{
	var $iUsrConID;
	var $iOrderNo;
	var	$strPhone;
	var	$arrDrivers;
	function __construct()
   {
		$this->byMsgCategory = WS_DATA;
		$this->byMsgProtocol = W2S_NEWORDERBRD_CMD;
   }
   function _WritePack()
   {
		$msgpack = pack("ICCII", 10, $this->byMsgCategory, $this->byMsgProtocol, $this->iUsrConID, $this->iOrderNo ); 
		//$msgpack = $msgpack.$this->strPhone;
		return $msgpack;
   }
}


// --测试订单消息
class MSG_S2W_TEST_ORDER_SYN  extends MsgBase
{
   	var	$byOrderType;
	var $iUsrConID;
	var $iOrderNo;
	var	$iUserPosX;
	var	$iUserPosY;
	var	$strPhone;
	var	$strTest;
	function __construct( $msgdata )
   {
		$this->byMsgCategory = WS_DATA;
		$this->byMsgProtocol = S2W_TEST_ORDER_SYN;
		$this->byOrderType = readMsgByte( $msgdata );
		$this->iUsrConID = readMsgInt( $msgdata );
		$this->iOrderNo = readMsgInt( $msgdata );
		$this->iUserPosX = readMsgInt( $msgdata );
		$this->iUserPosY = readMsgInt( $msgdata );
		$this->strPhone = readMsgStr( $msgdata, 11 );
		$this->strTest = readMsgStr( $msgdata, 168 );
   }
}

?>