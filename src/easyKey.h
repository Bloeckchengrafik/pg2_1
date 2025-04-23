/*
 * This is a *rough* port of https://github.com/elektrofuzzis/ftSwarm/blob/dev/src/ftswarm-core/src/easyKey.cpp
 * to linux. Original code by Stefan Fuss & Christian Bergschneider.
 */
#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

/*
 * This class is a simple wrapper around a range of integers. It is used to check if a given number is within the range
 * of the min and max values.
 */
class IntRange {
public:
    IntRange(int min, int max);
    [[nodiscard]] bool contains(int num) const;
private:
    int min;
    int max;
};

namespace Terminal {
    // Constants for text styles
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string UNDERLINE = "\033[4m";

    namespace Color {
        const std::string RED = "\033[31m";
        const std::string GREEN = "\033[32m";
        const std::string YELLOW = "\033[33m";
        const std::string BLUE = "\033[34m";
        const std::string MAGENTA = "\033[35m";
        const std::string CYAN = "\033[36m";
        const std::string WHITE = "\033[37m";
        const std::string GRAY = "\033[90m";
    }

    namespace Background {
        const std::string RED = "\033[41m";
        const std::string GREEN = "\033[42m";
        const std::string YELLOW = "\033[43m";
        const std::string BLUE = "\033[44m";
        const std::string MAGENTA = "\033[45m";
        const std::string CYAN = "\033[46m";
        const std::string WHITE = "\033[47m";
        const std::string GRAY = "\033[100m";
    }

    namespace HIBackground {
        const std::string RED = "\033[101m";
        const std::string GREEN = "\033[102m";
        const std::string YELLOW = "\033[103m";
        const std::string BLUE = "\033[104m";
        const std::string MAGENTA = "\033[105m";
        const std::string CYAN = "\033[106m";
        const std::string WHITE = "\033[107m";
        const std::string GRAY = "\033[47m";
    }

    // Functions to move the cursor around the terminal
    namespace Cursor {
        void moveTo(int x, int y);
        void moveUp(int n = 1);
        void moveDown(int n = 1);
        void moveRight(int n = 1);
        void moveLeft(int n = 1);
        void clearScreen();
        void clearLine();
    }

    // Functions to style text
    std::string styled(const std::string &text, const std::string &style);

    // curses-like terminal functions
    void noEcho(bool enable);
    char getCh();

    // Functions to simplify getting user input
    std::string getString(const char *prompt, const std::function<bool(const std::string &)> &validator, int maxlen);
    int getNumber(const char *prompt, IntRange *range, int maxdigits);
    std::string getOneOfChr(const char *prompt, const std::vector<char> &options);

    // Metaprompt adds a prompt before the actual prompt
    void metaprompt(const char *prompt);
    std::string lowerAngle(const std::string &text);
    void eraseprompt();

    // Print an error message and exit the program
    template<typename ... Args>
    void bail(const std::string& format, Args&&... args) {
        std::ostringstream oss;
        oss << std::endl << styled("[BAIL!] ", Color::RED) << format << " ";
        ((oss << args << " "), ...);
        std::cerr << oss.str() << std::endl;
        exit(1);
    }

    // Option in the menu
    struct MenuOption_t {
        std::string name;
        std::function<void()> action;
    };

    // Some menu with options and callbacks
    class Menu {
    public:
        explicit Menu(const std::string& title);
        ~Menu();

        // Add an option to the menu
        void addOption(const std::string &name, std::function<void()> action);

        // Run the menu. False if the user chose to exit the menu
        [[nodiscard]] bool askUser() const;

    private:
        std::string title;
        std::vector<MenuOption_t> options;

        void printMenu() const;
    };
}