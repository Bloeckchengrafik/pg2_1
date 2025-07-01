#ifndef TRAVELAGENCYUI_H
#define TRAVELAGENCYUI_H

#include <QMainWindow>
#include <QtWidgets/QTableView>

#include "../backend/Check.h"
#include "../backend/TravelAgency.h"
#include "../geojson/geojsonview.h"

class QListWidgetItem;

namespace Ui {
    class TravelAgencyUi;
}

typedef std::string (TravelAgency::*ReadFunc)(const std::string &name);

class TravelAgencyUi : public QMainWindow, public BookingController, public std::enable_shared_from_this<TravelAgencyUi> {
    Q_OBJECT

public:
    explicit TravelAgencyUi(std::shared_ptr<TravelAgency> agency, QWidget *parent = nullptr);
    ~TravelAgencyUi() override;

    void onChange() override;
    std::optional<std::shared_ptr<Airport>> getAirport(std::string &code) override;

signals:
    void somethingChanged();

private slots:
    void onSave();
    void onReadInFile();
    void onSearchId();
    void onDblClickTravel(int row, int);
    void onDblClickBooking(int row, int);
    void onOpenConfig() const;

    void checkAll();

private:
    Ui::TravelAgencyUi *ui;
    GeoJsonView *mapView;
    std::unique_ptr<Check> check;
    std::shared_ptr<TravelAgency> agency;
    std::optional<std::shared_ptr<Customer>> selectedCustomer;
    std::optional<std::shared_ptr<Travel>> selectedTravel;
    bool allowSave = false;

    void readFile(ReadFunc func, const std::string &name);

    void clearUi();
    void displayCustomer(const std::shared_ptr<Customer> &customer);
    void displayTravel(const std::shared_ptr<Travel> &travel);
    void displayBooking(const std::shared_ptr<Booking> &booking);
};

#endif // TRAVELAGENCYUI_H
