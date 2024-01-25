#pragma once

// ripped this from another one of my projects ( not a public repo on here )..
// so if its messy, that's why. didn't expect to ever show this code, and it works sooo
// just removed the shit that wasn't needed for this project

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
		std::cout << std::vformat( format_str, format_args ) << std::endl;

		line_number++;
	}
}
