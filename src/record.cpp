#include "record.h"

#include <ctime>
#include <string>

// Returns the current unix time.
int time_of_record_made() {

    std::time_t current_time_t = std::time(nullptr);
    std::tm *current_time_tm = std::localtime(&current_time_t);

    int current_time_int = static_cast<int>(std::mktime(current_time_tm));
    return current_time_int;
}

// Creates a Record. (static function)
// points: The score to store in the Record.
// username: The player name to store in the Record. Defaults to "Guest" if the player provides no name.
Record Record::get_new_record(int points, std::string username = "Guest") {
    int current_time = time_of_record_made();
    Record new_record = {points, current_time, username};
    return new_record;
}