#ifndef TRAVELAGENCYUI_H
#define TRAVELAGENCYUI_H

#include <QMainWindow>
#include <QtWidgets/QTableView>

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
    void onSearchId();
    void onDblClickTravel(int row, int);
    void onDblClickBooking(int row, int);

private:
    Ui::TravelAgencyUi *ui;
    TravelAgency *agency;
    Customer *selectedCustomer;
    Travel *selectedTravel;

    void readFile(ReadFunc func, const std::string &name);

    void clearUi();
    void displayCustomer(Customer *customer);
    void displayTravel(Travel *travel);
    void displayBooking(Booking *booking);
};

#endif // TRAVELAGENCYUI_H
