#include "PageDashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFrame>
#include <QScrollArea>

PageDashboard::PageDashboard(BankSampahController* c, QWidget* parent)
    : QWidget(parent), ctrl(c) {

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(20);

    QLabel* title = new QLabel("Dashboard", this);
    title->setStyleSheet("font-size:24px; font-weight:700; color:#212529;");
    QLabel* sub = new QLabel("Sistem ini dirancang untuk mengotomatisasi seluruh alur operasional bank sampah mulai "
                             "dari antrean kedatangan warga, klasifikasi harga, manajemen tumpukan gudang, hingga visualisasi "
                             "rute logistik tercepat secara instan, aman, dan efisien.", this);
    sub->setWordWrap(true);
    sub->setStyleSheet("font-size:13px; color:#5a7a9a;");
    root->addWidget(title);
    root->addWidget(sub);

    QHBoxLayout* cardRow = new QHBoxLayout();
    cardRow->setSpacing(16);
    cardRow->addWidget(makeStatCard("Total Nasabah",   "#28a745", lblNasabah));
    cardRow->addWidget(makeStatCard("Antrean Aktif",   "#e67e22", lblAntrean));
    cardRow->addWidget(makeStatCard("Balok di Gudang", "#2980b9", lblGudang));
    cardRow->addWidget(makeStatCard("Pengiriman",      "#8e44ad", lblPengiriman));
    root->addLayout(cardRow);

    //struktur data aktif
    QGroupBox* gbSD = new QGroupBox("Bentuk Implementasi Struktur Data", this);
    gbSD->setStyleSheet("QGroupBox{border:1px solid #dee2e6; border-radius:10px;margin-top:12px;padding-top:12px;color:#495057;font-weight:700;}");
    QHBoxLayout* sdRow = new QHBoxLayout(gbSD);
    sdRow->setSpacing(12);

    auto makeTag = [&](const QString& text, const QString& color) {
        QLabel* tag = new QLabel(text);
        tag->setStyleSheet(QString("background:%1;color:#fff;border-radius:6px;padding:6px 14px;font-size:12px;font-weight:600;").arg(color));
        tag->setAlignment(Qt::AlignCenter);
        return tag;
    };
    sdRow->addWidget(makeTag("Queue",       "#28a745"));
    sdRow->addWidget(makeTag("BST Tree",    "#2980b9"));
    sdRow->addWidget(makeTag("Hash Table",  "#8e44ad"));
    sdRow->addWidget(makeTag("Linked List", "#dc3545"));
    sdRow->addWidget(makeTag("Stack",       "#e67e22"));
    sdRow->addWidget(makeTag("Sorting",     "#16a085"));
    sdRow->addWidget(makeTag("Searching",   "#7f8c8d"));
    sdRow->addWidget(makeTag("Graph",       "#2c3e50"));
    root->addWidget(gbSD);

    //info wilayah kritis
    QFrame* kritisCard = new QFrame(this);
    kritisCard->setStyleSheet("background:#fff5f5;border:1px solid #f5c6cb;border-radius:10px;padding:16px;");
    QHBoxLayout* kh = new QHBoxLayout(kritisCard);
    QLabel* kIcon = new QLabel("⚠", kritisCard);
    kIcon->setStyleSheet("font-size:28px;");
    lblKritis = new QLabel("", kritisCard);
    lblKritis->setStyleSheet("color:#721c24;font-size:13px;");
    lblKritis->setWordWrap(true);
    kh->addWidget(kIcon);
    kh->addWidget(lblKritis, 1);
    root->addWidget(kritisCard);

    //resi terakhir
    QFrame* resiCard = new QFrame(this);
    resiCard->setStyleSheet("background:#f1f7fc;border:1px solid #d6e4f0;border-radius:10px;padding:16px;");
    QVBoxLayout* rv = new QVBoxLayout(resiCard);
    QLabel* rLabel = new QLabel("Aktivitas Terbaru", resiCard);
    rLabel->setStyleSheet("color:#2b6cb0;font-size:11px;font-weight:700;");
    lblResiTerakhir = new QLabel("—", resiCard);
    lblResiTerakhir->setStyleSheet("color:#212529;font-size:13px;");
    rv->addWidget(rLabel);
    rv->addWidget(lblResiTerakhir);
    root->addWidget(resiCard);

    root->addStretch();
    refresh();
}

QWidget* PageDashboard::makeStatCard(const QString& title, const QString& color, QLabel*& valLabel) {
    QFrame* card = new QFrame(this);
    card->setStyleSheet(QString("background:#ffffff;border:1px solid %1;border-radius:12px;padding:20px;").arg(color));
    QVBoxLayout* vl = new QVBoxLayout(card);
    vl->setSpacing(8);
    valLabel = new QLabel("0", card);
    valLabel->setStyleSheet(QString("font-size:36px;font-weight:700;color:%1;").arg(color));
    QLabel* lbl = new QLabel(title, card);
    lbl->setStyleSheet("font-size:12px;color:#6c757d;");
    vl->addWidget(valLabel);
    vl->addWidget(lbl);
    return card;
}

void PageDashboard::refresh() {
    lblNasabah->setText(QString::number(ctrl->getTotalNasabah()));
    lblAntrean->setText(QString::number(ctrl->getAntreanSize()));
    lblGudang->setText(QString::number(ctrl->getGudangSize()));
    lblPengiriman->setText(QString::number(ctrl->getTotalPengiriman()));

    //wilayah kritis
    ArrayWilayah& w = ctrl->getWilayah();
    QStringList kritis;
    for (int i = 0; i < w.getUkuran(); i++) {
        const DataWilayah* d = w.get(i);
        if (d && d->statusKritis)
            kritis << QString("%1 (%2 ton)").arg(d->namaKecamatan).arg(d->totalVolumeTon);
    }
    if (kritis.isEmpty())
        lblKritis->setText("Tidak ada wilayah dalam status kritis saat ini.");
    else
        lblKritis->setText("Wilayah Kritis: " + kritis.join(" · "));

    //resi terakhir
    PengirimanNode* p = ctrl->getHeadPengiriman();
    if (p && p->data)
        lblResiTerakhir->setText("Pengiriman terbaru: " + p->data->kodeResi +
                                 " | " + namaStatus(p->data->status));
    else
        lblResiTerakhir->setText("Belum ada aktivitas pengiriman.");
}
