#include "../includes.h"
#include "misc.h"

namespace patches
{
	bool shell_window( )
	{
		HWND shell_wnd = FindWindowA( "Shell_TrayWnd", NULL );
		if ( !shell_wnd )
		{
			logger::log( logger::LOG_ERROR, "failed to find shell window" );
			return false;
		}

		if ( !ShowWindow( shell_wnd, SW_SHOW ) )
		{
			logger::log( logger::LOG_ERROR, "failed to show shell window" );
			return false;
		}

		logger::log( logger::INFO, "successfully showed shell window" );
		return true;
	}
}