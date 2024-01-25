#pragma once

namespace memory
{
	bool attach_to_process( const char* process_name );
	bool detach_from_process( );
	bool launch_and_attach( const char* targ_path );

	uintptr_t get_module_base( const char* module_name );
	std::vector<uint8_t> read_bytes( uintptr_t address, size_t size );
	bool write_bytes( uintptr_t address, std::vector<uint8_t>& bytes );

	extern HANDLE targ_handle;
}