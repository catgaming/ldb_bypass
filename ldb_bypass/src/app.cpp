#include "includes.h"

#include "memory/memory.h"

int main( )
{
	logger::initialize( );

	memory::launch_and_attach( "C:\\Program Files (x86)\\Respondus\\LockDown Browser\\LockDownBrowser.exe" );

	logger::shutdown( );
	return 0;
}