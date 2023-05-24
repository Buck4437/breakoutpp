#include "general_utils.h"
#include "ncu.h"

#include <string>

// Repeats a string multiple times and returns it.
// a: The string to repeat.
// b: The number of times to repeat.
// For example, strmulti("ab", 3) returns "ababab".
std::string strmulti(std::string a, int b) {
    std::string output = "";
    while (b--) {
        output += a;
    }
    return output;
}