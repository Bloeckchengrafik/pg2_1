//
// Created by chris on 5/14/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TrainTicketUi.h" resolved

#include "trainticketui.h"
#include "ui_trainticketui.h"
#include "../backend/booking/Booking.h"
#include <QStringListModel>
#include <QInputDialog>

TrainTicketUi::TrainTicketUi(const std::shared_ptr<TrainTicket> &booking,
                             const std::shared_ptr<BookingController> &changeController,
                             QWidget *parent) : QWidget(parent), ui(new Ui::TrainTicketUi), booking(booking),
                                                changeController(changeController) {
    ui->setupUi(this);
    ui->fromStation->setText(QString::fromStdString(booking->getFromStation()));
    ui->toStation->setText(QString::fromStdString(booking->getToStation()));
    ui->fromDate->setDate(parseDate(booking->getFromDate()));
    ui->fromDate->setTime(QTime::fromString(QString::fromStdString(booking->getDepartureTime())));
    ui->arrivalTime->setTime(QTime::fromString(QString::fromStdString(booking->getArrivalTime())));

    QStringList connectingStationList;
    for (const auto &[station, _]: booking->getConnectingStations()) {
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

    ui->price->setValue(booking->getPrice());

    connect(ui->fromStation, &QLineEdit::textChanged, this, &TrainTicketUi::onChangeFromStation);
    connect(ui->toStation, &QLineEdit::textChanged, this, &TrainTicketUi::onChangeToStation);
    connect(ui->fromDate, &QDateTimeEdit::dateTimeChanged, this, &TrainTicketUi::onChangeFromDate);
    connect(ui->arrivalTime, &QDateTimeEdit::dateTimeChanged, this, &TrainTicketUi::onChangeToDate);
    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TrainTicketUi::onChangeType);
    connect(ui->price, &QDoubleSpinBox::valueChanged, this, &TrainTicketUi::onChangePrice);
    connect(ui->connectingStations, &QListView::clicked, this, &TrainTicketUi::onChosenConnectingStations);
    connect(ui->addItem, &QPushButton::clicked, this, &TrainTicketUi::onAddStation);
}

TrainTicketUi::~TrainTicketUi() {
    delete ui;
}

void TrainTicketUi::onChangeFromStation(QString station) const {
    booking->setFromStation(station.toStdString());
    changeController->onChange();
}

void TrainTicketUi::onChangeToStation(QString station) const {
    booking->setToStation(station.toStdString());
    changeController->onChange();
}

void TrainTicketUi::onChangeFromDate(QDateTime station) const {
    booking->setFromDate(station.date().toString("yyyyMMdd").toStdString());
    booking->setDepartureTime(station.time().toString("hh:mm").toStdString());
    changeController->onChange();
}

void TrainTicketUi::onChangeToDate(QDateTime station) const {
    booking->setToDate(station.date().toString("yyyyMMdd").toStdString());
    booking->setArrivalTime(station.time().toString("hh:mm").toStdString());
    changeController->onChange();
}

void TrainTicketUi::onChangeType(int index) const {
    booking->setTicketType(ui->type->itemData(index).value<TicketType>());
    changeController->onChange();
}

void TrainTicketUi::onChangePrice(double amount) const {
    booking->setPrice(amount);
    changeController->onChange();
}

void TrainTicketUi::onChosenConnectingStations(QModelIndex index) {
    bool ok;
    QString station = QInputDialog::getText(this, "Edit Station",
                                            "Station name:", QLineEdit::Normal,
                                            index.data().toString(), &ok);
    if (ok && !station.isEmpty()) {
        auto stations = booking->getConnectingStations();
        stations[index.row()] = {
            .station = station.toStdString(),
            .position = stations[index.row()].position,
        };
        booking->setConnectingStations(stations);
        updateConnectingStationsModel();
        changeController->onChange();
    }
}

void TrainTicketUi::onAddStation() {
    bool ok;
    QString station = QInputDialog::getText(this, "Add Station",
                                            "Station name:", QLineEdit::Normal,
                                            "", &ok);
    if (ok && !station.isEmpty()) {
        auto stations = booking->getConnectingStations();
        booking->setConnectingStations(stations);
        updateConnectingStationsModel();
        changeController->onChange();
    }
}

void TrainTicketUi::updateConnectingStationsModel() const {
    QStringList connectingStationList;
    for (const auto &[station, _]: booking->getConnectingStations()) {
        connectingStationList.append(QString::fromStdString(station));
    }
    auto *model = qobject_cast<QStringListModel *>(ui->connectingStations->model());
    model->setStringList(connectingStationList);
}
