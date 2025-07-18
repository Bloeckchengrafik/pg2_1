//
// Created by chris on 5/14/25.
//

#ifndef RENTALCARRESERVATIONUI_H
#define RENTALCARRESERVATIONUI_H

#include <QDate>
#include "../backend/booking/RentalCarReservation.h"


QT_BEGIN_NAMESPACE
namespace Ui { class RentalCarReservationUi; }
QT_END_NAMESPACE

class RentalCarReservationUi : public QWidget {
Q_OBJECT

public:
    explicit RentalCarReservationUi(const std::shared_ptr<RentalCarReservation> &res, const std::shared_ptr<BookingController> &changeController, QWidget *parent = nullptr);
    ~RentalCarReservationUi() override;

public slots:
    void onSetCompany(QString company);
    void onSetPickupLocation(QString location);
    void onSetReturnLocation(QString location);
    void onSetFromDate(QDate date);
    void onSetToDate(QDate date);
    void onSetPrice(double amount);

private:
    Ui::RentalCarReservationUi *ui;
    std::shared_ptr<RentalCarReservation> res;
    std::shared_ptr<BookingController> changeController;
};


#endif //RENTALCARRESERVATIONUI_H
