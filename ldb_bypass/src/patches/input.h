#pragma once

namespace patches
{
	// the second callbacks are set for more restrictive modes

	bool cb_keyboard( );
	bool cb_keyboard2( );

	bool cb_mouse( );
	bool cb_mouse2( );
	bool cb_shell( );

	bool cldb_do_some_stuff( ); // only useful if we also call the function rapidly
}