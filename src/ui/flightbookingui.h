//
// Created by chris on 5/14/25.
//

#ifndef FLIGHTBOOKINGUI_H
#define FLIGHTBOOKINGUI_H

#include <QWidget>
#include <QDate>

#include "../backend/booking/FlightBooking.h"


QT_BEGIN_NAMESPACE
namespace Ui { class FlightBookingUi; }
QT_END_NAMESPACE

class FlightBookingUi : public QWidget {
Q_OBJECT

public:
    explicit FlightBookingUi(FlightBooking *booking, ChangeController *changeController, QWidget *parent = nullptr);
    ~FlightBookingUi() override;

public slots:
    void onSetFromStation(QString station);
    void onSetToStation(QString station);
    void onSetAirline(QString airline);
    void onSetDate(QDate date);
    void onSetPrice(double amount);
    void onSetBookingClass(int index);

private:
    Ui::FlightBookingUi *ui;
    FlightBooking *booking;
    ChangeController *changeController;
};


#endif //FLIGHTBOOKINGUI_H
