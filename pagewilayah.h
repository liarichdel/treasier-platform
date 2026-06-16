#ifndef PAGEWILAYAH_H
#define PAGEWILAYAH_H
#include <QWidget>
#include <QTableWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include "BankSampahController.h"

class PageWilayah : public QWidget {
    Q_OBJECT
public:
    explicit PageWilayah(BankSampahController* ctrl, QWidget* parent = nullptr);
    void refresh();
private:
    BankSampahController* ctrl;
    QTableWidget* tblWilayah;
    QLabel* lblAlgo;
    QLineEdit* edtCariWilayah;
    QLabel* lblCariResult;
    QLabel* lblStatus;

    QLineEdit* edtIdPos;
    QLineEdit* edtNamaKec;
    QDoubleSpinBox* spnVol;
private slots:
    void onTambahWilayah();
    void onSortir();
    void onCariWilayah();
};
#endif // PAGEWILAYAH_H
