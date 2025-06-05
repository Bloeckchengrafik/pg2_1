//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TrainTicketUi.h" resolved

#include "trainticketui.h"
#include "ui_trainticketui.h"
#include "../backend/booking/Booking.h"
#include <QStringListModel>

TrainTicketUi::TrainTicketUi(TrainTicket *booking, QWidget *parent) :
    QWidget(parent), ui(new Ui::TrainTicketUi) {
    ui->setupUi(this);
    ui->fromStation->setText(QString::fromStdString(booking->getFromStation()));
    ui->toStation->setText(QString::fromStdString(booking->getToStation()));
    ui->fromDate->setDate(parseDate(booking->getFromDate()));
    ui->fromDate->setTime(QTime::fromString(QString::fromStdString(booking->getDepartureTime())));
    ui->arrivalTime->setTime(QTime::fromString(QString::fromStdString(booking->getArrivalTime())));

    QStringList connectingStationList;
    for (const auto& station : booking->getConnectingStations()) {
        connectingStationList.append(QString::fromStdString(station));
    }

    const auto connectingStationsModel = new QStringListModel(this);
    ui->connectingStations->setModel(connectingStationsModel);
    connectingStationsModel->setStringList(connectingStationList);

    ui->type->addItem("Supersparpreis 1. Klasse", QVariant::fromValue(SUPER_SAVING_FIRST_CLASS));
    ui->type->addItem("Supersparpreis 2. Klasse", QVariant::fromValue(SUPER_SAVING_SECOND_CLASS));
    ui->type->addItem("Sparpreis 1. Klasse", QVariant::fromValue(SAVING_FIRST_CLASS));
    ui->type->addItem("Sparpreis 2. Klasse", QVariant::fromValue(SAVING_SECOND_CLASS));
    ui->type->addItem("Flexpreis 1. Klasse", QVariant::fromValue(FLEX_FIRST_CLASS));
    ui->type->addItem("Flexpreis 2. Klasse", QVariant::fromValue(FLEX_SECOND_CLASS));
    ui->type->setCurrentIndex(booking->getTicketType());

    ui->price->setText(QString::number(booking->getPrice()));
}

TrainTicketUi::~TrainTicketUi() {
    delete ui;
}
