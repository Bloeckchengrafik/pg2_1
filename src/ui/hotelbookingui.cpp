//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_HotelBookingUi.h" resolved

#include "hotelbookingui.h"
#include "ui_hotelbookingui.h"
#include "../backend/Booking.h"


HotelBookingUi::HotelBookingUi(HotelBooking* booking, QWidget *parent) :
    QWidget(parent), ui(new Ui::HotelBookingUi) {
    ui->setupUi(this);
    ui->hotelName->setText(QString::fromStdString(booking->getHotel()));
    ui->checkinDate->setDate(parseDate(booking->getFromDate()));
    ui->town->setText(QString::fromStdString(booking->getTown()));
    ui->checkoutDate->setDate(parseDate(booking->getToDate()));
    ui->price->setText(QString::number(booking->getPrice()));
}

HotelBookingUi::~HotelBookingUi() {
    delete ui;
}
