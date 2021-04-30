#include "PdfView.h"
#include "ui_PdfView.h"

PdfView::PdfView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PdfView)
{
    ui->setupUi(this);
}

PdfView::~PdfView()
{
    delete ui;
}
