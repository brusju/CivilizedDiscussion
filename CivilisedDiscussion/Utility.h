#pragma once
#pragma warning(disable : 4996)

#include <string>
#include <iostream>
#include <memory>


/**
 * Convert all std::strings to const char* using constexpr if (C++17)
 */
template<typename T>
auto convert(T&& t) {
	if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value) {
		return std::forward<T>(t).c_str();
	}
	else {
		return std::forward<T>(t);
	}
}

/**
 * printf like formatting for C++ with std::string
 * Original source: https://stackoverflow.com/a/26221725/11722
 */
template<typename ... Args>
std::string stringFormatInternal(const std::string& format, Args&& ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args) ...) + 1;
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1);
}

template<typename ... Args>
std::string string_format(std::string fmt, Args&& ... args) {
	return stringFormatInternal(fmt, convert(std::forward<Args>(args))...);
}
