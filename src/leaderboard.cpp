#include "leaderboard.h"
#include "record.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Creates a new leaderboard from data in leaderboard.txt.
Leaderboard::Leaderboard() {
    record_list = pull_leaderboard();
}

// Reads the leaderboard from leaderboard.txt, converts it into a vector of Records, and returns it.
std::vector<Record> Leaderboard::pull_leaderboard() {

    std::vector<Record> pulled_record_list;
    std::ifstream input( path.c_str() );

    if (input.is_open()) {
        std::string line;
        Record record;

        while (std::getline(input, line)) {
            std::istringstream iss(line);
            iss >> record.score >> record.time >> std::ws &&std::getline(iss, record.name);
            pulled_record_list.push_back(record);
        }
    } else {
        throw std::runtime_error(path);
    }
    
    input.close();
    return pulled_record_list;
}

// Returns the rank that a new score would be placed on the leaderboard.
// If it falls out of the top 10, return -1 instead.
// record_list: The current list of records.
// points: The new score.
int Leaderboard::check_if_top_10(int points) {

    int rank = 1;
    for (std::vector<Record>::iterator i = record_list.begin(); i != record_list.end(); ++i) {
        if (i->score < points) {
            return rank;
        }
        rank++;
    }
    if (rank <= 10) {
        return rank;
    }
    // -1 if not in top 10
    return -1;
}

// Sorts the leaderboard by score. In case of a tie, the more recent scores are ranked lower.
// record_list: The leaderboard (vector of Records) to sort.
void Leaderboard::sort_leaderboard() {

    sort(record_list.begin(), record_list.end(),
         [](const Record &lhs, const Record &rhs) {
             return (lhs.score != rhs.score) ? lhs.score > rhs.score : lhs.time < rhs.time;
         });
}

// Adds a new record to the leaderboard.
// record_list: The leaderboard to add to.
// new_record: The record to add to the leaderboard.
void Leaderboard::add_to_leaderboard(Record new_record) {
    record_list.push_back(new_record);
}

// Refreshes the leaderboard such that it is sorted and only contains the top 10 records.
// record_list: The leaderboard to refresh.
void Leaderboard::revise_leaderboard() {
    sort_leaderboard();
    while (record_list.size() > 10) {
        record_list.pop_back();
    }
}

// Outputs the leaderboard data to leaderboard.txt.
// record_list: The leaderboard to output to leaderboard.txt.
void Leaderboard::push_leaderboard() {

    std::ofstream output( path.c_str() );

    if (output.is_open()) {
        for (std::vector<Record>::iterator i = record_list.begin(); i != record_list.end(); ++i) {
            output << i->score << " " << i->time << " " << i->name << std::endl;
        }
    }
    output.close();

}

// Returns the records stored in this Leaderboard.
std::vector<Record> Leaderboard::get_records() {
    return record_list;
}