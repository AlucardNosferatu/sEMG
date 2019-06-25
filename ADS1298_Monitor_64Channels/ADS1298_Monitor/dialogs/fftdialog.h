#ifndef FFTDIALOG_H
#define FFTDIALOG_H

#include <QDialog>

namespace Ui {
class FFTDialog;
}

class FFTDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FFTDialog(QWidget *parent,double freq);
    ~FFTDialog();

    void addFFTPlot(QList<double> *x);

private slots:
    void on_pushButton_clicked();

private:
    Ui::FFTDialog *ui;
    double yMax;
    double freq;
    int graphIndex;
};

#endif // FFTDIALOG_H
