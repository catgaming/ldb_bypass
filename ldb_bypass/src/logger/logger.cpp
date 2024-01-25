#include "../includes.h"
#include "logger.h"

#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <sstream>

namespace logger
{
	std::mutex log_mutex;
	HANDLE	   console_handle = nullptr;

	unsigned int line_number = 0;

	bool initialize( )
	{
		AllocConsole( );
		freopen_s( reinterpret_cast<FILE**>( stdout ), "CONOUT$", "w", stdout );
		SetConsoleTitleA( "ldb bypass ^-^ enjoy" );

		console_handle = GetStdHandle( STD_OUTPUT_HANDLE );
		if ( console_handle == INVALID_HANDLE_VALUE )
		{
			return false;
		}

		// Enable ANSI escape codes (needed for 24bit)
		DWORD console_mode;
		if ( GetConsoleMode( console_handle, &console_mode ) )
		{
			console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode( console_handle, console_mode );
		}

		return true;
	}

	void shutdown( )
	{
		log( logger::INFO, "[logger] Shutdown complete." );
		FreeConsole( );
	}

	std::string level_to_string( log_level level )
	{
		switch ( level )
		{
		case DEBUG:
			return "DBG";
		case INFO:
			return "INFO";
		case WARNING:
			return "WARN";
		case LOG_ERROR:
			return "ERR";
		case CRITICAL:
			return "CRIT";
		default:
			return "UNK";
		}
	}

	std::string level_to_color( log_level level )
	{
		switch ( level )
		{
		case DEBUG:
			return get_ansi_color_code( 64, 64, 64 );
		case INFO:
			return get_ansi_color_code( 128, 255, 0 );
		case WARNING:
			return get_ansi_color_code( 225, 255, 0 );
		case LOG_ERROR:
			return get_ansi_color_code( 255, 16, 32 );
		case CRITICAL:
			return get_ansi_color_code( 255, 128, 128 );
		default:
			return get_ansi_color_code( 255, 255, 0255 );
		}
	}

	std::string get_current_date_time( )
	{
		std::ostringstream ss;
		std::time_t		   t = std::time( nullptr );
		std::tm			   tm;
		localtime_s( &tm, &t );

		ss << std::put_time( &tm, "%Y-%m-%d %H-%M-%S" );
		return ss.str( );
	}

	std::string get_ansi_color_code( int r, int g, int b )
	{
		return std::format( "\x1b[38;2;{};{};{}m", r, g, b );
	}

	// generate a rainbow color based on the line number
	std::tuple<int, int, int> get_rainbow_color( int line )
	{
		// simple rainbow color generation (can be improved for smoother gradient)
		const double frequency = 0.1;
		int			 r		   = static_cast<int>( std::sin( frequency * line + 0 ) * 64 + 64 );
		int			 g		   = static_cast<int>( std::sin( frequency * line + 2 ) * 32 + 128 );
		// int b = static_cast<int>( std::sin( frequency * line + 4 ) * 127 + 128 );
		int b = 255;
		return { r, g, b };
	}

	std::string get_ansi_reset_code( )
	{
		return "\x1b[0m";
	}
}