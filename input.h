#pragma once

#include <iostream>
#include <string>
#include <vector>

/**
 Provides utilities for console input with error handling and input validation.
 */
namespace input {

/**
 Reads a line of input from the user and removes leading and trailing whitespace.
 @param prompt The prompt to display to the user.
 @return The trimmed input line from the user.
 */
std::string read_line(const std::string& prompt);

/**
 Prompts the user to enter 'y' or 'n' (case-insensitive).
 Repeats the prompt until a valid input is received.
 @param prompt The prompt to display to the user.
 @return true if the user enters 'y', false if the user enters 'n'.
 */
bool read_yn(const std::string& prompt);

/**
 Prompts the user to enter an integer within a specified range.
 Repeats the prompt until a valid integer within the range is received.
 @param prompt The prompt to display to the user.
 @param low The minimum acceptable integer value.
 @param high The maximum acceptable integer value.
 @return The valid integer entered by the user.
 */
int read_int(const std::string& prompt, int low, int high);

/**
 Displays a list of items with indices and prompts the user to select an index.
 Ensures the selected index is within the valid range (1 to number of items).
 @param items The list of items to display.
 @param title An optional title to display above the list.
 @return The zero-based index of the selected item.
 @throws std::runtime_error if the items list is empty.
 */
template <typename T>
int read_index(const std::vector<T>& items, const std::string& title = "") {
    int num_items = int(items.size());
    if (num_items == 0) {
        throw std::runtime_error("Items list is empty");
    }

    if (!title.empty()) {
        std::cout << title << "\n\n";
    }

    for (int i = 0; i < num_items; ++i) {
        std::cout << i + 1 << ". " << items[i] << "\n";
    }
    std::cout << "\n";
    
    std::string prompt = "Enter your selection (1-" + std::to_string(num_items) + "): ";
    int index = read_int(prompt, 1, num_items);
    return index - 1;
}

}
