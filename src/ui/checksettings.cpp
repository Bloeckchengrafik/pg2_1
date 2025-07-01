#include "checksettings.h"
#include "ui_checksettings.h"


CheckSettings::CheckSettings(const std::shared_ptr<CheckConfigurationController> &controller, QWidget *parent) :
    QWidget(parent), ui(new Ui::CheckSettings), controller(controller) {
    ui->setupUi(this);

    const auto config = controller->getConfig();
    ui->tooLittleHotels->setChecked(config->tooLittleHotels);
    ui->travelOverlaps->setChecked(config->travelOverlaps);
    ui->tooManyRentalCars->setChecked(config->tooManyRentalCars);
    ui->tooManyHotels->setChecked(config->tooManyHotels);

    connect(ui->ok, &QPushButton::pressed, this, &CheckSettings::onOk);
    connect(ui->tooLittleHotels, &QCheckBox::checkStateChanged, this, &CheckSettings::onChanged);
    connect(ui->travelOverlaps, &QCheckBox::checkStateChanged, this, &CheckSettings::onChanged);
    connect(ui->tooManyRentalCars, &QCheckBox::checkStateChanged, this, &CheckSettings::onChanged);
    connect(ui->tooManyHotels, &QCheckBox::checkStateChanged, this, &CheckSettings::onChanged);
}

CheckSettings::~CheckSettings() {
    delete ui;
}

void CheckSettings::onOk() {
    controller->saveConfig();
    close();
}

void CheckSettings::onChanged(Qt::CheckState) const {
    const auto config = controller->getConfig();
    config->tooLittleHotels = ui->tooLittleHotels->isChecked();
    config->travelOverlaps = ui->travelOverlaps->isChecked();
    config->tooManyRentalCars = ui->tooManyRentalCars->isChecked();
    config->tooManyHotels = ui->tooManyHotels->isChecked();
}
