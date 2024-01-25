#include "includes.h"

#include "memory/memory.h"

int main( )
{
	logger::initialize( );

	if ( !memory::attach_to_process( "LockDownBrowser.exe" ) )
	{
		logger::log( logger::LOG_ERROR, "LockDownBrowser.exe not running. opening it..." );

		// may not hardcode this in the future. for now, i dont care
		if ( !memory::launch_and_attach( "C:\\Program Files (x86)\\Respondus\\LockDown Browser\\LockDownBrowser.exe" ) )
		{
			logger::log( logger::LOG_ERROR, "initialization failed" );
			return 1;
		}
	}

	const char* main_module = "LockDownBrowser.dll";
	logger::log( logger::INFO, "waiting for: {}", main_module );
	while ( !memory::get_module_base( main_module ) )
	{
		Sleep( 50 );
	}

	logger::log( logger::INFO, "{} found", main_module );

	logger::shutdown( );
	return 0;
}