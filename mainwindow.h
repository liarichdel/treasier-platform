#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include "BankSampahController.h"
#include "PageDashboard.h"
#include "PageAntrean.h"
#include "PageNasabah.h"
#include "PageGudang.h"
#include "PageWilayah.h"
#include "PagePengiriman.h"
#include "PagePeta.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    BankSampahController ctrl;
    QWidget*       central;
    QHBoxLayout*   mainLayout;
    QFrame*        sidebar;
    QStackedWidget* pages;

    // pagenya
    PageDashboard*   pgDashboard;
    PageAntrean*     pgAntrean;
    PageNasabah*     pgNasabah;
    PageGudang*      pgGudang;
    PageWilayah*     pgWilayah;
    PagePengiriman*  pgPengiriman;
    PagePeta*        pgPeta;

    // sidebar buttons
    QPushButton* btnDashboard;
    QPushButton* btnAntrean;
    QPushButton* btnNasabah;
    QPushButton* btnGudang;
    QPushButton* btnWilayah;
    QPushButton* btnPengiriman;
    QPushButton* btnPeta;

    QPushButton* activeBtn;

    void setupSidebar();
    void setupPages();
    void applyGlobalStyle();
    void setActivePage(int idx, QPushButton* btn);

private slots:
    void onDashboard();
    void onAntrean();
    void onNasabah();
    void onGudang();
    void onWilayah();
    void onPengiriman();
    void onPeta();
};

#endif // MAINWINDOW_H
