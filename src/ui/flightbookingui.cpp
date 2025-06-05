//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FlightBookingUi.h" resolved

#include "flightbookingui.h"
#include "ui_flightbookingui.h"


FlightBookingUi::FlightBookingUi(FlightBooking *booking, ChangeController *changeController,
                                 QWidget *parent) : QWidget(parent), ui(new Ui::FlightBookingUi), booking(booking),
                                                    changeController(changeController) {
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
    booking->setFromDestination(station.toStdString());
    changeController->onChange();
}

void FlightBookingUi::onSetToStation(QString station) {
    booking->setToDestination(station.toStdString());
    changeController->onChange();
}

void FlightBookingUi::onSetAirline(QString airline) {
    booking->setAirline(airline.toStdString());
    changeController->onChange();
}

void FlightBookingUi::onSetDate(QDate date) {
    booking->setFromDate(date.toString("yyyyMMdd").toStdString());
    changeController->onChange();
}

void FlightBookingUi::onSetPrice(double amount) {
    booking->setPrice(amount);
    changeController->onChange();
}

void FlightBookingUi::onSetBookingClass(int index) {
    booking->setBookingClass(static_cast<BookingClass>(index));
    changeController->onChange();
}
