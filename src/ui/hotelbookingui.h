//
// Created by chris on 5/14/25.
//

#ifndef HOTELBOOKINGUI_H
#define HOTELBOOKINGUI_H

#include <QWidget>
#include <QDate>

#include "../backend/booking/HotelBooking.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HotelBookingUi; }
QT_END_NAMESPACE

class HotelBookingUi : public QWidget {
Q_OBJECT

public:
    explicit HotelBookingUi(HotelBooking* booking, ChangeController *changeController, QWidget *parent = nullptr);
    ~HotelBookingUi() override;

public slots:
    void onSetHotel(QString value);
    void onSetTown(QString value);
    void onSetFromDate(QDate date);
    void onSetToDate(QDate date);
    void onSetRoomType(int index);
    void onSetRoomPrice(double amount);

private:
    Ui::HotelBookingUi *ui;
    HotelBooking *booking;
    ChangeController *changeController;
};


#endif //HOTELBOOKINGUI_H
