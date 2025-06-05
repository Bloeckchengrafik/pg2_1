//
// Created by chris on 5/14/25.
//

#ifndef TRAINTICKETUI_H
#define TRAINTICKETUI_H

#include <QDateTime>
#include <QStringListModel>

#include "../backend/booking/TrainTicket.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class TrainTicketUi;
}

QT_END_NAMESPACE

class TrainTicketUi : public QWidget {
    Q_OBJECT

public:
    explicit TrainTicketUi(TrainTicket *booking, ChangeController *changeController, QWidget *parent = nullptr);

    ~TrainTicketUi() override;

public slots:
    void onChangeFromStation(QString station);

    void onChangeToStation(QString station);

    void onChangeFromDate(QDateTime station);

    void onChangeToDate(QDateTime station);

    void onChangeType(int index);

    void onChangePrice(double amount);

    void onChosenConnectingStations(QModelIndex index);

    void onAddStation();

private:
    Ui::TrainTicketUi *ui;
    TrainTicket *booking;
    ChangeController *changeController;

    void updateConnectingStationsModel();
};


#endif //TRAINTICKETUI_H
