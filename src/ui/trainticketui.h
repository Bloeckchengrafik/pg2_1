//
// Created by chris on 5/14/25.
//

#ifndef TRAINTICKETUI_H
#define TRAINTICKETUI_H

#include "../backend/booking/TrainTicket.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TrainTicketUi; }
QT_END_NAMESPACE

class TrainTicketUi : public QWidget {
Q_OBJECT

public:
    explicit TrainTicketUi(TrainTicket *booking, QWidget *parent = nullptr);
    ~TrainTicketUi() override;

private:
    Ui::TrainTicketUi *ui;
};


#endif //TRAINTICKETUI_H
