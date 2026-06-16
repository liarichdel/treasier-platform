#ifndef PAGEANTREAN_H
#define PAGEANTREAN_H
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include "BankSampahController.h"

class PageAntrean : public QWidget {
    Q_OBJECT
public:
    explicit PageAntrean(BankSampahController* ctrl, QWidget* parent = nullptr);
    void refresh();
private:
    BankSampahController* ctrl;
    QLineEdit*    edtKTP;
    QComboBox*    cmbJenis;
    QDoubleSpinBox* spnBerat;
    QTableWidget* tblAntrean;
    QLabel*       lblStatus;
    QLabel*       lblProses;
    QLabel*       lblUkuran;

private slots:
    void onTambahAntrean();
    void onPanggilBerikutnya();
    void onProsesSetoran();
};
#endif // PAGEANTREAN_H
