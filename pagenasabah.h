#ifndef PAGENASABAH_H
#define PAGENASABAH_H
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include "BankSampahController.h"

class PageNasabah : public QWidget {
    Q_OBJECT
public:
    explicit PageNasabah(BankSampahController* ctrl, QWidget* parent = nullptr);
    void refresh();
private:
    BankSampahController* ctrl;
    QLineEdit*    edtCariNama;
    QTableWidget* tblNasabah;
    QLabel*       lblCount;
    QLineEdit* edtRegKTP;
    QLineEdit* edtRegNama;
    QLineEdit* edtRegAlamat;
    QLineEdit* edtRegTelp;
    QLabel*       lblDetailNama;
    QLabel*       lblDetailPoin;
    QTableWidget* tblRiwayat;
    QLabel*       lblHashInfo;

private slots:
    void onDaftarNasabah();
    void onCariNama();
    void onLookupKTP();
    void onNasabahSelected(int row, int col);
};
#endif // PAGENASABAH_H
