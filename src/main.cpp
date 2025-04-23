#include <nlohmann/detail/exceptions.hpp>

#include "easyKey.h"
#include "TravelAgency.h"

int main() {
    Terminal::Cursor::clearScreen();

    const auto agency = new TravelAgency();
    Terminal::Menu menu("Praktikum 1 - Travel Agency");

    menu.addOption("Show bookings", [&agency] {
        agency->printBookings();
    });

    menu.addOption("Load JSON File", [&agency] {
        const auto fileName =
                Terminal::getString("Enter file name: ", [](auto ch) { return true; }, 255);
        Terminal::eraseprompt();
        while (true) {
            auto result = agency->readJsonFile(fileName);

            if (result.has_value()) {
                std::cout << std::endl << Terminal::Color::RED << "ERROR: " << Terminal::RESET << result.value() <<
                        std::endl << std::endl;
                Terminal::metaprompt("Fix the file and type y or type n to cancel");
                if (const auto yesNo = Terminal::getOneOfChr(Terminal::lowerAngle("").c_str(), {'y', 'n'}); yesNo == "y") {
                    Terminal::eraseprompt();
                    continue;
                }
            }

            break;
        }
    });

    menu.addOption("Load Binary File", [&agency] {
        const auto fileName = Terminal::getString("Enter file name: ", [](auto ch) { return true; }, 255);
        Terminal::eraseprompt();

        try {
            agency->readBinaryFile(fileName);
        } catch (std::exception &e) {
            Terminal::bail("Error while reading binary file: " + std::string(e.what()) + "\n");
        }
    });

    while (menu.askUser()) {
    }

    delete agency;
    return 0;
}
