#pragma once

#include <TlHelp32.h>

namespace memory
{
	bool attach_to_process( const char* process_name );
	bool detach_from_process( );
	bool launch_and_attach( const char* targ_path );

	bool get_module_info( const char* module_name, MODULEENTRY32& module_info );

	std::vector<uint8_t> read_bytes( uintptr_t address, size_t size );
	bool				 write_bytes( uintptr_t address, const std::vector<uint8_t>& bytes );

	bool apply_patch( uintptr_t base_address, size_t offset, const std::vector<uint8_t>& patch_bytes );

	extern HANDLE targ_handle;	
}