//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_HotelBookingUi.h" resolved

#include "hotelbookingui.h"
#include "ui_hotelbookingui.h"
#include "../backend/booking/Booking.h"


HotelBookingUi::HotelBookingUi(HotelBooking* booking, QWidget *parent) :
    QWidget(parent), ui(new Ui::HotelBookingUi) {
    ui->setupUi(this);
    ui->hotelName->setText(QString::fromStdString(booking->getHotel()));
    ui->checkinDate->setDate(parseDate(booking->getFromDate()));
    ui->town->setText(QString::fromStdString(booking->getTown()));
    ui->checkoutDate->setDate(parseDate(booking->getToDate()));
    ui->price->setText(QString::number(booking->getPrice()));
    ui->type->addItem("Single Room", QVariant::fromValue(SINGLE_ROOM));
    ui->type->addItem("Double Room", QVariant::fromValue(DOUBLE_ROOM));
    ui->type->addItem("Suite", QVariant::fromValue(SUITE));
    ui->type->addItem("Appartment", QVariant::fromValue(APPARTMENT));
    ui->type->setCurrentIndex(booking->getRoomType());
}

HotelBookingUi::~HotelBookingUi() {
    delete ui;
}
