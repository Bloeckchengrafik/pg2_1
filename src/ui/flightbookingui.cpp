//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FlightBookingUi.h" resolved

#include "flightbookingui.h"
#include "ui_flightbookingui.h"


FlightBookingUi::FlightBookingUi(FlightBooking *booking, QWidget *parent) :
    QWidget(parent), ui(new Ui::FlightBookingUi) {
    ui->setupUi(this);
    ui->fromDestination->setText(QString::fromStdString(booking->getFromDestination()));
    ui->toDestination->setText(QString::fromStdString(booking->getToDestination()));
    ui->airline->setText(QString::fromStdString(booking->getAirline()));
    ui->fromDate->setDate(parseDate(booking->getFromDate()));
    ui->price->setText(QString::number(booking->getPrice()));
}

FlightBookingUi::~FlightBookingUi() {
    delete ui;
}
