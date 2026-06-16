#include "PageGudang.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QProgressBar>
#include <QTreeWidget>
#include <QHeaderView>
#include <QMessageBox>

PageGudang::PageGudang(BankSampahController* c, QWidget* parent)
    : QWidget(parent), ctrl(c), treeHarga(nullptr) {

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(28,24,28,24);
    root->setSpacing(14);

    QLabel* title = new QLabel("Gudang", this);
    title->setStyleSheet("font-size:20px;font-weight:700;color:#212529;");
    QLabel* sub = new QLabel("Implementasi BST Tree dan Stack", this);
    sub->setStyleSheet("font-size:11px;color:#e67e22;font-weight:600;");
    root->addWidget(title);
    root->addWidget(sub);

    QSplitter* sp = new QSplitter(Qt::Horizontal, this);

    // Kiri: Stack Gudang
    QWidget* leftW = new QWidget(sp);
    QVBoxLayout* lv = new QVBoxLayout(leftW);
    lv->setContentsMargins(0,0,0,0); lv->setSpacing(10);

    // Stat row
    QHBoxLayout* statRow = new QHBoxLayout();
    auto makeMini = [&](const QString& lbl, QLabel*& out, const QString& color) {
        QFrame* f = new QFrame(leftW);
        f->setStyleSheet(QString("background:#ffffff;border:1px solid %1;border-radius:8px;padding:10px;").arg(color));
        QVBoxLayout* fv = new QVBoxLayout(f);
        out = new QLabel("0", f);
        out->setStyleSheet(QString("font-size:24px;font-weight:700;color:%1;").arg(color));
        QLabel* l = new QLabel(lbl, f);
        l->setStyleSheet("font-size:11px;color:#6c757d;");
        fv->addWidget(out); fv->addWidget(l);
        return f;
    };
    statRow->addWidget(makeMini("Balok di Gudang", lblUkuran, "#e67e22"));
    statRow->addWidget(makeMini("Kapasitas",       lblKapasitas, "#2980b9"));
    lv->addLayout(statRow);

    lblTop = new QLabel("TOP → (gudang kosong)", leftW);
    lblTop->setStyleSheet("background:#fff9db;border:1px solid #ffe3e3;border-radius:6px;padding:8px 14px;color:#b58105;font-size:12px;font-weight:600;");
    lv->addWidget(lblTop);

    // Stack visual
    lstStack = new QListWidget(leftW);
    lstStack->setStyleSheet(R"(
        QListWidget { background:#ffffff; border:1px solid #dee2e6; border-radius:8px; }
        QListWidget::item { padding:8px 14px; border-bottom:1px solid #f1f3f5; color:#212529; }
        QListWidget::item:first-child { background:#e8f5e9; color:#1b5e20; font-weight:600; }
    )");
    lv->addWidget(lstStack, 1);

    QHBoxLayout* actRow = new QHBoxLayout();
    QPushButton* btnPop = new QPushButton("Pop (Ambil Balok Teratas)", leftW);
    btnPop->setObjectName("btnSecondary");
    actRow->addWidget(btnPop);
    actRow->addStretch();
    lv->addLayout(actRow);

    sp->addWidget(leftW);

    // Kanan: BST Tree Harga
    QWidget* rightW = new QWidget(sp);
    QVBoxLayout* rv = new QVBoxLayout(rightW);
    rv->setContentsMargins(10,0,0,0); rv->setSpacing(10);

    QGroupBox* gbTree = new QGroupBox("Klasifikasi Harga Sampah", rightW);
    QVBoxLayout* tv = new QVBoxLayout(gbTree);

    lblTreeInfo = new QLabel("", gbTree);
    lblTreeInfo->setStyleSheet("color:#495057;font-size:11px;");
    tv->addWidget(lblTreeInfo);

    treeHarga = new QTreeWidget(gbTree);
    treeHarga->setColumnCount(4);
    treeHarga->setHeaderLabels({"Jenis Sampah","Poin/kg","Min. Berat","Keterangan"});
    treeHarga->header()->setSectionResizeMode(QHeaderView::Stretch);
    tv->addWidget(treeHarga, 1);
    rv->addWidget(gbTree, 1);

    sp->addWidget(rightW);
    sp->setSizes({500, 580});
    root->addWidget(sp, 1);

    connect(btnPop, &QPushButton::clicked, this, &PageGudang::onPopBalok);
    refresh();
}

void PageGudang::refresh() {
    // Stack
    lstStack->clear();
    StackNode* curr = ctrl->getGudang().getTop();
    bool first = true;
    while (curr) {
        QString txt = (first ? "▶ TOP ▶  " : "          ") +
                      curr->data.idBalok + "  |  " +
                      namaJenisSampah(curr->data.jenis) + "  |  " +
                      QString::number(curr->data.beratKg,'f',1) + " kg  |  " +
                      curr->data.noKTPAsal;
        QListWidgetItem* item = new QListWidgetItem(txt, lstStack);
        if (first) { item->setForeground(QColor(0x1b5e20)); first = false; }
        curr = curr->next;
    }

    int sz  = ctrl->getGudang().getUkuran();
    int kap = ctrl->getGudang().getKapasitas();
    lblUkuran->setText(QString::number(sz));
    lblKapasitas->setText(QString::number(kap));

    BalokSampah top;
    if (ctrl->getGudang().peek(top))
        lblTop->setText("TOP →  " + top.idBalok + "  |  " + namaJenisSampah(top.jenis) + "  |  " + QString::number(top.beratKg,'f',1) + " kg");
    else
        lblTop->setText("TOP → (gudang kosong)");

    // BST Tree
    treeHarga->clear();
    QStringList inorder = ctrl->getPohon().getInorderList();
    int idx = 0;
    // Jenis mapping untuk tampilan berstruktur
    struct JenisInfo { JenisSampah j; };
    JenisSampah urutan[] = { ORGANIK, TEKSTIL, KACA, KERTAS, PLASTIK, LOGAM, ELEKTRONIK };
    for (JenisSampah j : urutan) {
        double ppk = ctrl->getPohon().getPoinPerKg(j);
        if (ppk == 0.0) continue;
        QTreeWidgetItem* item = new QTreeWidgetItem(treeHarga);
        item->setText(0, namaJenisSampah(j));
        item->setText(1, QString::number(ppk,'f',0) + " poin");
        item->setText(2, QString::number(ctrl->getPohon().getMinKg(j),'f',1) + " kg");
        QString ket;
        if (ppk >= 500)      ket = "Premium";
        else if (ppk >= 200) ket = "Tinggi";
        else if (ppk >= 80)  ket = "Sedang";
        else                 ket = "Rendah";
        item->setText(3, ket);
        QColor c;
        if (ppk >= 500)      c = QColor(0xd9a711);
        else if (ppk >= 200) c = QColor(0x1e7e34);
        else if (ppk >= 80)  c = QColor(0x1f618d);
        else                 c = QColor(0x5d6d7e);
        for (int col = 0; col < 4; col++) item->setForeground(col, c);
        idx++;
    }
    treeHarga->expandAll();
    lblTreeInfo->setText(QString(" %1 kategori terdaftar ").arg(idx));
}

void PageGudang::onPopBalok() {
    BalokSampah b;
    if (!ctrl->getGudang().pop(b)) {
        QMessageBox::information(this, "Gudang Kosong", "Tidak ada balok di gudang.");
        return;
    }
    QMessageBox::information(this, "Balok Diambil",
                             QString("ID: %1\nJenis: %2\nBerat: %3 kg\nDari: %4")
                                 .arg(b.idBalok).arg(namaJenisSampah(b.jenis)).arg(b.beratKg).arg(b.noKTPAsal));
    refresh();
}
