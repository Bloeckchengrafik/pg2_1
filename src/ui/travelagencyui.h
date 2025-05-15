#ifndef TRAVELAGENCYUI_H
#define TRAVELAGENCYUI_H

#include <QMainWindow>

#include "../backend/TravelAgency.h"

class QListWidgetItem;

namespace Ui {
    class TravelAgencyUi;
}

typedef std::string (TravelAgency::*ReadFunc)(const std::string &name);

class TravelAgencyUi : public QMainWindow {
    Q_OBJECT

public:
    explicit TravelAgencyUi(TravelAgency *agency, QWidget *parent = nullptr);
    ~TravelAgencyUi() override;

private slots:
    void onReadInFile();
    void onClickOnDataBox(QListWidgetItem *item);

private:
    Ui::TravelAgencyUi *ui;
    TravelAgency *agency;

    void reloadDataBox();
    void readFile(ReadFunc func, const std::string &name);
};

#endif // TRAVELAGENCYUI_H
