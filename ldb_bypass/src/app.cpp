#include "includes.h"

#include "memory/memory.h"

#include "patches/input.h"
#include "patches/misc.h"

const char* globals::ldb_module_name = "LockDownBrowser.dll";

int main( )
{
	logger::initialize( );

	// attach to / load process
	if ( !memory::attach_to_process( "LockDownBrowser.exe" ) )
	{
		logger::log( logger::DEBUG, "LockDownBrowser.exe not running. loading..." );

		// may not hardcode this in the future. for now, i dont care
		if ( !memory::launch_and_attach( "C:\\Program Files (x86)\\Respondus\\LockDown Browser\\LockDownBrowser.exe" ) )
		{
			logger::log( logger::LOG_ERROR, "initialization failed" );
			return 1;
		}
	}

	// get main module
	MODULEENTRY32 ldb_module_info;
	logger::log( logger::INFO, "waiting for: {}", globals::ldb_module_name );
	while ( !memory::get_module_info( globals::ldb_module_name, ldb_module_info ) )
	{
		Sleep( 50 );
	}
	
	// apply patches
	if ( !patches::cb_keyboard( ) || !patches::cb_keyboard2( ) )
	{
		logger::log( logger::LOG_ERROR, "keyboard patch(es) failed" );
		return 1;
	}

	if ( !patches::cb_mouse( ) || !patches::cb_mouse2( ) )
	{
		logger::log( logger::LOG_ERROR, "mouse patch(es) failed" );
		return 1;
	}

	/*
	if ( !patches::shell_window( ) )
	{
		logger::log( logger::LOG_ERROR, "shell window fix failed" );
		return 1;
	}
	*/

	logger::log( logger::INFO, "detaching from process..." );
	memory::detach_from_process( );
	logger::shutdown( );
	return 0;
}