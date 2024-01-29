#include "../includes.h"
#include "input.h"

#include "../memory/memory.h"
#include "../memory/signature_scanner.h"

namespace patches
{
	bool cb_keyboard( )
	{
		/*
		 *	ORIGINAL BYTES:
		 *	0F 85 CC 01 00 00 -> JNZ rel
		 *
		 *	PATCH BYTES:
		 *	E9 CD 01 00 00 90 -> JMP rel
		 */

		/*
		 *	FUNCTION SIG:
		 *	55 8B EC 83 EC ? 83 7D ? ? 53
		 *	PATCH OFF: 0x12
		 *
		 *	XREF Signature #1 @ 11035: 68 ? ? ? ? EB ? 68 ? ? ? ? 6A ? FF 15
		 *	XREF Signature #2 @ 110DA: 68 ? ? ? ? EB ? 68 ? ? ? ? 6A ? FF D7 6A
		 *
		 *	will have to implement offset handling if i use the xref signatures.
		 *	and in this case, if the function sig breaks, its 100% likely that my patch offset will break too
		 */

		// get the function address
		const auto keyboard_callback = signature_scanner::find_pattern( globals::ldb_module_name, { "55 8B EC 83 EC ? 83 7D ? ? 53" } );
		if ( !keyboard_callback )
		{
			logger::log( logger::LOG_ERROR, "failed to find keyboard callback. outdated signature?" );
			return false;
		}

		// set patch info and call apply_patch
		if ( const std::vector<uint8_t> patch_bytes = { 0xE9, 0xCD, 0x01, 0x00, 0x00, 0x90 };
			 !memory::apply_patch( keyboard_callback, 0x12, patch_bytes ) )
		{
			logger::log( logger::LOG_ERROR, "failed to apply patch" );
			return false;
		}

		logger::log( logger::INFO, "keyboard hook patched" );
		return true;
	}

	bool cb_keyboard2( )
	{
		/*
		 *	ORIGINAL BYTES:
		 *	75 7A -> JNZ short rel
		 *
		 *	PATCH BYTES:
		 *	EB 7A -> JMP short rel
		 */

		/*
		 *	FUNCTION SIG:
		 *	55 8B EC 83 7D ? ? 53 8B 5D ? 57
		 *  PATCH OFF: 0xE
		 *
		 *	XREF Signature #1 @ 110E1: 68 ? ? ? ? 6A ? FF D7 6A
		 *	XREF Signature #2 @ 1103C: 68 ? ? ? ? 6A ? FF 15 ? ? ? ? 8B C8
		 */

		// get the function address
		const auto keyboard2_callback = signature_scanner::find_pattern( globals::ldb_module_name, { "55 8B EC 83 7D ? ? 53 8B 5D ? 57" } );
		if ( !keyboard2_callback )
		{
			logger::log( logger::LOG_ERROR, "failed to find keyboard2 callback. outdated signature?" );
			return false;
		}

		// set patch info and call apply_patch
		if ( const std::vector<uint8_t> patch_bytes = { 0xEB, 0x7A };
			 !memory::apply_patch( keyboard2_callback, 0xE, patch_bytes ) )
		{
			logger::log( logger::LOG_ERROR, "failed to apply patch" );
			return false;
		}

		logger::log( logger::INFO, "keyboard2 hook patched" );
		return true;
	}

	bool cb_mouse( )
	{
		/*
		 *	ORIGINAL BYTES:
		 *	75 4F -> JNZ short rel
		 *
		 *	PATCH BYTES:
		 *	EB 4F -> JMP short rel
		 */

		/*
		 *	FUNCTION SIG:
		 *	55 8B EC 8B 4D ? 85 C9 79
		 *  PATCH OFF: 0x24
		 *
		 *	XREF Signature #1 @ 11212: 68 ? ? ? ? 6A ? FF 15 ? ? ? ? 33 C9
		 *	XREF Signature #2 @ 11111: 68 ? ? ? ? EB ? 68 ? ? ? ? 6A ? FF D7 39 35
		 */

		// get the function address
		const auto mouse_callback = signature_scanner::find_pattern( globals::ldb_module_name, { "55 8B EC 8B 4D ? 85 C9 79" } );
		if ( !mouse_callback )
		{
			logger::log( logger::LOG_ERROR, "failed to find mouse callback. outdated signature?" );
			return false;
		}

		// set patch info and call apply_patch
		if ( const std::vector<uint8_t> patch_bytes = { 0xEB, 0x4F };
			 !memory::apply_patch( mouse_callback, 0x24, patch_bytes ) )
		{
			logger::log( logger::LOG_ERROR, "failed to apply patch" );
			return false;
		}

		logger::log( logger::INFO, "mouse hook patched" );
		return true;
	}
}