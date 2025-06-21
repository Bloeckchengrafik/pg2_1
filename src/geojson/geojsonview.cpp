//
// Created by chris on 6/21/25.
//

#include "geojsonview.h"

#include <iostream>
#include <nlohmann/json.hpp>


GeoJsonView::GeoJsonView(QWidget *parent) : QWebEngineView(parent) {
    setUrl(QUrl("https://geo.bloeckchengrafik.de"));
}

void GeoJsonView::setElements(std::vector<std::unique_ptr<GeoJsonElement> > &elements) {
    this->elements.clear();
    for (auto &element: elements) {
        this->elements.push_back(std::move(element));
    }
    updateView();
}

void GeoJsonView::updateView() {
    nlohmann::json el;
    el["type"] = "FeatureCollection";
    el["features"] = nlohmann::json::array();
    for (const auto &element: elements) {
        nlohmann::json feature;
        element->writeInto(feature);
        el["features"].push_back(feature);
    }

    const QString jsonString = QString::fromStdString(el.dump());
    const QString urlencoded = QUrl::toPercentEncoding(jsonString);
    QString nonce = QString::fromStdString(std::to_string(rand()));
    setUrl(QUrl("https://geo.bloeckchengrafik.de/?_nonce=" + nonce + "#data=data:application/json," + urlencoded));
}
