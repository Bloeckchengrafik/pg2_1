#include "easyKey.h"

#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <sstream>

namespace Terminal {
    namespace Cursor {
        void moveTo(const int x, const int y) {
            std::cout << "\033[" << x << ";" << y << "H";
        }

        void moveUp(const int n) {
            std::cout << "\033[" << n << "A";
        }

        void moveDown(const int n) {
            std::cout << "\033[" << n << "B";
        }

        void moveRight(const int n) {
            std::cout << "\033[" << n << "C";
        }

        void moveLeft(const int n) {
            std::cout << "\033[" << n << "D";
        }

        void clearScreen() {
            std::cout << "\033[2J";
        }

        void clearLine() {
            std::cout << "\033[2K";
        }
    }

    std::string styled(const std::string &text, const std::string &style) {
        return style + text + RESET;
    }

    void noEcho(const bool enable) {
        if (enable) {
            termios settings{};
            tcgetattr(STDIN_FILENO, &settings);
            settings.c_lflag &= ~ECHO;
            tcsetattr(STDIN_FILENO, TCSANOW, &settings);
        } else {
            termios settings{};
            tcgetattr(STDIN_FILENO, &settings);
            settings.c_lflag |= ECHO;
            tcsetattr(STDIN_FILENO, TCSANOW, &settings);
        }
    }
    char getCh() {
        termios oldt{}, newt{};
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        const char ch = static_cast<char>(getchar());
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }


    std::string getString(const char *prompt, const std::function<bool(const std::string &)> &validator, const int maxlen) {
        std::string input;
        std::cout << prompt;
        while (true) {
            if (const char ch = getCh(); ch == '\n' || ch == ' ' || ch == '\r') {
                if (validator(input)) {
                    return input;
                }
            } else if (ch == 127) {
                if (!input.empty()) {
                    input.pop_back();
                    std::cout << "\r" << std::string(input.size(), ' ') << "\r";
                    std::cout << prompt << input << " ";
                    Cursor::moveLeft();
                }
            } else if (isprint(ch) && validator(input + ch)) {
                input.push_back(ch);
                std::cout << "\r" << std::string(input.size(), ' ') << "\r";
                std::cout << prompt << input;

                if (input.size() == maxlen) {
                    return input;
                }
            }
        }
    }

    int getNumber(const char *prompt, IntRange *range, const int maxdigits) {
        const auto string = getString(prompt, [range](const std::string &input) {
            if (input.empty()) {
                return false;
            }

            if (input.size() == 1 && input[0] == '-') {
                return range->contains(-1);
            }

            for (size_t i = 0; i < input.size(); ++i) {
                if (i == 0 && input[i] == '-') {
                    continue;
                }
                if (!isdigit(input[i])) {
                    return false;
                }
            }

            if (const auto num = std::stoi(input); !range->contains(num)) {
                return false;
            }

            return true;
        }, maxdigits);

        return std::stoi(string);
    }

    std::string getOneOfChr(const char *prompt, const std::vector<char> &options) {
        const auto string = getString(prompt, [&options](const std::string &input) {
            if (input.size() != 1) {
                return false;
            }
            bool anyMatch = false;
            for (const auto &c: options) {
                if (c == input[0]) {
                    anyMatch = true;
                    break;
                }
            }

            return anyMatch;
        }, 1);

        return string;
    }

    void metaprompt(const char *prompt) {
        Cursor::clearLine();
        Cursor::moveUp();
        Cursor::clearLine();
        std::cout << "\r" << styled("┌ ", Color::GRAY) << prompt << std::endl;
    }

    std::string lowerAngle(const std::string &text) {
        return styled("└ ", Color::GRAY) + text;
    }

    void eraseprompt() {
        Cursor::clearLine();
        Cursor::moveUp();
        Cursor::clearLine();
        std::cout << "\r";
    }

    Menu::Menu(const std::string& title) : title(std::move(title)) {
        noEcho(true);
    }

    Menu::~Menu() {
        noEcho(false);
    }

    void Menu::addOption(const std::string &name, std::function<void()> action) {
        options.push_back({name, std::move(action)});
    }

    bool Menu::askUser() const {
        printMenu();
        const int num = getNumber("$ ", new IntRange(0, options.size()), 1);

        std::cout << std::endl;

        if (num == 0) {
            return false;
        }

        const auto &[_, action] = options[num - 1];
        action();

        return true;
    }

    void Menu::printMenu() const {
        std::cout <<
                styled(title, BOLD) << std::endl
                << std::endl;

        for (size_t i = 0; i < options.size(); i++) {
            std::cout << i + 1 << ") " << options[i].name << std::endl;
        }

        std::cout << "0) Exit" << std::endl << std::endl;
    }
}

IntRange::IntRange(const int min, const int max) : min(min), max(max) {
}

bool IntRange::contains(const int num) const {
    return num >= min && num <= max;
}
