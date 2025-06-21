//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FlightBookingUi.h" resolved

#include "flightbookingui.h"
#include "ui_flightbookingui.h"


FlightBookingUi::FlightBookingUi(const std::shared_ptr<FlightBooking> &booking, const std::shared_ptr<BookingController> &bookingController,
                                 QWidget *parent) : QWidget(parent), ui(new Ui::FlightBookingUi), booking(booking),
                                                    bookingController(bookingController) {
    ui->setupUi(this);
    ui->fromDestination->setText(QString::fromStdString(booking->getFromDestination()));
    ui->toDestination->setText(QString::fromStdString(booking->getToDestination()));
    ui->airline->setText(QString::fromStdString(booking->getAirline()));
    ui->fromDate->setDate(parseDate(booking->getFromDate()));
    ui->price->setValue(booking->getPrice());
    ui->type->addItem("Economy", QVariant::fromValue(ECONOMY));
    ui->type->addItem("Premium Economy", QVariant::fromValue(PREMIUM_ECONOMY));
    ui->type->addItem("Business", QVariant::fromValue(BUSINESS));
    ui->type->addItem("First Class", QVariant::fromValue(FIRST_CLASS));
    ui->type->setCurrentIndex(booking->getBookingClass());

    ui->labelFrom->setTextFormat(Qt::MarkdownText);
    ui->labelTo->setTextFormat(Qt::MarkdownText);

    updateStationFrom(booking->getFromDestination());
    updateStationTo(booking->getToDestination());

    connect(ui->fromDestination, &QLineEdit::textChanged, this, &FlightBookingUi::onSetFromStation);
    connect(ui->toDestination, &QLineEdit::textChanged, this, &FlightBookingUi::onSetToStation);
    connect(ui->airline, &QLineEdit::textChanged, this, &FlightBookingUi::onSetAirline);
    connect(ui->fromDate, &QDateEdit::dateChanged, this, &FlightBookingUi::onSetDate);
    connect(ui->price, &QDoubleSpinBox::valueChanged, this, &FlightBookingUi::onSetPrice);
    connect(ui->type, &QComboBox::currentIndexChanged, this, &FlightBookingUi::onSetBookingClass);
}

FlightBookingUi::~FlightBookingUi() {
    delete ui;
}

void FlightBookingUi::onSetFromStation(QString station) {
    auto stationStr = station.toStdString();
    if (!updateStationFrom(stationStr)) return;
    booking->setFromDestination(stationStr);
    bookingController->onChange();
}

void FlightBookingUi::onSetToStation(QString station) {
    auto stationStr = station.toStdString();
    if (!updateStationTo(stationStr)) return;
    booking->setToDestination(station.toStdString());
    bookingController->onChange();
}

void FlightBookingUi::onSetAirline(QString airline) {
    booking->setAirline(airline.toStdString());
    bookingController->onChange();
}

void FlightBookingUi::onSetDate(QDate date) {
    booking->setFromDate(date.toString("yyyyMMdd").toStdString());
    bookingController->onChange();
}

void FlightBookingUi::onSetPrice(double amount) {
    booking->setPrice(amount);
    bookingController->onChange();
}

void FlightBookingUi::onSetBookingClass(int index) {
    booking->setBookingClass(static_cast<BookingClass>(index));
    bookingController->onChange();
}

bool FlightBookingUi::updateStationFrom(std::string &code) {
    const auto airport = this->bookingController->getAirport(code);
    if (airport.has_value()) {
        setOkState(ui->labelFrom, airport.value());
        return true;
    }

    setErrorState(ui->labelFrom);
    return false;
}

bool FlightBookingUi::updateStationTo(std::string &code) {
    const auto airport = this->bookingController->getAirport(code);
    if (airport.has_value()) {
        setOkState(ui->labelTo, airport.value());
        return true;
    }

    setErrorState(ui->labelTo);
    return false;
}


void FlightBookingUi::setErrorState(QLabel *label) {
    label->setText("<span style=\"color:#bf616a;\">Invalid IATA Code</span>");
}

void FlightBookingUi::setOkState(QLabel *label, std::shared_ptr<Airport> airport) {
    label->setText((airport->getName() + " (" + airport->getIso() + ")").data());
}
