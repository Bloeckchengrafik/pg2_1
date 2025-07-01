//
// Created by chris on 7/1/25.
//

#ifndef CHECKRESULTUI_H
#define CHECKRESULTUI_H

#include <QWidget>

#include "../backend/Check.h"


QT_BEGIN_NAMESPACE
namespace Ui { class CheckResultUi; }
QT_END_NAMESPACE

class CheckResultUi : public QWidget {
Q_OBJECT

public:
    CheckResultUi(const std::vector<CheckError> &errors, const std::shared_ptr<TravelAgency> &agency, QWidget *parent);

    ~CheckResultUi() override;

private:
    Ui::CheckResultUi *ui;
};


#endif //CHECKRESULTUI_H
