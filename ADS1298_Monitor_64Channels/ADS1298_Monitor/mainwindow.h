#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>  //This is the head files
#include "ads1298decoder.h"
#include "qcustomplot.h"
#include <QLineEdit>
#include <QVector>
#include <QTimer>
#include <QQueue>

#include "iirfilter.h"
#include "dialogs/fftdialog.h"
#include "qt-cnn.hpp"
#include "sEMG.hpp"
#include "Matrix.hpp"
#include "Tensor.hpp"
#include "Filter.hpp"
#include "utils.hpp"


#include "tinyxml/tinyxml.h"



#define TIME_SPAN 5
#define TIME_BORDER 0
#define SAMPLE_Freq 250
#define PlotNum SAMPLE_Freq*TIME_SPAN

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    double **p; //semg data
    int count;  //
    int row, col;
    Filter conv1Filter;
    Filter conv2Filter;
    Filter conv3Filter;
    vector<double> convbias1;
    vector<double> convbias2;
    vector<double> convbias3;
    Matrix fc1weight;
    Matrix fc2weight;
    vector<double> fullbias1;
    vector<double> fullbias2;
    vector<double> bn1_weight;
    vector<double> bn1_bias;
    vector<double> bn1_running_mean;
    vector<double> bn1_running_var;



private slots:
    void on_pushButton_workBench_clicked();
    void on_pushButton_record_clicked();
    void handleRecordNameChanged();

private:
    Ui::MainWindow *ui;
    QList<Ads1298Decoder*> module; //the Ads1298Decoder is a QObject,store the QObject and its subclass's pointer
    QList<QList<double>*> rawData;
    QList<QList<double>*> filterData;
    QList<QList<double>*> detrendedData;
    QList<IIRFilter*> notchfilters_50;
    QList<IIRFilter*> notchfilters_100;
    QList<IIRFilter*> hpfilters;
    void refreshChannelLabels();
    void refreshIIRFilters();
    void refreshDataBuffer();
    void updatePlotData(int index);
    double getPlotMax(QQueue<double> &plotData);
    double getPlotMin(QQueue<double> &plotData);
    void predict();
    string getMotionStr(int num);

private slots:
    void handleHasNewDataPacket(int index, double* newDP);
    void handleHasNewCmdReply(char cmdR);
    void handleHasNewWifiConnection(int index);


private slots:
    void on_pushButton_open_clicked();
    void on_pushButton_fresh_clicked();
    void handleFreshPort();
    void on_pushButton_close_clicked();
    void on_pushButton_connectWifi_clicked();


private:
    void log(QString &info);
private slots:
    void on_pushButton_ClearPlot_clicked();
    void on_pushButton_clearLog_clicked();
    void handleReplotTimerTimeout();
    void on_pushButton_FFT_clicked();


private slots:
    void on_pushButton_squareWaveTest_clicked();
    void on_pushButton_noiseTest_clicked();
    void on_pushButton_reset_clicked();
    void on_pushButton_beginReadC_clicked();
    void on_pushButton_stopReadC_clicked();
    void on_pushButton_normalTest_clicked();

    void on_pushButton_record1_clicked();

private:
    void setCustomPlotPattern();
    void setCustomPlotData(double, double*);
    QTimer replotTimer;
    int timeCounter;
    int plotCounter;
    
    bool isFileNameValid();  // if valid, the value is true

    bool isRecording;     // the state whether recornding the data or not, When record the data,the value is true
    QList<QFile*> rFile_raw;  //Store the pointers as variables...write raw data
    QList<QFile*> rFile;      //us the pointers as variables,,,write filtered data
    QList<QFile*> rFile_det;
    QList<QTextStream*> rOut_raw, rOut, rOut_det;

    QPixmap ledIcon;  //  The Led glitter when recording the data

    //QQueue<double> rawEMGData[CH_NUM];
    QQueue<double> rawPlotData;     //queeu, the FIFO structure,first-in-first-out

    //QQueue<double> filterEMGData[CH_NUM];
    QQueue<double> filterPlotData;

    
    FFTDialog *fftDialog;

    QCustomPlot* plots[64];
    QCustomPlot* channel[64];
};

#endif // MAINWINDOW_H
