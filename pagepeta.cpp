#include "PagePeta.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QHeaderView>
#include <QSplitter>
#include <QPainterPath>
#include <QFontMetrics>
#include <QToolTip>
#include <cmath>

GrafCanvas::GrafCanvas(GrafJaringan* g, QWidget* parent)
    : QWidget(parent), graf(g), panjangJalur(0), adaJalur(false), hoveredNode(-1) {
    setMouseTracking(true);
    setMinimumSize(500, 380);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    for (int i = 0; i < MAKS_NODE; i++) { nodePos[i] = {0,0}; jalurHighlight[i] = -1; }
}

void GrafCanvas::hitungPosisi() {
    int n = graf->getJumlahNode();
    if (n == 0) return;

    //cari bounding box lat/lon
    double minLat = 1e9, maxLat = -1e9, minLon = 1e9, maxLon = -1e9;
    for (int i = 0; i < n; i++) {
        const LokasiNode& loc = graf->getLokasi(i);
        if (loc.latitude  < minLat) minLat = loc.latitude;
        if (loc.latitude  > maxLat) maxLat = loc.latitude;
        if (loc.longitude < minLon) minLon = loc.longitude;
        if (loc.longitude > maxLon) maxLon = loc.longitude;
    }

    double padX = 60, padY = 60;
    double W = width()  - 2*padX;
    double H = height() - 2*padY;
    double dLat = maxLat - minLat;
    double dLon = maxLon - minLon;
    if (dLat < 1e-9) dLat = 1e-9;
    if (dLon < 1e-9) dLon = 1e-9;

    for (int i = 0; i < n; i++) {
        const LokasiNode& loc = graf->getLokasi(i);
        nodePos[i].x = (int)(padX + (loc.longitude - minLon) / dLon * W);
        nodePos[i].y = (int)(padY + (maxLat - loc.latitude)  / dLat * H);
    }
}

bool GrafCanvas::isInJalur(int u, int v) const {
    if (!adaJalur || panjangJalur < 2) return false;
    for (int i = 0; i < panjangJalur - 1; i++) {
        if ((jalurHighlight[i]==u && jalurHighlight[i+1]==v) ||
            (jalurHighlight[i]==v && jalurHighlight[i+1]==u))
            return true;
    }
    return false;
}

int GrafCanvas::nodeDekat(QPoint p) const {
    int n = graf->getJumlahNode();
    for (int i = 0; i < n; i++) {
        int dx = nodePos[i].x - p.x();
        int dy = nodePos[i].y - p.y();
        if (dx*dx + dy*dy <= 18*18) return i;
    }
    return -1;
}

void GrafCanvas::setJalur(const int* jalur, int panjang) {
    adaJalur    = (panjang > 0 && jalur != nullptr);
    panjangJalur = panjang;
    for (int i = 0; i < panjang && i < MAKS_NODE; i++) jalurHighlight[i] = jalur[i];
    update();
}

void GrafCanvas::clearJalur() {
    adaJalur    = false;
    panjangJalur = 0;
    update();
}

void GrafCanvas::mouseMoveEvent(QMouseEvent* e) {
    mousePos = e->pos();
    int prev = hoveredNode;
    hoveredNode = nodeDekat(e->pos());
    if (hoveredNode != prev) update();
}

void GrafCanvas::leaveEvent(QEvent*) {
    hoveredNode = -1;
    update();
}

void GrafCanvas::paintEvent(QPaintEvent*) {
    hitungPosisi();
    int n = graf->getJumlahNode();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.fillRect(rect(), QColor(0xfdfdfd));

    p.setPen(QPen(QColor(0xeaeaea), 1, Qt::SolidLine));
    for (int x = 0; x < width(); x += 40)  p.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += 40) p.drawLine(0, y, width(), y);

    if (n == 0) {
        p.setPen(QColor(0x7f8c8d));
        p.setFont(QFont("Segoe UI", 13));
        p.drawText(rect(), Qt::AlignCenter, "Graf belum diinisialisasi");
        return;
    }

    for (int u = 0; u < n; u++) {
        EdgeNode* edge = graf->getAdjList(u);
        while (edge) {
            int v = edge->idTetangga;
            if (v > u) { // Gambar tiap edge sekali
                bool highlight = isInJalur(u, v);

                if (highlight) {
                    // Glow effect: gambar 3 layer
                    p.setPen(QPen(QColor(80, 220, 120, 40), 12, Qt::SolidLine, Qt::RoundCap));
                    p.drawLine(nodePos[u].x, nodePos[u].y, nodePos[v].x, nodePos[v].y);
                    p.setPen(QPen(QColor(80, 220, 120, 100), 5, Qt::SolidLine, Qt::RoundCap));
                    p.drawLine(nodePos[u].x, nodePos[u].y, nodePos[v].x, nodePos[v].y);
                    p.setPen(QPen(QColor(0x28a745), 2, Qt::SolidLine, Qt::RoundCap));
                } else {
                    p.setPen(QPen(QColor(0xcbd5e0), 1.5, Qt::SolidLine, Qt::RoundCap));
                }
                p.drawLine(nodePos[u].x, nodePos[u].y, nodePos[v].x, nodePos[v].y);

                // Label bobot di tengah edge
                int mx = (nodePos[u].x + nodePos[v].x) / 2;
                int my = (nodePos[u].y + nodePos[v].y) / 2;
                p.setFont(QFont("Segoe UI", 8));
                if (highlight)
                    p.setPen(QColor(0x1e7e34));
                else
                    p.setPen(QColor(0x718096));
                p.drawText(mx - 14, my - 5, 28, 16, Qt::AlignCenter,
                           QString::number(edge->bobot, 'f', 1));
            }
            edge = edge->next;
        }
    }

    for (int i = 0; i < n; i++) {
        const LokasiNode& loc = graf->getLokasi(i);
        int  cx = nodePos[i].x;
        int  cy = nodePos[i].y;
        bool isPabrik  = loc.isPabrikUtama;
        bool isInPath  = false;
        bool isHovered = (i == hoveredNode);

        for (int j = 0; j < panjangJalur && adaJalur; j++)
            if (jalurHighlight[j] == i) { isInPath = true; break; }

        int  r  = isPabrik ? 18 : 14;
        QColor fillColor, borderColor;

        if (isPabrik) {
            fillColor   = QColor(0xffeeba);
            borderColor = QColor(0xffc107);
        } else if (isInPath) {
            fillColor   = QColor(0xd4edda);
            borderColor = QColor(0x28a745);
        } else if (isHovered) {
            fillColor   = QColor(0xe2e8f0);
            borderColor = QColor(0x4a5568);
        } else {
            fillColor   = QColor(0xf7fafc);
            borderColor = QColor(0xa0aec0);
        }

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, 80));
        p.drawEllipse(cx - r + 3, cy - r + 3, r*2, r*2);

        p.setBrush(fillColor);
        p.setPen(QPen(borderColor, isInPath || isPabrik ? 2.5 : 1.5));
        p.drawEllipse(cx - r, cy - r, r*2, r*2);

        p.setFont(QFont("Segoe UI", isPabrik ? 11 : 9, QFont::Bold));
        p.setPen(isPabrik ? QColor(0x856404) : QColor(0x2d3748));
        p.drawText(cx - r, cy - r, r*2, r*2, Qt::AlignCenter, isPabrik ? "🏭" : "♻");

        // Label nama
        p.setFont(QFont("Segoe UI", 8));
        p.setPen(isInPath ? QColor(0x155724) : (isPabrik ? QColor(0x856404) : QColor(0x2d3748)));
        QFontMetrics fm(p.font());
        QString shortName = loc.namaLokasi.length() > 14
                                ? loc.namaLokasi.left(12) + "…"
                                : loc.namaLokasi;
        int tw = fm.horizontalAdvance(shortName);
        int tx = cx - tw/2;
        int ty = cy + r + 14;
        // Latar label
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(240, 244, 248, 220));
        p.drawRoundedRect(tx - 3, ty - 11, tw + 6, 14, 3, 3);
        p.setPen(isInPath ? QColor(0x1e7e34) : (isPabrik ? QColor(0xb58105) : QColor(0x4a5568)));
        p.drawText(tx, ty, shortName);
        p.setFont(QFont("Segoe UI", 7));
        p.setPen(QColor(0xa0aec0));
        p.drawText(cx - r, cy - r - 12, r*2, 12, Qt::AlignCenter, QString("[%1]").arg(i));
    }

    if (adaJalur && panjangJalur >= 2) {
        p.setPen(QPen(QColor(0x28a745), 2));
        p.setBrush(QColor(0x28a745));
        for (int i = 0; i < panjangJalur - 1; i++) {
            int u = jalurHighlight[i], v = jalurHighlight[i+1];
            double dx = nodePos[v].x - nodePos[u].x;
            double dy = nodePos[v].y - nodePos[u].y;
            double len = std::sqrt(dx*dx + dy*dy);
            if (len < 1e-6) continue;
            double ux = dx/len, uy = dy/len;
            double r = 16;
            double ax = nodePos[v].x - ux*r;
            double ay = nodePos[v].y - uy*r;
            double perpX = -uy * 5, perpY = ux * 5;
            QPolygonF arrow;
            arrow << QPointF(ax, ay)
                  << QPointF(ax - ux*10 + perpX, ay - uy*10 + perpY)
                  << QPointF(ax - ux*10 - perpX, ay - uy*10 - perpY);
            p.drawPolygon(arrow);
        }
    }

    if (hoveredNode >= 0) {
        const LokasiNode& loc = graf->getLokasi(hoveredNode);
        QString tip = QString("Node %1: %2\n(%.4f, %.4f)")
                          .arg(hoveredNode).arg(loc.namaLokasi)
                          .arg(loc.latitude).arg(loc.longitude);
        int tx = nodePos[hoveredNode].x + 20;
        int ty = nodePos[hoveredNode].y - 10;
        p.setFont(QFont("Segoe UI", 9));
        QFontMetrics fm(p.font());
        QRect tr = fm.boundingRect(QRect(tx, ty, 200, 60), Qt::TextWordWrap, tip);
        tr.adjust(-6, -4, 6, 4);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(20, 40, 60, 220));
        p.drawRoundedRect(tr, 6, 6);
        p.setPen(QColor(0xffffff));
        p.drawText(tr.adjusted(6, 4, -6, -4), Qt::TextWordWrap, tip);
    }

    p.setFont(QFont("Segoe UI", 9));
    int lx = 12, ly = height() - 70;
    auto drawLegendItem = [&](const QColor& c, const QString& txt) {
        p.setPen(Qt::NoPen);
        p.setBrush(c);
        p.drawEllipse(lx, ly - 6, 10, 10);
        p.setPen(QColor(0x4a5568));
        p.drawText(lx + 14, ly + 3, txt);
        ly += 18;
    };
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(248, 249, 250, 230));
    p.drawRoundedRect(6, height()-80, 170, 76, 6, 6);
    drawLegendItem(QColor(0xffc107), "Pabrik Daur Ulang");
    drawLegendItem(QColor(0x28a745), "Jalur Terpendek");
    drawLegendItem(QColor(0xcbd5e0), "Pos Pengumpulan");
    p.setPen(QPen(QColor(0xcbd5e0), 1.5));
    p.drawLine(lx, ly-4, lx+10, ly-4);
    p.setPen(QColor(0x4a5568));
    p.drawText(lx+14, ly+3, "Jalan / Edge");
}

PagePeta::PagePeta(BankSampahController* c, QWidget* parent)
    : QWidget(parent), ctrl(c), jalurTerakhir(nullptr), panjangTerakhir(0) {

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(14);

    QLabel* title = new QLabel("Peta Rute Distribusi", this);
    title->setStyleSheet("font-size:20px;font-weight:700;color: #212529;");
    QLabel* sub = new QLabel("Implementasi Weighted Graph", this);
    sub->setStyleSheet("font-size:11px;color:#1a3a70;font-weight:600;");
    root->addWidget(title);
    root->addWidget(sub);

    lblLegend = new QLabel("", this);
    lblLegend->setStyleSheet("background:#e6f0fa; border:1px solid #b8daff;border-radius:6px;padding:8px 14px;color:#004085;font-size:11px;");
    root->addWidget(lblLegend);

    QSplitter* sp = new QSplitter(Qt::Horizontal, this);
    sp->setHandleWidth(6);

    QWidget* leftW = new QWidget(sp);
    QVBoxLayout* lv = new QVBoxLayout(leftW);
    lv->setContentsMargins(0,0,0,0); lv->setSpacing(8);

    canvas = new GrafCanvas(&ctrl->getPeta(), leftW);
    canvas->setStyleSheet("border:1px solid #cbd5e0;border-radius:10px;");
    lv->addWidget(canvas, 1);

    sp->addWidget(leftW);

    QWidget* rightW = new QWidget(sp);
    QVBoxLayout* rv = new QVBoxLayout(rightW);
    rv->setContentsMargins(10,0,0,0); rv->setSpacing(10);

    // Pilih titik asal
    QGroupBox* gbDijkstra = new QGroupBox("Rute Terpendek ke Pabrik", rightW);
    gbDijkstra->setStyleSheet("QGroupBox{border:1px solid #1a3060;border-radius:8px;margin-top:12px;padding-top:12px;color:#4a80d0;font-weight:700;}");
    QVBoxLayout* dv = new QVBoxLayout(gbDijkstra);

    QLabel* lAsal = new QLabel("Pilih Pos Asal:", gbDijkstra);
    lAsal->setStyleSheet("color:#4a5568;font-size:12px;");
    cmbAsal = new QComboBox(gbDijkstra);
    dv->addWidget(lAsal);
    dv->addWidget(cmbAsal);

    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* btnHitung = new QPushButton("Cari Rute", gbDijkstra);
    btnHitung->setStyleSheet("background:#0056b3; color:#ffffff;font-weight:700;");
    QPushButton* btnClear  = new QPushButton("Clear", gbDijkstra);
    btnClear->setObjectName("btnSecondary");
    btnRow->addWidget(btnHitung, 2);
    btnRow->addWidget(btnClear, 1);
    dv->addLayout(btnRow);

    lblHasil = new QLabel("Pilih pos asal lalu klik Cari Rute.", gbDijkstra);
    lblHasil->setStyleSheet("background:#f0f4f8 border:1px solid #cbd5e0 border-radius:6px;padding:10px;color:#2d3748;font-size:12px;");
    lblHasil->setWordWrap(true);
    dv->addWidget(lblHasil);

    txtJalur = new QTextEdit(gbDijkstra);
    txtJalur->setReadOnly(true);
    txtJalur->setStyleSheet("background:#f8f9fa;border:1px solid #e2e8f0;border-radius:6px;color:#2d3748;font-family:Consolas,monospace;font-size:12px;");
    txtJalur->setMaximumHeight(110);
    txtJalur->setPlaceholderText("Jalur terpendek akan ditampilkan di sini...");
    dv->addWidget(txtJalur);

    rv->addWidget(gbDijkstra);

    // Tabel jarak semua node
    QGroupBox* gbTbl = new QGroupBox("Jarak Terpendek dari Pos Asal ke Semua Lokasi", rightW);
    gbTbl->setStyleSheet("QGroupBox{border:1px solid #cbd5e0;border-radius:8px;margin-top:8px;padding-top:12px;color:#2d3748;font-weight:700;}");
    QVBoxLayout* tv = new QVBoxLayout(gbTbl);
    tblJarak = new QTableWidget(gbTbl);
    tblJarak->setColumnCount(4);
    tblJarak->setHorizontalHeaderLabels({"ID", "Lokasi", "Jarak (km)", "Jalur Melalui"});
    tblJarak->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblJarak->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblJarak->verticalHeader()->setVisible(false);
    tblJarak->setMaximumHeight(220);
    tv->addWidget(tblJarak);
    rv->addWidget(gbTbl);
    rv->addStretch();

    sp->addWidget(rightW);
    sp->setSizes({640, 440});
    root->addWidget(sp, 1);

    connect(btnHitung, &QPushButton::clicked, this, &PagePeta::onHitungRute);
    connect(btnClear,  &QPushButton::clicked, this, &PagePeta::onClearJalur);

    refresh();
}

PagePeta:: ~PagePeta() {
    delete[] jalurTerakhir;
}

void PagePeta::populateCombo() {
    cmbAsal->clear();
    GrafJaringan& peta = ctrl->getPeta();
    int n = peta.getJumlahNode();
    for (int i = 0; i < n; i++) {
        const LokasiNode& loc = peta.getLokasi(i);
        if (!loc.isPabrikUtama)
            cmbAsal->addItem(QString("[%1] %2").arg(i).arg(loc.namaLokasi),
                             QVariant(i));
    }
}

void PagePeta::refresh() {
    GrafJaringan& peta = ctrl->getPeta();
    int n = peta.getJumlahNode();

    int totalEdge = 0;
    for (int i = 0; i < n; i++) {
        EdgeNode* e = peta.getAdjList(i);
        while (e) { totalEdge++; e = e->next; }
    }
    totalEdge /= 2;

    lblLegend->setText(
        QString("%1 node (lokasi) · %2 edge (ruas jalan)")
            .arg(n).arg(totalEdge));

    populateCombo();
    tblJarak->setRowCount(0);
    canvas->update();
}

void PagePeta::onHitungRute() {
    if (cmbAsal->count() == 0) return;

    int nodeAsal = cmbAsal->currentData().toInt();
    GrafJaringan& peta = ctrl->getPeta();
    int pabrik = peta.cariPabrikUtama();

    if (pabrik < 0) {
        lblHasil->setText("Tidak ada node pabrik utama dalam graf.");
        return;
    }

    HasilDijkstra* hasil = peta.dijkstra(nodeAsal);

    delete[] jalurTerakhir;
    jalurTerakhir  = peta.rekonstruksiJalur(hasil, pabrik, panjangTerakhir);
    canvas->setJalur(jalurTerakhir, panjangTerakhir);

    double jarakTotal = hasil[pabrik].jarak;
    if (jarakTotal >= INF_DIST) {
        lblHasil->setText("Tidak ada jalur dari pos ini ke pabrik.");
        delete[] hasil;
        return;
    }

    // Jalur teks
    QString jalurStr;
    for (int i = 0; i < panjangTerakhir; i++) {
        if (i > 0) jalurStr += "  →  ";
        jalurStr += peta.getLokasi(jalurTerakhir[i]).namaLokasi;
    }

    lblHasil->setText(
        QString("Rute terpendek ditemukan!\n"
                "Dari:  %1\n"
                "Ke:    %2\n"
                "Jarak: %3 km\n"
                "Jumlah node: %4 titik")
            .arg(peta.getLokasi(nodeAsal).namaLokasi)
            .arg(peta.getLokasi(pabrik).namaLokasi)
            .arg(jarakTotal, 0, 'f', 2)
            .arg(panjangTerakhir));

    txtJalur->setPlainText("JALUR:\n" + jalurStr);

    int n = peta.getJumlahNode();
    tblJarak->setRowCount(0);
    for (int i = 0; i < n; i++) {
        int row = tblJarak->rowCount();
        tblJarak->insertRow(row);

        tblJarak->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
        tblJarak->setItem(row, 1, new QTableWidgetItem(peta.getLokasi(i).namaLokasi));

        bool terjangkau = (hasil[i].jarak < INF_DIST);
        QTableWidgetItem* jItem;
        if (terjangkau) {
            jItem = new QTableWidgetItem(QString::number(hasil[i].jarak, 'f', 2));
            if (i == pabrik) jItem->setForeground(QColor(0xb58105));
            else if (i == nodeAsal) jItem->setForeground(QColor(0x1e7e34));
            else jItem->setForeground(QColor(0x2d3748));
        } else {
            jItem = new QTableWidgetItem("∞ (tidak terjangkau)");
            jItem->setForeground(QColor(0xdc3545));
        }
        tblJarak->setItem(row, 2, jItem);

        // Predecessor
        QString predStr = "—";
        if (hasil[i].predecessor >= 0)
            predStr = peta.getLokasi(hasil[i].predecessor).namaLokasi;
        else if (i == nodeAsal)
            predStr = "(titik asal)";
        tblJarak->setItem(row, 3, new QTableWidgetItem(predStr));

        // Sorot baris pabrik dan asal
        if (i == pabrik) {
            for (int c = 0; c < 4; c++)
                if (tblJarak->item(row,c))
                    tblJarak->item(row,c)->setBackground(QColor(0xfff3cd));
        }
        if (i == nodeAsal) {
            for (int c = 0; c < 4; c++)
                if (tblJarak->item(row,c))
                    tblJarak->item(row,c)->setBackground(QColor(0xd4edda));
        }
    }

    delete[] hasil;
}

void PagePeta::onClearJalur() {
    canvas->clearJalur();
    tblJarak->setRowCount(0);
    txtJalur->clear();
    lblHasil->setText("Pilih pos asal lalu klik Cari Rute.");
    delete[] jalurTerakhir;
    jalurTerakhir  = nullptr;
    panjangTerakhir = 0;
}
