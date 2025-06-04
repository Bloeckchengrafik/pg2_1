//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RentalCarReservationUi.h" resolved

#include "rentalcarreservationui.h"
#include "ui_rentalcarreservationui.h"
#include "../backend/booking/Booking.h"


RentalCarReservationUi::RentalCarReservationUi(RentalCarReservation *res, QWidget *parent) :
    QWidget(parent), ui(new Ui::RentalCarReservationUi) {
    ui->setupUi(this);
    ui->companyName->setText(QString::fromStdString(res->getCompany()));
    ui->pickupDate->setDate(parseDate(res->getFromDate()));
    ui->pickupLocation->setText(QString::fromStdString(res->getPickupLocation()));
    ui->returnDate->setDate(parseDate(res->getToDate()));
    ui->returnLocation->setText(QString::fromStdString(res->getReturnLocation()));
    ui->price->setText(QString::number(res->getPrice()));
}

RentalCarReservationUi::~RentalCarReservationUi() {
    delete ui;
}
