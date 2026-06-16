#include "PageAntrean.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>

PageAntrean::PageAntrean(BankSampahController* c, QWidget* parent)
    : QWidget(parent), ctrl(c) {

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(16);

    QLabel* title = new QLabel("Antrean Warga", this);
    title->setStyleSheet("font-size:22px;font-weight:700;color:#212529;");
    QLabel* sub = new QLabel("Implementasi Queue", this);
    sub->setStyleSheet("font-size:12px;color:#28a745;font-weight:600;");
    root->addWidget(title);
    root->addWidget(sub);

    QGroupBox* gbInput = new QGroupBox("Tambah ke Antrean", this);
    QHBoxLayout* fh = new QHBoxLayout(gbInput);

    QLabel* l1 = new QLabel("No. KTP:", gbInput);
    edtKTP = new QLineEdit(gbInput);
    edtKTP->setPlaceholderText("16 digit NIK...");
    edtKTP->setMaxLength(16);

    QLabel* l2 = new QLabel("Jenis:", gbInput);
    cmbJenis = new QComboBox(gbInput);
    for (int i = 0; i < JENIS_COUNT; i++)
        cmbJenis->addItem(namaJenisSampah((JenisSampah)i));

    QLabel* l3 = new QLabel("Berat (kg):", gbInput);
    spnBerat = new QDoubleSpinBox(gbInput);
    spnBerat->setRange(0.1, 500.0);
    spnBerat->setSingleStep(0.5);
    spnBerat->setValue(1.0);
    spnBerat->setSuffix(" kg");

    QPushButton* btnTambah = new QPushButton("+ Masuk Antrean", gbInput);
    btnTambah->setFixedWidth(160);

    fh->addWidget(l1); fh->addWidget(edtKTP, 2);
    fh->addWidget(l2); fh->addWidget(cmbJenis, 1);
    fh->addWidget(l3); fh->addWidget(spnBerat, 1);
    fh->addWidget(btnTambah);
    root->addWidget(gbInput);

    // Aksi antrean
    QHBoxLayout* actRow = new QHBoxLayout();
    QPushButton* btnPanggil  = new QPushButton("Panggil Berikutnya", this);
    QPushButton* btnProses   = new QPushButton("Proses Setoran", this);
    btnPanggil->setObjectName("btnSecondary");
    btnProses->setObjectName("btnSecondary");
    lblProses = new QLabel("", this);
    lblProses->setStyleSheet("color:#1e7e34;font-size:13px;");
    actRow->addWidget(btnPanggil);
    actRow->addWidget(btnProses);
    actRow->addWidget(lblProses, 1);
    actRow->addStretch();
    root->addLayout(actRow);

    // Tabel antrean
    lblUkuran = new QLabel("Antrean: 0 orang", this);
    lblUkuran->setStyleSheet("color:#495057;font-size:12px;");
    root->addWidget(lblUkuran);

    tblAntrean = new QTableWidget(this);
    tblAntrean->setColumnCount(5);
    tblAntrean->setHorizontalHeaderLabels({"No. Antrian","No. KTP","Jenis Sampah","Berat (kg)","Waktu Tiba"});
    tblAntrean->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblAntrean->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblAntrean->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblAntrean->verticalHeader()->setVisible(false);
    root->addWidget(tblAntrean, 1);

    lblStatus = new QLabel("", this);
    lblStatus->setStyleSheet("color:#28a745;font-size:12px;");
    root->addWidget(lblStatus);

    connect(btnTambah,  &QPushButton::clicked, this, &PageAntrean::onTambahAntrean);
    connect(btnPanggil, &QPushButton::clicked, this, &PageAntrean::onPanggilBerikutnya);
    connect(btnProses,  &QPushButton::clicked, this, &PageAntrean::onProsesSetoran);

    refresh();
}

void PageAntrean::refresh() {
    tblAntrean->setRowCount(0);
    AntreanNode* curr = ctrl->getAntrean().getHead();
    int row = 0;
    while (curr) {
        tblAntrean->insertRow(row);
        tblAntrean->setItem(row, 0, new QTableWidgetItem(curr->data.noAntrian));
        tblAntrean->setItem(row, 1, new QTableWidgetItem(curr->data.noKTPWarga));
        tblAntrean->setItem(row, 2, new QTableWidgetItem(namaJenisSampah(curr->data.jenis)));
        tblAntrean->setItem(row, 3, new QTableWidgetItem(QString::number(curr->data.beratKg, 'f', 1)));
        tblAntrean->setItem(row, 4, new QTableWidgetItem(curr->data.waktuTiba.toString("hh:mm:ss")));
        // Warnai baris pertama (HEAD / akan dipanggil)
        if (row == 0) {
            for (int c = 0; c < 5; c++) {
                if (tblAntrean->item(row, c)) {
                    tblAntrean->item(row,c)->setBackground(QColor(0xe8f5e9));
                    tblAntrean->item(row, c)->setForeground(QColor(0x1b5e20));
                }
            }
        }
        curr = curr->next;
        row++;
    }
    lblUkuran->setText(QString("Antrean: %1 orang").arg(ctrl->getAntrean().getUkuran()));
}

void PageAntrean::onTambahAntrean() {
    QString ktp = edtKTP->text().trimmed();
    if (ktp.length() != 16) {
        lblStatus->setStyleSheet("color:#dc3545;font-size:12px;");
        lblStatus->setText("KTP harus 16 digit.");
        return;
    }
    JenisSampah j = (JenisSampah)cmbJenis->currentIndex();
    double berat  = spnBerat->value();
    QString no = ctrl->wargaTiba(ktp, j, berat);
    lblStatus->setStyleSheet("color:#28a745;font-size:12px;");
    lblStatus->setText(QString("Warga masuk antrean: %1 | Jenis: %2 | %.1f kg")
                           .arg(no).arg(namaJenisSampah(j)).arg(berat));
    edtKTP->clear();
    refresh();
}

void PageAntrean::onPanggilBerikutnya() {
    DataSetoran s;
    if (!ctrl->getAntrean().peek(s)) {
        lblProses->setStyleSheet("color:#dc3545;");
        lblProses->setText("Antrean kosong.");
        return;
    }
    lblProses->setStyleSheet("color:#b58105;");
    lblProses->setText(QString("Memanggil: %1 | KTP: %2 | %3")
                           .arg(s.noAntrian).arg(s.noKTPWarga).arg(namaJenisSampah(s.jenis)));
}

void PageAntrean::onProsesSetoran() {
    DataSetoran s;
    if (!ctrl->panggilBerikutnya(s)) {
        lblProses->setStyleSheet("color:#dc3545;");
        lblProses->setText("Tidak ada yang diproses.");
        return;
    }
    QString resi = ctrl->prosesSetoran(s);
    if (resi.isEmpty()) {
        lblProses->setStyleSheet("color:#d96b27;");
        lblProses->setText("Setoran ditolak: berat di bawah minimum untuk jenis " + namaJenisSampah(s.jenis));
    } else {
        lblProses->setStyleSheet("color:#28a745;");
        lblProses->setText(QString(" Setoran diproses | Resi: %1 | Poin: +%2")
                               .arg(resi)
                               .arg(ctrl->getPohon().hitungPoin(s.jenis, s.beratKg)));
    }
    refresh();
}
