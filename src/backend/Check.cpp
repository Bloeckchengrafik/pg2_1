#include "Check.h"

#include <QDate>

#include "booking/HotelBooking.h"
#include "booking/RentalCarReservation.h"
#include "digraph/algorithm.h"

Check::Check(const std::shared_ptr<TravelAgency> &agency) : agency(agency) {
}

std::vector<CheckError> Check::operator()() {
    std::vector<CheckError> errors;
    std::shared_ptr<CheckConfiguration> config = agency->getCheckConfigController()->getConfig();
    for (const auto customer: agency->getCustomers()) {
        for (auto travelA: customer->getTravels()) {
            if (config->travelOverlaps) {
                for (auto travelB: customer->getTravels()) {
                    if (travelA == travelB) continue;
                    if (!checkTravelsDisjunct(travelA, travelB)) {
                        errors.emplace_back(
                            customer->getId(),
                            "Travels " + std::to_string(travelA->getId()) + " and " + std::to_string(travelB->getId()) +
                            " overlap"
                        );
                        break;
                    }
                }
            }

            if (config->tooLittleHotels) {
                if (!checkNoMissingHotels(travelA)) {
                    errors.emplace_back(
                        customer->getId(),
                        "Travel " + std::to_string(travelA->getId()) + " has missing hotel"
                    );
                }
            }

            if (config->tooManyHotels) {
                if (!checkNoOverlappingHotels(travelA)) {
                    errors.emplace_back(
                        customer->getId(),
                        "Travel " + std::to_string(travelA->getId()) + " has overlapping hotels"
                    );
                }
            }

            if (config->tooManyRentalCars) {
                if (!checkNoOverlappingRentalCars(travelA)) {
                    errors.emplace_back(
                        customer->getId(),
                        "Travel " + std::to_string(travelA->getId()) + " has overlapping rental cars"
                    );
                }
            }
        }
    }
    return errors;
}

bool Check::checkTravelsDisjunct(std::shared_ptr<Travel> travelA, std::shared_ptr<Travel> travelB) {
    const auto aStart = travelA->getStart().value_or(QDate());
    const auto bStart = travelB->getStart().value_or(QDate());
    const auto aEnd = travelA->getEnd().value_or(QDate());
    const auto bEnd = travelB->getEnd().value_or(QDate());

    return aEnd <= bStart || bEnd <= aStart;
}

bool Check::checkNoMissingHotels(std::shared_ptr<Travel> travel) {
    const auto graph = travel->getGraph();
    const auto list = TopologicalSort(graph);

    std::vector<std::shared_ptr<Booking> > accommodations;
    for (const int vertexId: list) {
        if (auto booking = graph->getVertexValue(vertexId); std::dynamic_pointer_cast<RentalCarReservation>(booking) == nullptr) {
            accommodations.push_back(booking);
        }
    }

    if (accommodations.empty()) {
        return false;
    }

    for (size_t i = 0; i < accommodations.size() - 1; ++i) {
        const auto current = accommodations[i];

        if (const auto next = accommodations[i + 1]; current->getToDate() != next->getFromDate()) {
            return false;
        }
    }

    return parseDate(accommodations.front()->getFromDate()) == travel->getStart() && parseDate(accommodations.back()->getToDate()) == travel->getEnd();
}

bool Check::checkNoOverlappingHotels(std::shared_ptr<Travel> travel) {
    const auto graph = travel->getGraph();
    const auto list = TopologicalSort(graph);

    std::vector<std::shared_ptr<Booking>> accommodations;
    for (const int vertexId : list) {
        if (auto booking = graph->getVertexValue(vertexId);
            std::dynamic_pointer_cast<RentalCarReservation>(booking) == nullptr) {
            accommodations.push_back(booking);
        }
    }

    if (accommodations.size() <= 1) {
        return true;
    }

    for (size_t i = 0; i < accommodations.size(); ++i) {
        const auto current = accommodations[i];
        const auto currentFrom = parseDate(current->getFromDate());
        const auto currentTo = parseDate(current->getToDate());

        for (size_t j = i + 1; j < accommodations.size(); ++j) {
            const auto other = accommodations[j];
            const auto otherFrom = parseDate(other->getFromDate());
            const auto otherTo = parseDate(other->getToDate());

            if (!(currentTo <= otherFrom || otherTo <= currentFrom)) {
                return false;
            }
        }
    }

    return true;
}

bool Check::checkNoOverlappingRentalCars(std::shared_ptr<Travel> travel) {
    const auto graph = travel->getGraph();
    const auto list = TopologicalSort(graph);

    std::vector<std::shared_ptr<Booking>> rentalCars;
    for (const int vertexId : list) {
        if (auto booking = graph->getVertexValue(vertexId);
            std::dynamic_pointer_cast<RentalCarReservation>(booking) != nullptr) {
            rentalCars.push_back(booking);
        }
    }

    if (rentalCars.size() <= 1) {
        return true;
    }

    for (size_t i = 0; i < rentalCars.size(); ++i) {
        const auto current = rentalCars[i];
        const auto currentFrom = parseDate(current->getFromDate());
        const auto currentTo = parseDate(current->getToDate());

        for (size_t j = i + 1; j < rentalCars.size(); ++j) {
            const auto other = rentalCars[j];
            const auto otherFrom = parseDate(other->getFromDate());
            const auto otherTo = parseDate(other->getToDate());

            if (!(currentTo <= otherFrom || otherTo <= currentFrom)) {
                return false;
            }
        }
    }

    return true;
}
