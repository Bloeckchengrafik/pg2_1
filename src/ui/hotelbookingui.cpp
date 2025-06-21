//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_HotelBookingUi.h" resolved

#include "hotelbookingui.h"
#include "ui_hotelbookingui.h"
#include "../backend/booking/Booking.h"


HotelBookingUi::HotelBookingUi(const std::shared_ptr<HotelBooking> &booking,
                               const std::shared_ptr<BookingController> &changeController,
                               QWidget *parent) : QWidget(parent), ui(new Ui::HotelBookingUi), booking(booking),
                                                  changeController(changeController) {
    ui->setupUi(this);
    ui->hotelName->setText(QString::fromStdString(booking->getHotel()));
    ui->checkinDate->setDate(parseDate(booking->getFromDate()));
    ui->town->setText(QString::fromStdString(booking->getTown()));
    ui->checkoutDate->setDate(parseDate(booking->getToDate()));
    ui->price->setValue(booking->getPrice());
    ui->type->addItem("Single Room", QVariant::fromValue(SINGLE_ROOM));
    ui->type->addItem("Double Room", QVariant::fromValue(DOUBLE_ROOM));
    ui->type->addItem("Suite", QVariant::fromValue(SUITE));
    ui->type->addItem("Appartment", QVariant::fromValue(APPARTMENT));
    ui->type->setCurrentIndex(booking->getRoomType());

    connect(ui->hotelName, &QLineEdit::textChanged, this, &HotelBookingUi::onSetHotel);
    connect(ui->town, &QLineEdit::textChanged, this, &HotelBookingUi::onSetTown);
    connect(ui->checkinDate, &QDateEdit::dateChanged, this, &HotelBookingUi::onSetFromDate);
    connect(ui->checkoutDate, &QDateEdit::dateChanged, this, &HotelBookingUi::onSetToDate);
    connect(ui->type, &QComboBox::currentIndexChanged, this, &HotelBookingUi::onSetRoomType);
    connect(ui->price, &QDoubleSpinBox::valueChanged, this, &HotelBookingUi::onSetRoomPrice);
}

HotelBookingUi::~HotelBookingUi() {
    delete ui;
}

void HotelBookingUi::onSetHotel(QString value) {
    booking->setHotel(value.toStdString());
    changeController->onChange();
}

void HotelBookingUi::onSetTown(QString value) {
    booking->setTown(value.toStdString());
    changeController->onChange();
}

void HotelBookingUi::onSetFromDate(QDate date) {
    booking->setFromDate(date.toString("yyyyMMdd").toStdString());
    changeController->onChange();
}

void HotelBookingUi::onSetToDate(QDate date) {
    booking->setToDate(date.toString("yyyyMMdd").toStdString());
    changeController->onChange();
}

void HotelBookingUi::onSetRoomType(int index) {
    booking->setRoomType(static_cast<RoomType>(index));
    changeController->onChange();
}

void HotelBookingUi::onSetRoomPrice(double amount) {
    booking->setPrice(amount);
    changeController->onChange();
}
