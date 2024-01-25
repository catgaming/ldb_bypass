#pragma once

#include <format>
#include <iostream>
#include <mutex>
#include <string>

namespace logger
{
	enum log_level
	{
		DEBUG = 0,
		INFO,
		WARNING,
		LOG_ERROR,
		CRITICAL
	};

	bool initialize( );
	void shutdown( );

	std::string				  get_current_date_time( );
	std::string				  level_to_string( log_level level );
	std::string				  level_to_color( log_level level );
	std::string				  get_ansi_color_code( int r, int g, int b );
	std::string				  get_ansi_reset_code( );
	std::tuple<int, int, int> get_rainbow_color( int line );

	extern std::mutex	log_mutex;
	extern HANDLE		console_handle;
	extern unsigned int line_number;

	template <typename... arguments> void log( log_level level, const std::string& format_str, arguments... args )
	{
		log( level, format_str, true, args... );
	}

	template <typename... arguments>
	void log( log_level level, const std::string& format_str, bool newline, arguments... args )
	{
		std::lock_guard<std::mutex> guard( log_mutex );

		std::string current_time = get_current_date_time( );
		std::string level_str	 = level_to_string( level );

		// rainbow color for the datetime
		auto [ r, g, b ] = get_rainbow_color( line_number );
		std::cout << get_ansi_color_code( r, g, b ) << "[" << current_time << "] ";

		// color for the log level
		std::cout << level_to_color( level );
		std::cout << std::format( "[{:<4}]", level_str ) << get_ansi_reset_code( ) << " ";

		// use std::vformat with std::make_format_args for the actual log message
		auto format_args = std::make_format_args( args... );
		
		std::cout << std::vformat( format_str, format_args );
		if ( newline )
		{
			std::cout << '\n';
		}

		line_number++;
	}
}
