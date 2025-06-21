//
// Created by chris on 5/14/25.
//

#ifndef FLIGHTBOOKINGUI_H
#define FLIGHTBOOKINGUI_H

#include <QDate>
#include <QLabel>

#include "../backend/booking/FlightBooking.h"
#include "../backend/coord/Airport.h"


QT_BEGIN_NAMESPACE
namespace Ui { class FlightBookingUi; }
QT_END_NAMESPACE

class FlightBookingUi : public QWidget {
Q_OBJECT

public:
    explicit FlightBookingUi(const std::shared_ptr<FlightBooking> &booking, const std::shared_ptr<BookingController> &bookingController, QWidget *parent = nullptr);
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
    std::shared_ptr<FlightBooking> booking;
    std::shared_ptr<BookingController> bookingController;

    bool updateStationFrom(std::string& code);
    bool updateStationTo(std::string& code);

    void setErrorState(QLabel *label);
    void setOkState(QLabel *label, std::shared_ptr<Airport>);
};


#endif //FLIGHTBOOKINGUI_H
