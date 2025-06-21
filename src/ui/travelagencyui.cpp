#include "travelagencyui.h"

#include <iostream>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <utility>

#include "../backend/TravelAgency.h"
#include "ui_travelagencyui.h"

TravelAgencyUi::TravelAgencyUi(std::shared_ptr<TravelAgency> agency, QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::TravelAgencyUi), agency(std::move(agency)) {
    ui->setupUi(this);

    connect(ui->actionReadFile, &QAction::triggered,
            this, &TravelAgencyUi::onReadInFile);

    connect(ui->actionSearch, &QAction::triggered,
            this, &TravelAgencyUi::onSearchId);

    connect(ui->tableTravels, &QTableWidget::cellDoubleClicked,
            this, &TravelAgencyUi::onDblClickTravel);

    connect(ui->tableBookings, &QTableWidget::cellDoubleClicked,
            this, &TravelAgencyUi::onDblClickBooking);

    connect(ui->actionSave, &QAction::triggered,
            this, &TravelAgencyUi::onSave);

    clearUi();
    ui->actionSave->setEnabled(false);
}

TravelAgencyUi::~TravelAgencyUi() {
    delete ui;
}

void TravelAgencyUi::onChange() {
    allowSave = true;
    ui->actionSave->setEnabled(true);
}

std::optional<std::shared_ptr<Airport> > TravelAgencyUi::getAirport(std::string &code) {
    return agency->getAirport(code);
}

void TravelAgencyUi::onSave() {
    if (!allowSave) {
        return;
    }

    QFileDialog dialog(this);
    dialog.setWindowTitle("Save File");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setMimeTypeFilters(QStringList{
        "application/json",
    });
    dialog.setNameFilters(QStringList{
        "JSON files (*.json)",
    });

    if (dialog.exec() == QDialog::Accepted) {
        const QString selectedFile = dialog.selectedFiles().first();
        const QFileInfo fileInfo(selectedFile);

        try {
            if (fileInfo.suffix() == "json") {
                agency->writeFile(selectedFile.toStdString());
                allowSave = false;
                ui->actionSave->setEnabled(false);
            } else {
                QMessageBox::critical(this, "Error", "Unknown file type");
            }
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

void TravelAgencyUi::onReadInFile() {
    std::cout << "reading file..." << std::endl;
    QFileDialog dialog(this);
    dialog.setWindowTitle("Read File");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setMimeTypeFilters(QStringList{
        "application/json",
    });
    dialog.setNameFilters(QStringList{
        "JSON files (*.json)",
    });

    if (dialog.exec() == QDialog::Accepted) {
        const QString selectedFile = dialog.selectedFiles().first();
        const QFileInfo fileInfo(selectedFile);

        try {
            if (fileInfo.suffix() == "json") {
                readFile(&TravelAgency::readFile, selectedFile.toStdString());
            } else {
                QMessageBox::critical(this, "Error", "Unknown file type");
            }
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

void TravelAgencyUi::onSearchId() {
    bool ok;
    const long id = QInputDialog::getInt(
        this,
        "Search by ID",
        "Enter ID:",
        0, 0, 999999, 1,
        &ok
    );

    if (!ok) {
        return;
    }

    if (const auto customer = agency->findCustomer(id); customer.has_value()) {
        displayCustomer(customer.value());
        return;
    }

    QMessageBox::warning(this, "Not Found", "No customer with this ID found.");
}

void TravelAgencyUi::onDblClickTravel(const int row, const int) {
    if (!this->selectedCustomer.has_value()) {
        return;
    }

    const std::shared_ptr<Travel> travel = this->selectedCustomer.value()->getTravels()[row];
    displayTravel(travel);
}

void TravelAgencyUi::onDblClickBooking(const int row, const int) {
    if (!this->selectedTravel.has_value()) {
        return;
    }

    const std::shared_ptr<Booking> booking = this->selectedTravel.value()->getBookings()[row];
    displayBooking(booking);
}

void TravelAgencyUi::clearUi() {
    ui->customerBox->hide();
    ui->groupTravel->hide();
    this->selectedCustomer = nullptr;
    this->selectedTravel = nullptr;
}

void TravelAgencyUi::displayCustomer(const std::shared_ptr<Customer> &customer) {
    clearUi();
    ui->customerBox->show();
    ui->editId->setText(QString::number(customer->getId()));
    ui->editFirstName->setText(QString::fromStdString(customer->getFirstName()));
    ui->editLastName->setText(QString::fromStdString(customer->getLastName()));

    QTableWidget *table = ui->tableTravels;
    table->clear();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"ID", "Travel Start", "Travel End"});
    table->setRowCount(static_cast<int>(customer->getTravels().size()));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int row = 0;
    for (const auto &travel: customer->getTravels()) {
        table->setItem(row, 0, new QTableWidgetItem(QString::number(travel->getId())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(travel->getStart())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(travel->getEnd())));
        row++;
    }
    table->resizeColumnsToContents();

    this->selectedCustomer = customer;
}

void TravelAgencyUi::displayTravel(const std::shared_ptr<Travel> &travel) {
    ui->groupTravel->show();
    ui->groupTravel->setTitle("Travel " + QString::number(travel->getId()));
    QTableWidget *table = ui->tableBookings;
    table->clear();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"", "Start", "End", "Price"});
    table->setRowCount(static_cast<int>(travel->getBookings().size()));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int row = 0;
    for (const auto &booking: travel->getBookings()) {
        QIcon icon = booking->getIcon();
        const auto item = new QTableWidgetItem();
        item->setIcon(icon);
        table->setItem(row, 0, item);
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(formatDate(booking->getFromDate()))));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(formatDate(booking->getToDate()))));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(booking->getPrice())));
        row++;
    }

    table->resizeColumnsToContents();

    this->selectedTravel = travel;
}

void TravelAgencyUi::displayBooking(const std::shared_ptr<Booking> &booking) {
    booking->showEditor(shared_from_this());
}

void TravelAgencyUi::readFile(const ReadFunc func, const std::string &name) {
    while (true) {
        try {
            const auto meta = (*agency.*func)(name);
            QMessageBox::information(this, "Success", QString::fromStdString(meta));
            clearUi();
            return;
        } catch (const std::exception &e) {
            QMessageBox box;
            box.addButton("Retry", QMessageBox::YesRole);
            box.addButton(QMessageBox::Cancel);
            box.setDefaultButton(QMessageBox::Cancel);
            box.setText(QString::fromStdString(e.what()));
            if (box.exec() == QMessageBox::Cancel) {
                return;
            }
        }
    }
}
