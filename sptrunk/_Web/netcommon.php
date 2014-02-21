	<?php
	
	function readMsgInt( $mySocket )
	{
		$newData=socket_read($mySocket, 4 ) or die("Could not read input int ERR: ". socket_strerror( socket_last_error() )." Socket数据 ".$newData );
		$intstr = ord($newData{0}) | (ord($newData{1})<<8) | (ord($newData{2})<<16) | (ord($newData{3})<<24);
		return $intstr;
	}
	
	
	function readMsgShort( $msgData )
	{
		$newData=socket_read($msgData, 2 ) or die("Could not read input short".$msgData);
		$intstr = ord($newData{0}) | (ord($newData{1})<<8);
		return $intstr;
	}
	
	
	function readMsgByte( $msgData )
	{
		$newData=socket_read($msgData, 1 ) or die("Could not read input byte".$msgData);
		$intstr = ord($newData);
		return $intstr;
	}
	
	
	function readMsgStr( $msgData, $strlen )
	{
		if( $strlen <= 0 || $strlen > 8192 )
		{
			echo("错误的字符串长度！ \n");
			return 0;
		}
		$newData=socket_read($msgData, $strlen ) or die("Could not read input string".$msgData);
		return $newData;
	}

	?>