#include "ui/travelagencyui.h"
#include "backend/TravelAgency.h"

#include <QDir>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    const auto agency = std::make_shared<TravelAgency>();
    QFile f(":qdarkstyle/dark/darkstyle.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
    }
    const auto ui = std::make_shared<TravelAgencyUi>(agency);
    ui->show();
    return QApplication::exec();
}
