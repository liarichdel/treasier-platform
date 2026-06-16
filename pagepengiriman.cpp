#include "PagePengiriman.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>

PagePengiriman::PagePengiriman(BankSampahController* c, QWidget* parent)
    : QWidget(parent), ctrl(c) {

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(28,24,28,24);
    root->setSpacing(14);

    QLabel* title = new QLabel("Manajemen Pengiriman", this);
    title->setStyleSheet("font-size:20px;font-weight:700;color:#212529;");
    QLabel* sub = new QLabel("Implementasi Stack dan Graph", this);
    sub->setStyleSheet("font-size:11px;color:#8e44ad;font-weight:600;");
    root->addWidget(title);
    root->addWidget(sub);

    lblInfo = new QLabel("", this);
    lblInfo->setStyleSheet("background:#f3e5f5;border:1px solid #e1bee7;border-radius:6px;padding:8px 14px;color:#4a148c;font-size:11px;");
    root->addWidget(lblInfo);

    // Form buat pengiriman
    QGroupBox* gbBuat = new QGroupBox("Buat Pengiriman Baru", this);
    QHBoxLayout* fh = new QHBoxLayout(gbBuat);
    QLabel* l1 = new QLabel("Pos Asal:", gbBuat);
    cmbPos = new QComboBox(gbBuat);
    QPushButton* btnBuat = new QPushButton("Buat Pengiriman + Hitung Rute", gbBuat);
    btnBuat->setStyleSheet("background:#8e44ad;font-weight:700;padding:8px 20px;");
    fh->addWidget(l1); fh->addWidget(cmbPos, 1); fh->addWidget(btnBuat);
    root->addWidget(gbBuat);

    // Tabel daftar pengiriman
    tblPengiriman = new QTableWidget(this);
    tblPengiriman->setColumnCount(7);
    tblPengiriman->setHorizontalHeaderLabels({"Kode Resi","Pos Asal","Balok","Berat (kg)","Jarak (km)","Status","Waktu"});
    tblPengiriman->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblPengiriman->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblPengiriman->verticalHeader()->setVisible(false);
    root->addWidget(tblPengiriman, 1);

    // Update status
    QGroupBox* gbUpdate = new QGroupBox("Update Status Pengiriman", this);
    QHBoxLayout* uh = new QHBoxLayout(gbUpdate);
    QLabel* lu1 = new QLabel("Kode Resi:", gbUpdate);
    edtUpdateResi = new QLineEdit(gbUpdate);
    edtUpdateResi->setPlaceholderText("RES-YYYYMMDD-XXXX");
    QLabel* lu2 = new QLabel("Status Baru:", gbUpdate);
    cmbUpdateStatus = new QComboBox(gbUpdate);
    cmbUpdateStatus->addItem("Menunggu",         QVariant(MENUNGGU));
    cmbUpdateStatus->addItem("Dalam Perjalanan", QVariant(DALAM_PERJALANAN));
    cmbUpdateStatus->addItem("Tiba di Pabrik",   QVariant(TIBA_DI_PABRIK));
    cmbUpdateStatus->addItem("Selesai",          QVariant(SELESAI));
    QPushButton* btnUpdate = new QPushButton("Update", gbUpdate);
    btnUpdate->setObjectName("btnSecondary");
    uh->addWidget(lu1); uh->addWidget(edtUpdateResi, 1);
    uh->addWidget(lu2); uh->addWidget(cmbUpdateStatus);
    uh->addWidget(btnUpdate);
    root->addWidget(gbUpdate);

    lblStatus = new QLabel("", this);
    lblStatus->setStyleSheet("color:#28a745;font-size:12px;");
    root->addWidget(lblStatus);

    // Isi combo pos dari wilayah
    ArrayWilayah& w = ctrl->getWilayah();
    for (int i = 0; i < w.getUkuran(); i++) {
        const DataWilayah* d = w.get(i);
        if (d) cmbPos->addItem(d->namaKecamatan + " (" + d->idPos + ")", QVariant(i));
    }

    connect(btnBuat,   &QPushButton::clicked, this, &PagePengiriman::onBuatPengiriman);
    connect(btnUpdate, &QPushButton::clicked, this, &PagePengiriman::onUpdateStatus);

    refresh();
}

void PagePengiriman::refresh() {
    tblPengiriman->setRowCount(0);
    PengirimanNode* curr = ctrl->getHeadPengiriman();
    int row = 0;
    while (curr) {
        DataPengiriman* p = curr->data;
        if (!p) { curr = curr->next; continue; }
        tblPengiriman->insertRow(row);
        tblPengiriman->setItem(row,0,new QTableWidgetItem(p->kodeResi));
        tblPengiriman->setItem(row,1,new QTableWidgetItem(p->idPosAsal));
        tblPengiriman->setItem(row,2,new QTableWidgetItem(QString::number(p->jumlahBalok)));
        tblPengiriman->setItem(row,3,new QTableWidgetItem(QString::number(p->totalBeratKg,'f',1)));
        tblPengiriman->setItem(row,4,new QTableWidgetItem(
                                           p->jarakOptimalKm >= INF_DIST ? "∞" : QString::number(p->jarakOptimalKm,'f',2)));

        QTableWidgetItem* stItem = new QTableWidgetItem(namaStatus(p->status));
        QColor stColor;
        switch (p->status) {
        case MENUNGGU:         stColor = QColor(0xd4a020); break;
        case DALAM_PERJALANAN: stColor = QColor(0x2080d0); break;
        case TIBA_DI_PABRIK:   stColor = QColor(0xa050d0); break;
        case SELESAI:          stColor = QColor(0x30c060); break;
        }
        stItem->setForeground(stColor);
        tblPengiriman->setItem(row,5, stItem);
        tblPengiriman->setItem(row,6,new QTableWidgetItem(p->waktuBerangkat.toString("dd/MM hh:mm")));
        curr = curr->next; row++;
    }

    int gudangSz = ctrl->getGudangSize();
    lblInfo->setText(QString("List Pengiriman: %1 record  ·  "
                             "Balok siap dimuat di gudang: %2 ")
                         .arg(ctrl->getTotalPengiriman()).arg(gudangSz));
}

void PagePengiriman::onBuatPengiriman() {
    if (ctrl->getGudangSize() == 0) {
        lblStatus->setStyleSheet("color:#dc3545;");
        lblStatus->setText("Gudang kosong. Proses setoran terlebih dahulu agar ada balok.");
        return;
    }
    // Ambil ID pos
    int idx = cmbPos->currentIndex();
    const DataWilayah* d = ctrl->getWilayah().get(idx);
    QString idPos = d ? d->idPos : "POS-GBG";
    int nodeAsal  = idx;

    DataPengiriman* p = ctrl->buatPengiriman(idPos, nodeAsal);
    if (!p) {
        lblStatus->setStyleSheet("color:#dc3545;");
        lblStatus->setText("Gagal membuat pengiriman. Pastikan pabrik terdaftar di peta.");
        return;
    }
    lblStatus->setStyleSheet("color:#28a745;");
    lblStatus->setText(QString("Pengiriman dibuat: %1 | %2 balok | %.2f km")
                           .arg(p->kodeResi).arg(p->jumlahBalok).arg(p->jarakOptimalKm));
    refresh();
}

void PagePengiriman::onUpdateStatus() {
    QString resi = edtUpdateResi->text().trimmed();
    if (resi.isEmpty()) return;
    StatusPengiriman st = (StatusPengiriman)cmbUpdateStatus->currentData().toInt();
    if (ctrl->updateStatusPengiriman(resi, st)) {
        lblStatus->setStyleSheet("color:#28a745;");
        lblStatus->setText("Status berhasil diupdate: " + resi + " → " + namaStatus(st));
        edtUpdateResi->clear();
        refresh();
    } else {
        lblStatus->setStyleSheet("color:#dc3545;");
        lblStatus->setText("Kode resi tidak ditemukan.");
    }
}
