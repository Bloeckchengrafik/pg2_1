//
// Created by chris on 5/14/25.
//

#ifndef FLIGHTBOOKINGUI_H
#define FLIGHTBOOKINGUI_H

#include <QWidget>

#include "../backend/booking/FlightBooking.h"


QT_BEGIN_NAMESPACE
namespace Ui { class FlightBookingUi; }
QT_END_NAMESPACE

class FlightBookingUi : public QWidget {
Q_OBJECT

public:
    explicit FlightBookingUi(FlightBooking *booking, QWidget *parent = nullptr);
    ~FlightBookingUi() override;

private:
    Ui::FlightBookingUi *ui;
};


#endif //FLIGHTBOOKINGUI_H
