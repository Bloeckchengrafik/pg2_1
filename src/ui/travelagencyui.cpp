#include "travelagencyui.h"

#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

#include "../backend/TravelAgency.h"
#include "ui_travelagencyui.h"

TravelAgencyUi::TravelAgencyUi(TravelAgency *agency, QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::TravelAgencyUi), agency(agency) {
    ui->setupUi(this);
    connect(ui->actionReadFile, &QAction::triggered,
            this, &TravelAgencyUi::onReadInFile);

    connect(ui->listWidget, &QListWidget::itemDoubleClicked,
            this, &TravelAgencyUi::onClickOnDataBox);
}

TravelAgencyUi::~TravelAgencyUi() {
    delete ui;
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

void TravelAgencyUi::onClickOnDataBox(QListWidgetItem *item) {
    auto *booking = item->data(Qt::UserRole).value<Booking *>();
    if (booking == nullptr) {
        std::cout << "no data" << std::endl;
        return;
    }

    booking->showEditor();
}

void TravelAgencyUi::reloadDataBox() {
    const auto list = ui->listWidget;
    list->clear();
    for (const auto booking: agency->getBookings()) {
        auto *item = new QListWidgetItem(QString::fromStdString(booking->showDetails()));
        item->setData(Qt::UserRole, QVariant::fromValue(booking));
        list->addItem(item);
    }
}

void TravelAgencyUi::readFile(const ReadFunc func, const std::string &name) {
    while (true) {
        try {
            const auto meta = (agency->*func)(name);
            QMessageBox::information(this, "Success", QString::fromStdString(meta));
            reloadDataBox();
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
