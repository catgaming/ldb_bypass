#pragma once

namespace memory
{
	bool attach_to_process( const char* process_name );
	bool detach_from_process( );

	std::vector<uint8_t> read_bytes( uintptr_t address, size_t size );
	bool write_bytes( uintptr_t address, std::vector<uint8_t>& bytes );
}