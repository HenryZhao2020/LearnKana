#pragma once

#include <string>

/**
 Provides utilities for string operations.
 */
namespace strutil {

/**
 Removes leading and trailing white space from the given string.
 @param str The input string from which white space will be removed.
 @return A new string with leading and trailing white space removed.
 If the input string is empty or contains only white space, an empty string is returned.
 */
std::string strip(const std::string& str);

/**
 Converts all letters of the given string to lowercase.
 @param str The input string to be converted to lowercase.
 @return A new string with all alphabetic characters converted to lowercase.
 */
std::string tolower(const std::string& str);

}
