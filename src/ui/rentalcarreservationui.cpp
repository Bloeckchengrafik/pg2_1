//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RentalCarReservationUi.h" resolved

#include "rentalcarreservationui.h"
#include "ui_rentalcarreservationui.h"
#include "../backend/booking/Booking.h"


RentalCarReservationUi::RentalCarReservationUi(const std::shared_ptr<RentalCarReservation> &res, const std::shared_ptr<BookingController> &changeController, QWidget *parent) :
    QWidget(parent), ui(new Ui::RentalCarReservationUi), res(res), changeController(changeController) {
    ui->setupUi(this);
    ui->companyName->setText(QString::fromStdString(res->getCompany()));
    ui->pickupDate->setDate(parseDate(res->getFromDate()));
    ui->pickupLocation->setText(QString::fromStdString(res->getPickupLocation()));
    ui->returnDate->setDate(parseDate(res->getToDate()));
    ui->returnLocation->setText(QString::fromStdString(res->getReturnLocation()));
    ui->price->setValue(res->getPrice());

    connect(ui->companyName, &QLineEdit::textChanged, this, &RentalCarReservationUi::onSetCompany);
    connect(ui->pickupLocation, &QLineEdit::textChanged, this, &RentalCarReservationUi::onSetPickupLocation);
    connect(ui->returnLocation, &QLineEdit::textChanged, this, &RentalCarReservationUi::onSetReturnLocation);
    connect(ui->pickupDate, &QDateEdit::dateChanged, this, &RentalCarReservationUi::onSetFromDate);
    connect(ui->returnDate, &QDateEdit::dateChanged, this, &RentalCarReservationUi::onSetToDate);
    connect(ui->price, &QDoubleSpinBox::valueChanged, this, &RentalCarReservationUi::onSetPrice);
}

RentalCarReservationUi::~RentalCarReservationUi() {
    delete ui;
}

void RentalCarReservationUi::onSetCompany(QString company) {
    res->setCompany(company.toStdString());
    changeController->onChange();
}

void RentalCarReservationUi::onSetPickupLocation(QString location) {
    res->setPickupLocation(location.toStdString());
    changeController->onChange();
}

void RentalCarReservationUi::onSetReturnLocation(QString location) {
    res->setReturnLocation(location.toStdString());
    changeController->onChange();
}

void RentalCarReservationUi::onSetFromDate(QDate date) {
    res->setFromDate(date.toString("yyyyMMdd").toStdString());
    changeController->onChange();
}

void RentalCarReservationUi::onSetToDate(QDate date) {
    res->setToDate(date.toString("yyyyMMdd").toStdString());
    changeController->onChange();
}

void RentalCarReservationUi::onSetPrice(double amount) {
    res->setPrice(amount);
    changeController->onChange();
}
