#include "../includes.h"
#include "signature_scanner.h"

#include "memory.h"
#include <sstream>

namespace signature_scanner
{
	std::vector<std::vector<int>> convert_patterns( const std::vector<std::string>& pattern_strings )
	{
		std::vector<std::vector<int>> converted_patterns;
		for ( const auto& str : pattern_strings )
		{
			std::vector<int>   pattern;
			std::istringstream stream( str );
			std::string		   byte;

			while ( std::getline( stream, byte, ' ' ) )
			{
				if ( byte == "?" )
				{
					pattern.push_back( -1 );
				}
				else
				{
					pattern.push_back( std::stoi( byte, nullptr, 16 ) );
				}
			}
			converted_patterns.push_back( pattern );
		}
		return converted_patterns;
	}

	bool pattern_matches( uint8_t* base, const std::vector<int>& pattern )
	{
		for ( size_t i = 0; i < pattern.size( ); ++i )
		{
			if ( pattern[ i ] != -1 && base[ i ] != static_cast<uint8_t>( pattern[ i ] ) )
			{
				return false;
			}
		}
		return true;
	}

	uintptr_t find_pattern( const char* module_name, const std::vector<std::string>& pattern_strings )
	{
		MODULEENTRY32 module_info;
		if ( !memory::get_module_info( module_name, module_info ) )
		{
			logger::log( logger::LOG_ERROR, "[signature_scanner] failed to get module information for {}",
						 module_name );
			return 0;
		}

		const uintptr_t base		  = reinterpret_cast<uintptr_t>( module_info.modBaseAddr );
		const size_t	size		  = module_info.modBaseSize;
		uintptr_t		found_address = 0;
		size_t			found_count	  = 0;

		const auto search_patterns = convert_patterns( pattern_strings );

		for ( size_t i = 0; i < search_patterns.size( ); ++i )
		{
			if ( i > 0 && found_address )
			{
				return found_address; // already found a pattern, no need to continue
			}

			const auto& pattern = search_patterns[ i ];
			for ( uintptr_t current = base; current < base + size - pattern.size( ); ++current )
			{
				std::vector<uint8_t> bytes = memory::read_bytes( current, pattern.size( ) );
				if ( pattern_matches( bytes.data( ), pattern ) )
				{
					logger::log( logger::INFO, "[signature_scanner] pattern[{}] found @ {:X}", i, current );

					if ( found_count == 0 )
					{
						found_address = current;
					}
					found_count++;
				}
			}
		}

		if ( found_count == 0 )
		{
			logger::log( logger::CRITICAL, "[signature_scanner] pattern not found" );
			return 0;
		}
		if ( found_count > search_patterns.size( ) )
		{
			logger::log( logger::CRITICAL, "[signature_scanner] pattern found too many times" );
			return 0;
		}

		return found_address;
	}
}