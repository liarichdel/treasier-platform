#ifndef PAGEGUDANG_H
#define PAGEGUDANG_H
#include <QWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QLabel>
#include <QGroupBox>
#include "BankSampahController.h"

class PageGudang : public QWidget {
    Q_OBJECT
public:
    explicit PageGudang(BankSampahController* ctrl, QWidget* parent = nullptr);
    void refresh();
private:
    BankSampahController* ctrl;
    QListWidget* lstStack;
    QLabel* lblTop;
    QLabel* lblUkuran;
    QLabel* lblKapasitas;
    QLabel* lblTreeInfo;
    QTreeWidget* treeHarga;
private slots:
    void onPopBalok();
};
#endif // PAGEGUDANG_H
