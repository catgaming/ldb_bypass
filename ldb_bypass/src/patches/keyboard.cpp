#include "../includes.h"
#include "keyboard.h"

#include "../memory/signature_scanner.h"

bool keyboard_hook( )
{
	/*
	 *	ORIGINAL BYTES:
	 *	0F 85 CC 01 00 00 -> JNZ rel
	 *
	 *	PATCH BYTES:
	 *	E9 CD 01 00 00 90 -> JMP rel
	 */


}