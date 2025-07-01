//
// Created by chris on 7/1/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CheckResultUi.h" resolved

#include "checkresultui.h"
#include "ui_checkresultui.h"
#include <QStandardItemModel>


CheckResultUi::CheckResultUi(const std::vector<CheckError> &errors, const std::shared_ptr<TravelAgency> &agency, QWidget *parent) : QWidget(parent),
    ui(new Ui::CheckResultUi) {
    ui->setupUi(this);

    const auto model = new QStandardItemModel(this);
    std::unordered_map<int, QStandardItem*> results;

    for (const auto &error: errors) {
        QStandardItem *rootItem;
        if (int id = error.getCustomer(); !results.contains(id)) {
            const auto customer = agency->findCustomer(id).value();
            std::string name = "Customer " + customer->getLastName() + " " + customer->getFirstName() + " (" + std::to_string(id) + ")";
            rootItem = new QStandardItem(QString::fromStdString(name));
            results[id] = rootItem;
            model->appendRow(rootItem);
        } else {
            rootItem = results[id];
        }

        const auto item = new QStandardItem(QString::fromStdString(error.getMessage()));
        rootItem->appendRow(item);
    }

    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    connect(ui->ok, &QPushButton::clicked,
            this, &CheckResultUi::close);
}

CheckResultUi::~CheckResultUi() {
    delete ui;
}
