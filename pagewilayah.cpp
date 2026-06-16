#include "pagewilayah.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QHeaderView>
#include <QDoubleSpinBox>
#include <QMessageBox>

PageWilayah::PageWilayah(BankSampahController* c, QWidget* parent)
    : QWidget(parent), ctrl(c) {

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(28,24,28,24);
    root->setSpacing(14);

    QLabel* title = new QLabel("Analisis Wilayah", this);
    title->setStyleSheet("font-size:20px;font-weight:700;color:#212529;");
    QLabel* sub = new QLabel("Implementasi Merge Sort, Binary Search, Linear Search", this);
    sub->setStyleSheet("font-size:11px;color:#0e6251;font-weight:600;");
    root->addWidget(title);
    root->addWidget(sub);

    lblAlgo = new QLabel("", this);
    lblAlgo->setStyleSheet("background:#e8f8f5;border:1px solid #a3e4d7;border-radius:6px;padding:8px 14px;color:#117a65;font-size:11px;");
    root->addWidget(lblAlgo);

    // Aksi row
    QHBoxLayout* actRow = new QHBoxLayout();
    QPushButton* btnSort = new QPushButton("Urutkan Rank", this);
    edtCariWilayah = new QLineEdit(this);
    edtCariWilayah->setPlaceholderText("ID Pos (mis. POS-GBG)...");
    edtCariWilayah->setFixedWidth(200);
    QPushButton* btnCari = new QPushButton("Cari", this);
    btnCari->setObjectName("btnSecondary");
    lblCariResult = new QLabel("", this);
    lblCariResult->setStyleSheet("color:#1e7e34;font-size:12px;");
    actRow->addWidget(btnSort);
    actRow->addSpacing(20);
    actRow->addWidget(edtCariWilayah);
    actRow->addWidget(btnCari);
    actRow->addWidget(lblCariResult, 1);
    actRow->addStretch();
    root->addLayout(actRow);

    // Tabel
    tblWilayah = new QTableWidget(this);
    tblWilayah->setColumnCount(6);
    tblWilayah->setHorizontalHeaderLabels({"Rank","ID Pos","Kecamatan","Volume (ton)","Transaksi","Status"});
    tblWilayah->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblWilayah->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblWilayah->verticalHeader()->setVisible(false);
    root->addWidget(tblWilayah, 1);

    // Form tambah wilayah
    QGroupBox* gbAdd = new QGroupBox("Tambah Pos Wilayah Baru", this);
    QHBoxLayout* addRow = new QHBoxLayout(gbAdd);
    QLabel* l1 = new QLabel("ID Pos:", gbAdd);
    edtIdPos = new QLineEdit(gbAdd); edtIdPos->setPlaceholderText("POS-XXX");
    QLabel* l2 = new QLabel("Nama Kec:", gbAdd);
    edtNamaKec = new QLineEdit(gbAdd); edtNamaKec->setPlaceholderText("Kec. ...");
    QLabel* l3 = new QLabel("Volume (ton):", gbAdd);
    spnVol = new QDoubleSpinBox(gbAdd);
    spnVol->setRange(0, 9999); spnVol->setSuffix(" ton");
    QPushButton* btnAdd = new QPushButton("Tambah", gbAdd);
    addRow->addWidget(l1); addRow->addWidget(edtIdPos);
    addRow->addWidget(l2); addRow->addWidget(edtNamaKec);
    addRow->addWidget(l3); addRow->addWidget(spnVol);
    addRow->addWidget(btnAdd);
    root->addWidget(gbAdd);

    lblStatus = new QLabel("", this);
    lblStatus->setStyleSheet("font-size:11px;color:#4a5568;");
    root->addWidget(lblStatus);

    connect(btnSort, &QPushButton::clicked,  this, &PageWilayah::onSortir);
    connect(btnCari, &QPushButton::clicked,  this, &PageWilayah::onCariWilayah);
    connect(btnAdd,  &QPushButton::clicked,  this, &PageWilayah::onTambahWilayah);

    refresh();
}

void PageWilayah::refresh() {
    ArrayWilayah& w = ctrl->getWilayah();
    tblWilayah->setRowCount(0);
    for (int i = 0; i < w.getUkuran(); i++) {
        const DataWilayah* d = w.get(i);
        if (!d) continue;
        int row = i;
        tblWilayah->insertRow(row);
        tblWilayah->setItem(row,0,new QTableWidgetItem(QString("#%1").arg(i+1)));
        tblWilayah->setItem(row,1,new QTableWidgetItem(d->idPos));
        tblWilayah->setItem(row,2,new QTableWidgetItem(d->namaKecamatan));
        tblWilayah->setItem(row,3,new QTableWidgetItem(QString::number(d->totalVolumeTon,'f',1)));
        tblWilayah->setItem(row,4,new QTableWidgetItem(QString::number(d->jumlahTransaksi)));
        QTableWidgetItem* statusItem = new QTableWidgetItem(d->statusKritis ? "KRITIS" : "Normal");
        statusItem->setForeground(d->statusKritis ? QColor("#212529") : QColor("#28a745"));
        tblWilayah->setItem(row,5,statusItem);
        if (d->statusKritis) {

            for (int c = 0; c < 6; c++) {
                if (tblWilayah->item(row,c))
                tblWilayah->item(row,c)->setBackground(QColor("#dc3545"));

            }
        }
    }

    int kritis = 0;
    for (int i = 0; i < w.getUkuran(); i++) {
        const DataWilayah* d = w.get(i);
        if (d && d->statusKritis) kritis++;
    }
    lblAlgo->setText(QString(" %1 wilayah terdaftar · %2 wilayah kritis (≥10 ton) · Binary Search tersedia setelah diurutkan by ID")
                         .arg(w.getUkuran()).arg(kritis));
}

void PageWilayah::onSortir() {
    ctrl->sortirWilayah();
    refresh();
    lblStatus->setText("Pengurutan selesai — wilayah diurutkan descending berdasarkan volume sampah.");
}

void PageWilayah::onCariWilayah() {
    QString idPos = edtCariWilayah->text().trimmed();
    if (idPos.isEmpty()) return;
    DataWilayah* d = ctrl->cariWilayah(idPos);
    if (d) {
        lblCariResult->setStyleSheet("color:#28a745;font-size:12px;");
        lblCariResult->setText(QString("Ditemukan: %1 | %.1f ton | %2")
                                   .arg(d->namaKecamatan).arg(d->totalVolumeTon).arg(d->statusKritis ? "KRITIS" : "Normal"));
    } else {
        lblCariResult->setStyleSheet("color:#dc3545; font-size:12px;");
        lblCariResult->setText("ID Pos tidak ditemukan (Linear Search selesai).");
    }
}

void PageWilayah::onTambahWilayah() {
    DataWilayah w;
    w.idPos = edtIdPos->text().trimmed();
    w.namaKecamatan = edtNamaKec->text().trimmed();
    w.totalVolumeTon = spnVol->value();
    if (w.idPos.isEmpty() || w.namaKecamatan.isEmpty()) {
        QMessageBox::warning(this, "Input", "ID Pos dan Nama Kecamatan wajib diisi.");
        return;
    }
    ctrl->tambahWilayah(w);
    edtIdPos->clear(); edtNamaKec->clear(); spnVol->setValue(0);
    refresh();
}
