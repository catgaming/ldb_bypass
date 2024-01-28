#include "../includes.h"
#include "keyboard.h"

#include "../memory/memory.h"
#include "../memory/signature_scanner.h"

namespace patches
{
	bool keyboard_hook( )
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

		// set patch info
		const uintptr_t patch_address = keyboard_callback + 0x12; // function is 0x12 away from the patch location
		std::vector<uint8_t> patch_bytes = { 0xE9, 0xCD, 0x01, 0x00, 0x00, 0x90 };

		// adjust memory protection and apply patch
		DWORD old_protect;
		if ( VirtualProtectEx( memory::targ_handle, reinterpret_cast<LPVOID>( patch_address ), patch_bytes.size( ), PAGE_EXECUTE_READWRITE, &old_protect ) )
		{
			memory::write_bytes( patch_address, patch_bytes );
			VirtualProtectEx( memory::targ_handle, reinterpret_cast<LPVOID>( patch_address ), patch_bytes.size( ), old_protect, &old_protect );
		}
		else
		{
			logger::log( logger::LOG_ERROR, "failed to adjust memory protection" );
			return false;
		}

		logger::log( logger::INFO, "keyboard hook patched" );
		return true;
	}

	bool keyboard2_hook( )
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
		const auto keyboard2_callback =
			signature_scanner::find_pattern( globals::ldb_module_name, { "55 8B EC 83 7D ? ? 53 8B 5D ? 57" } );
		if ( !keyboard2_callback )
		{
			logger::log( logger::LOG_ERROR, "failed to find keyboard callback. outdated signature?" );
			return false;
		}

		// set patch info
		const uintptr_t		 patch_address = keyboard2_callback + 0xE;
		std::vector<uint8_t> patch_bytes   = { 0xEB, 0x7A };

		// adjust memory protection and apply patch
		DWORD old_protect;
		if ( VirtualProtectEx( memory::targ_handle, reinterpret_cast<LPVOID>( patch_address ), patch_bytes.size( ),
							   PAGE_EXECUTE_READWRITE, &old_protect ) )
		{
			memory::write_bytes( patch_address, patch_bytes );
			VirtualProtectEx( memory::targ_handle, reinterpret_cast<LPVOID>( patch_address ), patch_bytes.size( ),
							  old_protect, &old_protect );
		}
		else
		{
			logger::log( logger::LOG_ERROR, "failed to adjust memory protection" );
			return false;
		}

		logger::log( logger::INFO, "keyboard hook patched" );
		return true;
	}
}