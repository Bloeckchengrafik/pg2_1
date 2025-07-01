#pragma once
#include <QWidget>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>

#include "../BookingController.h"
#include "../../geojson/IntoGeoJsonElements.h"
#include "../../serde/prelude.h"

QDate parseDate(const std::string &date);
std::string formatDate(const std::string &date);

class Booking : public std::enable_shared_from_this<Booking>, public IntoGeoJsonElements {
protected:
    std::string id;
    double price;
    std::string fromDate;
    std::string toDate;
    std::vector<std::string> predecessors;

public:
    Booking(
        std::string id,
        const double price,
        std::string fromDate,
        std::string toDate,
        std::vector<std::string> predecessors
    ) : id(std::move(id)), price(price), fromDate(std::move(fromDate)), toDate(std::move(toDate)), predecessors(std::move(predecessors)) {
    }

    virtual ~Booking() = default;

    virtual std::string showDetails() = 0;
    virtual void showEditor(std::shared_ptr<BookingController> changeController) = 0;
    virtual QIcon getIcon() = 0;

    [[nodiscard]] double getPrice();
    [[nodiscard]] std::string &getId();
    [[nodiscard]] std::string &getFromDate();
    [[nodiscard]] std::string &getToDate();
    std::vector<std::string> &getPredecessors();

    void setPrice(double price);
    void setFromDate(std::string fromDate);
    void setToDate(std::string toDate);
};

void serializePredecessors(serde::Encoder *encoder, const std::vector<std::string> &predecessors);
std::vector<std::string> deserializePredecessors(serde::Decoder *decoder);