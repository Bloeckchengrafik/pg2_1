//
// Created by chris on 5/14/25.
//

#ifndef RENTALCARRESERVATIONUI_H
#define RENTALCARRESERVATIONUI_H

#include "../backend/booking/RentalCarReservation.h"


QT_BEGIN_NAMESPACE
namespace Ui { class RentalCarReservationUi; }
QT_END_NAMESPACE

class RentalCarReservationUi : public QWidget {
Q_OBJECT

public:
    explicit RentalCarReservationUi(RentalCarReservation *res, QWidget *parent = nullptr);
    ~RentalCarReservationUi() override;

private:
    Ui::RentalCarReservationUi *ui;
};


#endif //RENTALCARRESERVATIONUI_H
