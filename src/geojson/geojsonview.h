#ifndef GEOJSONVIEW_H
#define GEOJSONVIEW_H

#include <QWebEngineView>

#include "GeoJsonElement.h"


class GeoJsonView : public QWebEngineView {
Q_OBJECT

public:
    explicit GeoJsonView(QWidget *parent = nullptr);
    ~GeoJsonView() override = default;

    void setElements(std::vector<std::unique_ptr<GeoJsonElement>> &elements);

private:
    std::vector<std::unique_ptr<GeoJsonElement>> elements {};

    void updateView();
};


#endif //GEOJSONVIEW_H
