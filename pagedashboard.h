#ifndef PAGEDASHBOARD_H
#define PAGEDASHBOARD_H
#include <QWidget>
#include <QLabel>
#include "BankSampahController.h"

class PageDashboard : public QWidget {
    Q_OBJECT
public:
    explicit PageDashboard(BankSampahController* ctrl, QWidget* parent = nullptr);
    void refresh();
private:
    BankSampahController* ctrl;
    QLabel* lblNasabah;
    QLabel* lblAntrean;
    QLabel* lblGudang;
    QLabel* lblPengiriman;
    QLabel* lblKritis;
    QLabel* lblResiTerakhir;

    QWidget* makeStatCard(const QString& title, const QString& color, QLabel*& valLabel);
};
#endif // PAGEDASHBOARD_H
