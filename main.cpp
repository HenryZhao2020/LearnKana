#include "input.h"
#include "strutil.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <random>

// Script types
const std::string HIRA = "Hiragana";
const std::string KATA = "Katakana";
const std::string BOTH = "Both";

/**
 Represents a kana and its possible romaji variations.
 @example {"あ", {"a"}}, {"い", {"i"}}, {"う", {"u"}}
 @note Some kanas have multiple romaji for different pronunciations and inputs.
 @example {"を", {"o", "wo"}}, {"づ", {"zu", "du"}}
 */
struct kana_pair {
    std::string kana;
    std::vector<std::string> romajis;
};

/**
 Stores script components as vectors of 'kana_pair'.
 Components are ordered as follows:
 0: Base kanas
 1: Dakuon kanas
 2: Base combos
 3: Dakuon combos
 */
std::vector<std::vector<kana_pair>> hiras(4), katas(4);

/**
 Clears the console screen.
 */
void clear_screen() {
#if defined(_WIN32)
    system("cls");
#else
    system("clear");
#endif
}

/**
 Loads kana and romaji pairs from a file into a vector.
 @param pairs The vector to store the kana-romaji pairs.
 @param filename The file to read from;
 each line includes a kana, the number of romajis, and the romajis.
 @throws std::runtime_error If the file cannot be opened.
 */
void load_kana_pairs(std::vector<kana_pair>& pairs, const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string kana;
    int num_romajis;

    while (fin >> kana >> num_romajis) {
        kana_pair pair;
        pair.kana = kana;
        pair.romajis.resize(num_romajis);
        for (int i = 0; i < num_romajis; ++i) {
            fin >> pair.romajis[i];
        }
        pairs.push_back(pair);
    }
}

/**
 Prints kana-romaji pairs for debugging.
 @param pairs The vector of 'kana_pair' to print.
 */
void print_kana_pairs(const std::vector<kana_pair>& pairs) {
    for (const auto& pair : pairs) {
        std::cout << pair.kana << ": ";
        for (const auto& romaji : pair.romajis) {
            std::cout << romaji << " ";
        }
        std::cout << "\n";
    }
}

/**
 Prompts the user to select a script type.
 @return The selected script.
 */
const std::string& select_script() {
    static const std::vector<std::string> scripts = {HIRA, KATA, BOTH};
    int index = input::read_index(scripts, "Scripts");
    return scripts[index];
}

/**
 Prompts the user to select quiz components.
 @param script The script type to filter components.
 @return A vector of 'kana_pair' for the quiz.
 */
std::vector<kana_pair> select_components(const std::string& script) {
    static const std::vector<std::string> comps
        = {"Base Kana", "Dakuon", "Base Combo", "Dakuon Combo"};
    static const std::vector<std::string> hira_samples
        = {"あ, い, う, え, お", "が, ぎ, ぐ, げ, ご", "きゃ, きゅ, きょ", "ぎゃ, ぎゅ, ぎょ"};
    static const std::vector<std::string> kata_samples
        = {"ア, イ, ウ, エ, オ", "ガ, ギ, グ, ゲ, ゴ", "キャ, キュ, キョ", "ギャ, ギュ, ギョ"};

    std::vector<kana_pair> quiz;

    for (int i = 0; i < 4; ++i) {
        // Display the sample kana based on the selected script for each component
        // Show hiragana if the script is set to 'Both'
        const auto& sample = (script == KATA ? kata_samples : hira_samples)[i];
        std::cout << "\n" << i + 1 << ". " << sample << "\n";

        if (!input::read_yn("Include in the test? (Y/n) ")) {
            continue;
        }

        if (script != KATA) {
            quiz.insert(quiz.end(), hiras[i].begin(), hiras[i].end());
        }
        if (script != HIRA) {
            quiz.insert(quiz.end(), katas[i].begin(), katas[i].end());
        }
    }

    return quiz;
}

/**
 Prompts users to enter romaji for kana and provides feedback.
 @param quiz A vector of 'kana_pair' with questions and answers.
 @return The updated vector with incorrect answers.
 @throws std::runtime_error If the quiz vector is empty.
 */
std::vector<kana_pair>& start_quiz(std::vector<kana_pair>& quiz) {
    if (quiz.empty()) {
        throw std::runtime_error("Quiz must not be empty!");
    }

    // Shuffle the questions
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(quiz.begin(), quiz.end(), g);

    int total = int(quiz.size());
    for (int i = total - 1; i >= 0; --i) {
        const auto& kana = quiz[i].kana;
        const auto& romajis = quiz[i].romajis;
        
        std::stringstream prompt;
        prompt << "(" << total - i << "/" << total << ") " << kana << ": ";
        std::string ans = strutil::tolower(input::read_line(prompt.str()));

        // Remove question if answered correctly
        if (std::find(romajis.begin(), romajis.end(), ans) != romajis.end()) {
            std::cout << "Correct!\n\n";
            quiz.erase(quiz.begin() + i);
        // Wrong question remains for later redos; display correct answers
        } else {
            std::cout << "Wrong! It is " << romajis[0];
            for (size_t j = 1; j < romajis.size(); ++j) {
                std::cout << " or " << romajis[j];
            }
            std::cout << "\n\n";
        }
    }

    // Result
    int num_wrongs = int(quiz.size());
    int num_corrects = total - num_wrongs;
    int accuracy = 100 * num_corrects / total;

    clear_screen();
    std::cout << "Result\n\n";
    std::cout << "Correct: " << num_corrects << "\n";
    std::cout << "Wrong: " << num_wrongs << "\n";
    std::cout << "Accuracy: " << accuracy << "%\n";

    // Display mistakes
    if (num_wrongs > 0) {
        std::cout << "\nMistakes: " << quiz[0].kana;
        for (int i = 1; i < num_wrongs; ++i) {
            std::cout << ", " << quiz[i].kana;
        }
        std::cout << "\n";
    }

    return quiz;
}

int main(int argc, char* argv[]) {
#if defined(_WIN32)
    // Display UTF-8 characters on Windows
    SetConsoleOutputCP(65001);
#endif

    // Set executable path as current path
    try {
        std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
    } catch (...) {
        std::cerr << "Could not load the current path.\n";
        return 1;
    }

    // Load kana from file
    try {
        const std::vector<std::string> comps = {"base", "dakuon", "base_combo", "dakuon_combo"};
        for (int i = 0; i < 4; ++i) {
            load_kana_pairs(hiras[i], "kanas/hira_" + comps[i] + ".txt");
            load_kana_pairs(katas[i], "kanas/kata_" + comps[i] + ".txt");
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    clear_screen();
    std::cout << "Learn Japanese Kana\n";
    std::cout << "Developed by Henry Zhao\n\n";

    // Select script
    const std::string& script = select_script();
    clear_screen();

    // Select component
    std::cout << "Components\n";
    std::vector<kana_pair> quiz = select_components(script);
    while (quiz.empty()) {
        std::cout << "\nSelect at least 1 component! Try again.\n";
        quiz = select_components(script);
    }

    // Quiz
    clear_screen();
    std::cout << "Kana Quiz\n";
    std::cout << "Direction: Enter the romaji for each kana\n\n";
    std::vector<kana_pair>& wrongs = start_quiz(quiz);

    // Quiz-redo
    int num_redos = 0;
    while (!wrongs.empty()) {
        if (!input::read_yn("Redo mistakes? (Y/n) ")) {
            break;
        }

        clear_screen();
        std::cout << "Kana Quiz Redo #" << ++num_redos << "\n\n";
        wrongs = start_quiz(wrongs);
    }

    std::cout << "\nBest of luck with your Japanese learning journey!\n";
    return 0;
}
