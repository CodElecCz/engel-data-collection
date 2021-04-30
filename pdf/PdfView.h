#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <QWidget>

namespace Ui {
class PdfView;
}

class PdfView : public QWidget
{
    Q_OBJECT

public:
    explicit PdfView(QWidget *parent = nullptr);
    ~PdfView();

private:
    Ui::PdfView *ui;
};

#endif // PDFVIEW_H
