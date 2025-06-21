#include "Check.h"

#include <QDate>

Check::Check(const std::shared_ptr<TravelAgency> &agency) : agency(agency) {
}

std::optional<CheckError> Check::operator()() {
    for (const auto customer: agency->getCustomers()) {
        for (auto travelA: customer->getTravels()) {
            for (auto travelB: customer->getTravels()) {
                if (travelA == travelB) continue;
                if (!checkTravelsDisjunct(travelA, travelB)) {
                    return CheckError(
                        customer->getId(),
                        travelA->getId(),
                        travelB->getId()
                    );
                }
            }
        }
    }
    return std::nullopt;
}

bool Check::checkTravelsDisjunct(std::shared_ptr<Travel> travelA, std::shared_ptr<Travel> travelB) {
    const auto aStart = travelA->getStart().value_or(QDate());
    const auto bStart = travelB->getStart().value_or(QDate());
    const auto aEnd = travelA->getEnd().value_or(QDate());
    const auto bEnd = travelB->getEnd().value_or(QDate());

    return aEnd < bStart || bEnd < aStart;
}
