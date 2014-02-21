
#pragma once
#define BOOST_NO_RTTI
//#define BOOST_ASIO_ENABLE_CANCELIO

#include <boost/asio/local/stream_protocol.hpp>
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <tchar.h>
#include <set>
#include <map>
#include <list>
#include <queue>


#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#include "Console/ConsoleO.h"

