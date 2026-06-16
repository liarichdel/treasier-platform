#include "PageNasabah.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QHeaderView>
#include <QMessageBox>

PageNasabah::PageNasabah(BankSampahController* c, QWidget* parent)
    : QWidget(parent), ctrl(c) {

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(14);

    QLabel* title = new QLabel("Database Nasabah", this);
    title->setStyleSheet("font-size:20px;font-weight:700;color:#212529;");
    QLabel* sub = new QLabel("Implementasi Hash Table dan Linked List", this);
    sub->setStyleSheet("font-size:11px;color:#8e44ad;font-weight:600;");
    root->addWidget(title);
    root->addWidget(sub);

    lblHashInfo = new QLabel("", this);
    lblHashInfo->setStyleSheet("background:#f3e5f5;border:1px solid #e1bee7;border-radius:6px;padding:8px 14px;color:#4a148c;font-size:11px;");
    root->addWidget(lblHashInfo);

    QSplitter* sp = new QSplitter(Qt::Horizontal, this);
    sp->setHandleWidth(8);

    // Panel Kiri: Daftar
    QWidget* leftPanel = new QWidget(sp);
    QVBoxLayout* lv = new QVBoxLayout(leftPanel);
    lv->setContentsMargins(0,0,0,0); lv->setSpacing(10);

    // Cari
    QHBoxLayout* cariRow = new QHBoxLayout();
    edtCariNama = new QLineEdit(leftPanel);
    edtCariNama->setPlaceholderText("Cari nama nasabah...");
    QPushButton* btnCari = new QPushButton("Cari", leftPanel);
    btnCari->setFixedWidth(70);
    QPushButton* btnRefresh = new QPushButton("", leftPanel);
    btnRefresh->setFixedWidth(40);
    btnRefresh->setObjectName("btnSecondary");
    cariRow->addWidget(edtCariNama, 1);
    cariRow->addWidget(btnCari);
    cariRow->addWidget(btnRefresh);
    lv->addLayout(cariRow);

    lblCount = new QLabel("", leftPanel);
    lblCount->setStyleSheet("color:#6c757d;font-size:11px;");
    lv->addWidget(lblCount);

    tblNasabah = new QTableWidget(leftPanel);
    tblNasabah->setColumnCount(4);
    tblNasabah->setHorizontalHeaderLabels({"Nama","KTP","Poin","Bucket#"});
    tblNasabah->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblNasabah->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblNasabah->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblNasabah->verticalHeader()->setVisible(false);
    tblNasabah->setAlternatingRowColors(true);
    tblNasabah->setStyleSheet("QTableWidget{alternate-background-color:#f8f9fa;}");
    lv->addWidget(tblNasabah, 1);

    // Form daftar
    QGroupBox* gbReg = new QGroupBox("Daftar Nasabah Baru", leftPanel);
    QVBoxLayout* gv = new QVBoxLayout(gbReg);
    gv->setSpacing(6);
    auto makeRow = [&](const QString& lbl, QLineEdit*& edt) {
        QHBoxLayout* h = new QHBoxLayout();
        QLabel* l = new QLabel(lbl + ":", gbReg);
        l->setFixedWidth(70);
        edt = new QLineEdit(gbReg);
        h->addWidget(l); h->addWidget(edt);
        gv->addLayout(h);
    };
    makeRow("No. KTP",  edtRegKTP);    edtRegKTP->setMaxLength(16); edtRegKTP->setPlaceholderText("16 digit NIK");
    makeRow("Nama",     edtRegNama);   edtRegNama->setPlaceholderText("Nama lengkap");
    makeRow("Alamat",   edtRegAlamat); edtRegAlamat->setPlaceholderText("Alamat lengkap");
    makeRow("No. Telp", edtRegTelp);   edtRegTelp->setPlaceholderText("08xxxxxxxxxx");
    QPushButton* btnDaftar = new QPushButton("Daftarkan", gbReg);
    gv->addWidget(btnDaftar);
    lv->addWidget(gbReg);

    sp->addWidget(leftPanel);

    // Panel Kanan: Detail
    QWidget* rightPanel = new QWidget(sp);
    QVBoxLayout* rv = new QVBoxLayout(rightPanel);
    rv->setContentsMargins(10,0,0,0); rv->setSpacing(10);

    QGroupBox* gbDetail = new QGroupBox("Detail Nasabah", rightPanel);
    QVBoxLayout* dv = new QVBoxLayout(gbDetail);
    lblDetailNama = new QLabel("— Pilih nasabah dari tabel —", gbDetail);
    lblDetailNama->setStyleSheet("font-size:14px;font-weight:600;color:#2c3e50;");
    lblDetailPoin = new QLabel("", gbDetail);
    lblDetailPoin->setStyleSheet("font-size:20px;font-weight:700;color:#28a745;");
    dv->addWidget(lblDetailNama);
    dv->addWidget(lblDetailPoin);

    tblRiwayat = new QTableWidget(gbDetail);
    tblRiwayat->setColumnCount(5);
    tblRiwayat->setHorizontalHeaderLabels({"Kode Resi","Jenis","Berat","Poin","Waktu"});
    tblRiwayat->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblRiwayat->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblRiwayat->verticalHeader()->setVisible(false);
    dv->addWidget(tblRiwayat, 1);
    rv->addWidget(gbDetail, 1);

    // Lookup KTP (Searching)
    QGroupBox* gbSearch = new QGroupBox("------------------   Pencarian KTP", rightPanel);
    QHBoxLayout* sh = new QHBoxLayout(gbSearch);
    QLineEdit* edtLookup = new QLineEdit(gbSearch);
    edtLookup->setPlaceholderText("Masukkan 16 digit KTP...");
    QPushButton* btnLookup = new QPushButton("Cari KTP", gbSearch);
    QLabel* lblLookupResult = new QLabel("", gbSearch);
    lblLookupResult->setStyleSheet("color:#495057;font-size:11px;");
    sh->addWidget(edtLookup, 1); sh->addWidget(btnLookup);
    QVBoxLayout* sv = new QVBoxLayout(); sv->addLayout(sh); sv->addWidget(lblLookupResult);
    QWidget* sgw = new QWidget(gbSearch);
    sgw->setLayout(sv);
    gbSearch->setLayout(sv);
    rv->addWidget(gbSearch);

    sp->addWidget(rightPanel);
    sp->setSizes({500, 600});
    root->addWidget(sp, 1);

    connect(btnCari,    &QPushButton::clicked,   this, &PageNasabah::onCariNama);
    connect(btnRefresh, &QPushButton::clicked,   this, &PageNasabah::refresh);
    connect(btnDaftar,  &QPushButton::clicked,   this, &PageNasabah::onDaftarNasabah);
    connect(tblNasabah, &QTableWidget::cellClicked, this, &PageNasabah::onNasabahSelected);
    connect(btnLookup,  &QPushButton::clicked, this, [=](){
        QString ktp = edtLookup->text().trimmed();
        NasabahNode* n = ctrl->getNasabahPtr(ktp);
        if (n) {
            lblLookupResult->setStyleSheet("color:#28a745;");
            lblLookupResult->setText(QString("Ditemukan: %1 | Poin: %2 | Bucket: %3")
                                         .arg(n->nama).arg(n->totalPoin).arg(ctrl->getDB().getHashIndex(ktp)));
        } else {
            lblLookupResult->setStyleSheet("color:#dc3545;");
            lblLookupResult->setText("KTP tidak ditemukan dalam Table.");
        }
    });

    refresh();
}

void PageNasabah::refresh() {
    tblNasabah->setRowCount(0);
    HashTableNasabah& ht = ctrl->getDB();
    int row = 0;
    for (int i = 0; i < ht.getBucketCount(); i++) {
        NasabahNode* curr = ht.getBucket(i);
        while (curr) {
            tblNasabah->insertRow(row);
            tblNasabah->setItem(row,0,new QTableWidgetItem(curr->nama));
            tblNasabah->setItem(row,1,new QTableWidgetItem(curr->noKTP));
            tblNasabah->setItem(row,2,new QTableWidgetItem(QString::number(curr->totalPoin)));
            tblNasabah->setItem(row,3,new QTableWidgetItem(QString::number(i)));
            curr = curr->nextChain;
            row++;
        }
    }
    lblCount->setText(QString("Total nasabah: %1 dari %2 bucket").arg(ctrl->getTotalNasabah()).arg(HASH_SIZE));

    // Hitung chain
    int maxChain = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        int len = 0; NasabahNode* c = ht.getBucket(i);
        while (c) { len++; c = c->nextChain; }
        if (len > maxChain) maxChain = len;
    }
    lblHashInfo->setText(QString("Hash Table  |  Ukuran bucket: %1  |  Nasabah terdaftar: %2  |  Chain terpanjang: %3  |  Load factor: %.2f")
                             .arg(HASH_SIZE).arg(ctrl->getTotalNasabah()).arg(maxChain)
                             .arg((double)ctrl->getTotalNasabah() / HASH_SIZE));
}

void PageNasabah::onDaftarNasabah() {
    QString ktp    = edtRegKTP->text().trimmed();
    QString nama   = edtRegNama->text().trimmed();
    QString alamat = edtRegAlamat->text().trimmed();
    QString telp   = edtRegTelp->text().trimmed();
    if (ktp.length() != 16 || nama.isEmpty()) {
        QMessageBox::warning(this, "Input Tidak Valid", "KTP harus 16 digit dan nama tidak boleh kosong.");
        return;
    }
    if (!ctrl->daftarNasabah(ktp, nama, alamat, telp)) {
        QMessageBox::warning(this, "Gagal", "KTP sudah terdaftar dalam sistem.");
        return;
    }
    edtRegKTP->clear(); edtRegNama->clear();
    edtRegAlamat->clear(); edtRegTelp->clear();
    refresh();
}

void PageNasabah::onCariNama() {
    QString nama = edtCariNama->text().trimmed();
    if (nama.isEmpty()) { refresh(); return; }
    NasabahNode* n = ctrl->cariNasabahNama(nama);
    tblNasabah->setRowCount(0);
    if (!n) {
        lblCount->setText("Tidak ditemukan: " + nama);
        return;
    }
    tblNasabah->insertRow(0);
    tblNasabah->setItem(0,0,new QTableWidgetItem(n->nama));
    tblNasabah->setItem(0,1,new QTableWidgetItem(n->noKTP));
    tblNasabah->setItem(0,2,new QTableWidgetItem(QString::number(n->totalPoin)));
    tblNasabah->setItem(0,3,new QTableWidgetItem(QString::number(ctrl->getDB().getHashIndex(n->noKTP))));
    lblCount->setText("Hasil searching: 1 nasabah ditemukan");
}

void PageNasabah::onLookupKTP() {}

void PageNasabah::onNasabahSelected(int row, int) {
    QTableWidgetItem* it = tblNasabah->item(row, 1);
    if (!it) return;
    QString ktp = it->text();
    NasabahNode* n = ctrl->getNasabahPtr(ktp);
    if (!n) return;

    lblDetailNama->setText(n->nama + " · " + n->noKTP);
    lblDetailPoin->setText(QString("%1 Poin").arg(n->totalPoin));

    tblRiwayat->setRowCount(0);
    TransaksiNode* tx = n->headRiwayat;
    int r = 0;
    while (tx) {
        tblRiwayat->insertRow(r);
        tblRiwayat->setItem(r,0,new QTableWidgetItem(tx->kodeResi));
        tblRiwayat->setItem(r,1,new QTableWidgetItem(namaJenisSampah(tx->jenis)));
        tblRiwayat->setItem(r,2,new QTableWidgetItem(QString::number(tx->beratKg,'f',1)+" kg"));
        QTableWidgetItem* poinItem = new QTableWidgetItem(
            (tx->poinMutasi >= 0 ? "+" : "") + QString::number(tx->poinMutasi));
        poinItem->setForeground(tx->poinMutasi >= 0 ? QColor(0x218838) : QColor(0xc82333));
        tblRiwayat->setItem(r,3,poinItem);
        tblRiwayat->setItem(r,4,new QTableWidgetItem(tx->waktu.toString("dd/MM/yy hh:mm")));
        tx = tx->next; r++;
    }
}
