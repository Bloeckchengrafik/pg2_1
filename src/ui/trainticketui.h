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
    explicit TrainTicketUi(const std::shared_ptr<TrainTicket> &booking, const std::shared_ptr<BookingController> &changeController, QWidget *parent = nullptr);

    ~TrainTicketUi() override;

public slots:
    void onChangeFromStation(QString station) const;

    void onChangeToStation(QString station) const;

    void onChangeFromDate(QDateTime station) const;

    void onChangeToDate(QDateTime station) const;

    void onChangeType(int index) const;

    void onChangePrice(double amount) const;

    void onChosenConnectingStations(QModelIndex index);

    void onAddStation();

private:
    Ui::TrainTicketUi *ui;
    std::shared_ptr<TrainTicket> booking;
    std::shared_ptr<BookingController> changeController;

    void updateConnectingStationsModel() const;
};


#endif //TRAINTICKETUI_H
