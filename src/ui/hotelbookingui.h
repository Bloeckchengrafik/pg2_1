//
// Created by chris on 5/14/25.
//

#ifndef HOTELBOOKINGUI_H
#define HOTELBOOKINGUI_H

#include <QWidget>

#include "../backend/booking/HotelBooking.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HotelBookingUi; }
QT_END_NAMESPACE

class HotelBookingUi : public QWidget {
Q_OBJECT

public:
    explicit HotelBookingUi(HotelBooking* booking, QWidget *parent = nullptr);
    ~HotelBookingUi() override;

private:
    Ui::HotelBookingUi *ui;
};


#endif //HOTELBOOKINGUI_H
