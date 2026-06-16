#ifndef PAGEPETA_H
#define PAGEPETA_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QPainter>
#include <QMouseEvent>
#include "BankSampahController.h"

class GrafCanvas : public QWidget {
    Q_OBJECT
public:
    explicit GrafCanvas(GrafJaringan* graf, QWidget* parent = nullptr);
    void setJalur(const int* jalur, int panjang);
    void clearJalur();

protected:
    void paintEvent(QPaintEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent* e) override;

private:
    GrafJaringan* graf;

    // Posisi piksel tiap node (dihitung dari lat/lon)
    struct NodePos { int x; int y; };
    NodePos nodePos[MAKS_NODE];

    int  jalurHighlight[MAKS_NODE];
    int  panjangJalur;
    bool adaJalur;
    int  hoveredNode;
    QPoint mousePos;

    void hitungPosisi();
    bool isInJalur(int u, int v) const;
    int  nodeDekat(QPoint p) const;
};

class PagePeta : public QWidget {
    Q_OBJECT
public:
    explicit PagePeta(BankSampahController* ctrl, QWidget* parent = nullptr);
    ~PagePeta();
    void refresh();

private:
    BankSampahController* ctrl;
    GrafCanvas*   canvas;
    QComboBox*    cmbAsal;
    QTableWidget* tblJarak;
    QTextEdit*    txtJalur;
    QLabel*       lblHasil;
    QLabel*       lblLegend;

    int*          jalurTerakhir;
    int           panjangTerakhir;

    void populateCombo();

private slots:
    void onHitungRute();
    void onClearJalur();
};

#endif // PAGEPETA_H
