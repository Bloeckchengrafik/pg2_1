//
// Created by chris on 7/1/25.
//

#ifndef CHECKSETTINGS_H
#define CHECKSETTINGS_H

#include <QWidget>

#include "../backend/CheckConfiguration.h"


QT_BEGIN_NAMESPACE
namespace Ui { class CheckSettings; }
QT_END_NAMESPACE

class CheckSettings : public QWidget {
Q_OBJECT

public:
    explicit CheckSettings(const std::shared_ptr<CheckConfigurationController> &controller, QWidget *parent = nullptr);
    ~CheckSettings() override;

public slots:
    void onOk();
    void onChanged(Qt::CheckState) const;

private:
    Ui::CheckSettings *ui;
    std::shared_ptr<CheckConfigurationController> controller;
};


#endif //CHECKSETTINGS_H
