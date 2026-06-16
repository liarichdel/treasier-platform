#include "mainwindow.h"
#include <QApplication>

static const char* SIDEBAR_STYLE = R"(
QFrame#sidebar {
    background: #f8f9fa;
    border-right: 1px solid #f8f9fa;
    min-width: 210px;
    max-width: 210px;
}

QPushButton#navBtn {
    color: #495057;
    background: transparent;
    border: none;
    border-radius: 8px;
    padding: 10px 16px;
    text-align: left;
    font-size: 14px;
    font-weight: 500;
    margin: 2px 10px;
}

QPushButton#navBtn:hover {
    background: #e9ecef;
    color: #212529;

}

QPushButton#navBtn[active="true"] {
    background: #28a745;
    color: #ffffff;
}
)";



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), activeBtn(nullptr) {

    ctrl.inisialisasi();
    applyGlobalStyle();

    setWindowTitle("treasIER");
    resize(1280, 780);

    central = new QWidget(this);
    setCentralWidget(central);
    mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    setupSidebar();
    setupPages();


    // Default halaman dashboard
    onDashboard();
}

MainWindow::~MainWindow() {}

void MainWindow::applyGlobalStyle() {
    qApp->setStyleSheet(R"(
        QMainWindow, QWidget { background: #ffffff; color: #212529; font-family: 'Segoe UI', Arial; }
        QLabel { color: #212529;}
        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
            background: #ffffff; border: 1px solid #ced4da; border-radius: 6px;
            padding: 6px 10px; color: #212529; font-size: 13px;
        }
        QLineEdit:focus, QComboBox:focus { border-color: #28a745; }
        QComboBox::drop-down { border: none; }
        QComboBox QAbstractItemView { background: #ffffff; color: #212529; border: 1px solid #ced4da; }
            background: #28a745; color: #fff; border: none; border-radius: 7px;
            padding: 8px 18px; font-size: 13px; font-weight: 600;
        }
        QPushButton:hover { background: #218838; }
        QPushButton:pressed { background: #1e7e34; }
        QPushButton#btnDanger { background: #dc3545; }
        QPushButton#btnDanger:hover { background: #bd2130; }
        QPushButton#btnSecondary { background: #6c757d; }
        QPushButton#btnSecondary:hover { background: #5a6268; }
        QTableWidget {
            background: #ffffff; border: 1px solid #dee2e6; border-radius: 8px;
            gridline-color: #e9ecef; color: #212529;
        }
        QTableWidget::item { padding: 6px 10px; }
        QTableWidget::item:selected { background: #28a745; color: #fff; }
        QHeaderView::section {
            background: #f8f9fa; color: #495057; border: none;
            padding: 8px 10px; font-weight: 600; font-size: 12px;
            border-bottom: 2px solid #dee2e6;
        }
        QScrollBar:vertical { background: #f1f3f5; width: 8px; border-radius: 4px; }
        QScrollBar::handle:vertical { background: #ced4da; border-radius: 4px; min-height: 30px; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        QGroupBox { border: 1px solid #dee2e6; border-radius: 8px; margin-top: 12px;
                    padding-top: 12px; color: #495057; font-weight: 600; }

        QGroupBox::title { subcontrol-origin: margin; left: 12px; }
        QListWidget { background: #ffffff; border: 1px solid #dee2e6; border-radius: 8px; color: #212529; }
        QListWidget::item:selected { background: #212529; }
        QTreeWidget { background: #ffffff; border: 1px solid #dee2e6; border-radius: 8px; color: #212529; }
        QTreeWidget::item:selected { background: #e9ecef; color: #212529; }
        QSplitter::handle { background: #dee2e6; }
    )");
}

void MainWindow::setupSidebar() {
    sidebar = new QFrame(central);
    sidebar->setObjectName("sidebar");
    sidebar->setStyleSheet(SIDEBAR_STYLE);

    QVBoxLayout* sbl = new QVBoxLayout(sidebar);
    sbl->setContentsMargins(0, 20, 0, 20);
    sbl->setSpacing(4);

    // Logo area
    QLabel* logo = new QLabel("treasIER", sidebar);
    logo->setStyleSheet("color:#28a745; font-size:16px; font-weight:700; padding:0 20px 16px;");
    QLabel* subLogo = new QLabel("Sistem Manajemen Sampah Cerdas", sidebar);
    subLogo->setStyleSheet("color:#6c757d; font-size:10px; padding:0 20px 16px;");
    sbl->addWidget(logo);
    sbl->addWidget(subLogo);

    QFrame* sep = new QFrame(sidebar);
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color:#e9ecef;");
    sbl->addWidget(sep);
    sbl->addSpacing(8);

    auto makeBtn = [&](const QString& icon, const QString& label) {
        QPushButton* btn = new QPushButton(icon + "  " + label, sidebar);
        btn->setObjectName("navBtn");
        btn->setCursor(Qt::PointingHandCursor);
        return btn;
    };

    btnDashboard  = makeBtn("", "Dashboard");
    btnAntrean    = makeBtn("", "Antrean");
    btnNasabah    = makeBtn("", "Nasabah");
    btnGudang     = makeBtn("",  "Gudang");
    btnWilayah    = makeBtn("",  "Wilayah");
    btnPengiriman = makeBtn("", "Pengiriman");
    btnPeta       = makeBtn("",  "Peta Rute");

    sbl->addWidget(btnDashboard);
    sbl->addWidget(btnAntrean);
    sbl->addWidget(btnNasabah);
    sbl->addWidget(btnGudang);
    sbl->addWidget(btnWilayah);
    sbl->addWidget(btnPengiriman);
    sbl->addWidget(btnPeta);
    sbl->addStretch();

    QLabel* ver = new QLabel("v1.0.0 treasIER", sidebar);
    ver->setStyleSheet("color:#adb5bd; font-size:10px; padding:0 20px;");
    sbl->addWidget(ver);

    mainLayout->addWidget(sidebar);

    connect(btnDashboard,  &QPushButton::clicked, this, &MainWindow::onDashboard);
    connect(btnAntrean,    &QPushButton::clicked, this, &MainWindow::onAntrean);
    connect(btnNasabah,    &QPushButton::clicked, this, &MainWindow::onNasabah);
    connect(btnGudang,     &QPushButton::clicked, this, &MainWindow::onGudang);
    connect(btnWilayah,    &QPushButton::clicked, this, &MainWindow::onWilayah);
    connect(btnPengiriman, &QPushButton::clicked, this, &MainWindow::onPengiriman);
    connect(btnPeta,       &QPushButton::clicked, this, &MainWindow::onPeta);
}

void MainWindow::setupPages() {
    pages = new QStackedWidget(central);

    pgDashboard  = new PageDashboard(&ctrl, pages);
    pgAntrean    = new PageAntrean(&ctrl, pages);
    pgNasabah    = new PageNasabah(&ctrl, pages);
    pgGudang     = new PageGudang(&ctrl, pages);
    pgWilayah    = new PageWilayah(&ctrl, pages);
    pgPengiriman = new PagePengiriman(&ctrl, pages);
    pgPeta       = new PagePeta(&ctrl, pages);

    pages->addWidget(pgDashboard);   // 0
    pages->addWidget(pgAntrean);     // 1
    pages->addWidget(pgNasabah);     // 2
    pages->addWidget(pgGudang);      // 3
    pages->addWidget(pgWilayah);     // 4
    pages->addWidget(pgPengiriman);  // 5
    pages->addWidget(pgPeta);        // 6

    mainLayout->addWidget(pages);
}



void MainWindow::setActivePage(int idx, QPushButton* btn) {
    pages->setCurrentIndex(idx);
    if (activeBtn) {
        activeBtn->setProperty("active", false);
        activeBtn->style()->unpolish(activeBtn);
        activeBtn->style()->polish(activeBtn);
    }

    btn->setProperty("active", true);
    btn->style()->unpolish(btn);
    btn->style()->polish(btn);

    activeBtn = btn;

    // Refresh halaman
    QWidget* w = pages->currentWidget();
    if (auto* p = qobject_cast<PageDashboard*>(w))  p->refresh();
    if (auto* p = qobject_cast<PageAntrean*>(w))    p->refresh();
    if (auto* p = qobject_cast<PageNasabah*>(w))    p->refresh();
    if (auto* p = qobject_cast<PageGudang*>(w))     p->refresh();
    if (auto* p = qobject_cast<PageWilayah*>(w))    p->refresh();
    if (auto* p = qobject_cast<PagePengiriman*>(w)) p->refresh();
    if (auto* p = qobject_cast<PagePeta*>(w))       p->refresh();
}

void MainWindow::onDashboard()  { setActivePage(0, btnDashboard); }
void MainWindow::onAntrean()    { setActivePage(1, btnAntrean); }
void MainWindow::onNasabah()    { setActivePage(2, btnNasabah); }
void MainWindow::onGudang()     { setActivePage(3, btnGudang); }
void MainWindow::onWilayah()    { setActivePage(4, btnWilayah); }
void MainWindow::onPengiriman() { setActivePage(5, btnPengiriman); }
void MainWindow::onPeta()       { setActivePage(6, btnPeta); }
