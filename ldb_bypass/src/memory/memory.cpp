#include "../includes.h"
#include "memory.h"

namespace memory
{
	HANDLE targ_handle;

	bool attach_to_process( const char* process_name )
	{
		DWORD		   proc_id = 0;
		const HANDLE   snap	   = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		PROCESSENTRY32 proc_entry;
		proc_entry.dwSize = sizeof( proc_entry );

		if ( Process32First( snap, &proc_entry ) )
		{
			do
			{
				if ( !strcmp( proc_entry.szExeFile, process_name ) )
				{
					proc_id = proc_entry.th32ProcessID;
					break;
				}
			} while ( Process32Next( snap, &proc_entry ) );
		}
		CloseHandle( snap );

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
		STARTUPINFOA startup_info;
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
}