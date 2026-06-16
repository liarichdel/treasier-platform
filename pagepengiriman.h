#ifndef PAGEPENGIRIMAN_H
#define PAGEPENGIRIMAN_H
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "BankSampahController.h"

class PagePengiriman : public QWidget {
    Q_OBJECT
public:
    explicit PagePengiriman(BankSampahController* ctrl, QWidget* parent = nullptr);
    void refresh();
private:
    BankSampahController* ctrl;
    QTableWidget* tblPengiriman;
    QComboBox*    cmbPos;
    QLabel*       lblStatus;
    QLabel*       lblInfo;
    QLineEdit*    edtUpdateResi;
    QComboBox*    cmbUpdateStatus;

private slots:
    void onBuatPengiriman();
    void onUpdateStatus();
};
#endif // PAGEPENGIRIMAN_H
