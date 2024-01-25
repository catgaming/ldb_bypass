#pragma once

namespace signature_scanner
{
	std::vector<std::vector<int>> convert_patterns( const std::vector<std::string>& pattern_strings );
	bool						  pattern_matches( uint8_t* base, const std::vector<int>& pattern );
	uintptr_t find_pattern( const char* module_name, const std::vector<std::string>& pattern_strings );
}