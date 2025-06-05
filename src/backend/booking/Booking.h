#pragma once
#include <QWidget>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>

#include "../ChangeController.h"
#include "../../serde/prelude.h"

QDate parseDate(const std::string &date);
std::string formatDate(const std::string &date);

class Booking {
protected:
    std::string id;
    double price;
    std::string fromDate;
    std::string toDate;

public:
    Booking(
        std::string id,
        const double price,
        std::string fromDate,
        std::string toDate
    ) : id(std::move(id)), price(price), fromDate(std::move(fromDate)), toDate(std::move(toDate)) {
    }

    virtual ~Booking() = default;

    virtual std::string showDetails() = 0;
    virtual void showEditor(ChangeController *changeController) = 0;
    virtual QIcon getIcon() = 0;

    [[nodiscard]] double getPrice();
    [[nodiscard]] std::string &getId();
    [[nodiscard]] std::string &getFromDate();
    [[nodiscard]] std::string &getToDate();

    void setPrice(double price);
    void setFromDate(std::string fromDate);
    void setToDate(std::string toDate);
};
