#include "fftdialog.h"
#include "ui_fftdialog.h"

#include "fftw3/fftw3.h"

QPen penList[] = {QPen(Qt::red),QPen(Qt::green),QPen(Qt::blue),QPen(Qt::yellow),QPen(Qt::black)};
int penNum = 5;

FFTDialog::FFTDialog(QWidget *parent, double f) :
    QDialog(parent),
    ui(new Ui::FFTDialog),
    freq(f)
{
    ui->setupUi(this);
    graphIndex = 0;
}

FFTDialog::~FFTDialog()
{
    delete ui;
}

void FFTDialog::addFFTPlot(QList<double> *x)
{
    int num = x->size();


    fftw_complex *in1_c, *out1_c;
    fftw_plan p;
    in1_c = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*num);
    out1_c = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*num);
    p = fftw_plan_dft_1d(num, in1_c, out1_c, FFTW_FORWARD, FFTW_ESTIMATE);

    for (int i=0; i<num; i++)
    {
        in1_c[i][0] = x->at(i);
        in1_c[i][1] = 0;
    }
    fftw_execute(p);
    fftw_destroy_plan(p);


    QList<double> amp;
    double a = sqrt(out1_c[0][0]*out1_c[0][0]+out1_c[0][1]*out1_c[0][1]);
    amp.append(a/num*1000);
    int n = num/2;
    for(int i=1; i<n; i++)
    {
        double a1 = sqrt(out1_c[i][0]*out1_c[i][0]+out1_c[i][1]*out1_c[i][1]);
        double a2 = sqrt(out1_c[num-i][0]*out1_c[num-i][0]+out1_c[num-i][1]*out1_c[num-i][1]);
        amp.append((a1+a2)/2/num*1000);
    }
    fftw_free(in1_c); fftw_free(out1_c);


    ui->customPlot->addGraph();
    double df = freq / num;
    for(int i=0; i<amp.size(); i++)
    {
        ui->customPlot->graph()->addData(df*i,amp.at(i));
        if(yMax<amp.at(i))
            yMax = amp.at(i);
    }

    ui->customPlot->graph()->setPen(penList[graphIndex%penNum]);
    ui->customPlot->graph()->setName(QString("Plot %1").arg(++graphIndex));
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->xAxis->setRange(0,amp.size()*df);
    ui->customPlot->yAxis->setRange(0,yMax);
    ui->customPlot->xAxis->setLabel("freq(Hz)");
    ui->customPlot->yAxis->setLabel("Voltage(mV)");
    ui->customPlot->replot();
}

void FFTDialog::on_pushButton_clicked()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
    yMax = 0;
    graphIndex = 0;
}
