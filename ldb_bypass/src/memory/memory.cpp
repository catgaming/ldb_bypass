#include "../includes.h"
#include "memory.h"

namespace memory
{
	HANDLE targ_handle;

	bool attach_to_process( const char* process_name )
	{
		DWORD		   proc_id = 0;
		const HANDLE   snapshot	   = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		PROCESSENTRY32 proc_entry;
		proc_entry.dwSize = sizeof( proc_entry );

		if ( Process32First( snapshot, &proc_entry ) )
		{
			do
			{
				if ( !strcmp( proc_entry.szExeFile, process_name ) )
				{
					proc_id = proc_entry.th32ProcessID;
					break;
				}
			} while ( Process32Next( snapshot, &proc_entry ) );
		}
		CloseHandle( snapshot );

		if ( !proc_id )
		{
			logger::log( logger::LOG_ERROR, "failed to find process" );
			return false;
		}

		// open a handle with all access
		logger::log( logger::INFO, "found process with id: {}", proc_id );
		targ_handle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, proc_id );

		if ( targ_handle == nullptr )
		{
			logger::log( logger::LOG_ERROR, "failed to open process handle" );
			return false;
		}

		logger::log( logger::INFO, "successfully opened process handle" );
		return true;
	}

	bool detach_from_process( )
	{
		if ( targ_handle != nullptr )
		{
			CloseHandle( targ_handle );
			targ_handle = nullptr;

			logger::log( logger::INFO, "successfully closed process handle" );
			return true;
		}

		logger::log( logger::LOG_ERROR, "cant detach from process: handle is null" );
		return false;
	}

	bool launch_and_attach( const char* targ_path )
	{
		STARTUPINFOA		startup_info;
		PROCESS_INFORMATION process_info;

		ZeroMemory( &startup_info, sizeof( startup_info ) );
		ZeroMemory( &process_info, sizeof( process_info ) );

		startup_info.cb = sizeof( startup_info );

		if ( !CreateProcessA( 
			targ_path, 
			nullptr, 
			nullptr,
			nullptr,
			FALSE, 
			0, 
			nullptr, 
			nullptr, 
			&startup_info, 
			&process_info ) )
		{
			logger::log( logger::LOG_ERROR, "failed to create process. error: {}", GetLastError() );
			return false;
		}

		logger::log( logger::INFO, "successfully created process" );

		targ_handle = process_info.hProcess;

		if ( targ_handle == nullptr )
		{
			logger::log( logger::LOG_ERROR, "failed to open process handle" );
			return false;
		}

		logger::log( logger::INFO, "successfully opened process handle" );

		return true;
	}

	/*
	uintptr_t get_module_base( const char* module_name )
	{
		MODULEENTRY32 module_entry;
		module_entry.dwSize	  = sizeof( MODULEENTRY32 );
		const HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetProcessId( targ_handle ) );
		if ( snapshot == INVALID_HANDLE_VALUE )
		{
			logger::log( logger::LOG_ERROR, "failed to create snapshot" );
			return 0;
		}

		if ( Module32First( snapshot, &module_entry ) )
		{
			do
			{
				if ( strcmp( module_entry.szModule, module_name ) == 0 )
				{
					CloseHandle( snapshot );
					logger::log( logger::INFO, "found module: {}", module_name );
					return reinterpret_cast<uintptr_t>( module_entry.modBaseAddr );
				}
			} while ( Module32Next( snapshot, &module_entry ) );
		}

		CloseHandle( snapshot );
		logger::log( logger::LOG_ERROR, "failed to find module: {}", module_name );
		return 0;
	}

	size_t get_module_size( const char* module_name )
	{
		MODULEENTRY32 module_entry;
		module_entry.dwSize	  = sizeof( MODULEENTRY32 );
		const HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetProcessId( targ_handle ) );
		if ( snapshot == INVALID_HANDLE_VALUE )
		{
			logger::log( logger::LOG_ERROR, "failed to create snapshot" );
			return 0;
		}

		if ( Module32First( snapshot, &module_entry ) )
		{
			do
			{
				if ( strcmp( module_entry.szModule, module_name ) == 0 )
				{
					CloseHandle( snapshot );
					return module_entry.modBaseSize;
				}
			} while ( Module32Next( snapshot, &module_entry ) );
		}

		CloseHandle( snapshot );
		logger::log( logger::LOG_ERROR, "failed to find module: {}", module_name );
		return 0;
	}
	*/

	bool get_module_info( const char* module_name, MODULEENTRY32& module_info )
	{
		module_info.dwSize = sizeof( MODULEENTRY32 );
		const HANDLE snapshot	   = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetProcessId( targ_handle ) );
		if ( snapshot == INVALID_HANDLE_VALUE )
		{
			logger::log( logger::LOG_ERROR, "failed to create snapshot" );
			return false;
		}

		bool module_found = false;
		if ( Module32First( snapshot, &module_info ) )
		{
			do
			{
				if ( strcmp( module_info.szModule, module_name ) == 0 )
				{
					module_found = true;
					break;
				}
			} while ( Module32Next( snapshot, &module_info ) );
		}

		CloseHandle( snapshot );

		if ( !module_found )
		{
			logger::log( logger::LOG_ERROR, "failed to find module: {}", module_name );
			return false;
		}

		logger::log( logger::LOG_ERROR, "module found: {}", module_name );
		return true;
	}

	std::vector<uint8_t> read_bytes( uintptr_t address, size_t size )
	{
		std::vector<uint8_t> buffer( size );
		SIZE_T				 bytes_read; // for dbg
		ReadProcessMemory( targ_handle, reinterpret_cast<LPCVOID>( address ), buffer.data( ), size, &bytes_read );
		return buffer;
	}

	bool write_bytes( uintptr_t address, std::vector<uint8_t>& bytes )
	{
		SIZE_T bytes_written; // for dbg
		return WriteProcessMemory( targ_handle, reinterpret_cast<LPVOID>( address ), bytes.data( ), bytes.size( ), &bytes_written );
	}
}