#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>     //include the header files associated the serial port
#include <QDebug>

static float ahp[1]={
    1.0f
};
static float bhp[5]={
    static_cast<float>(0.0627402311),
    static_cast<float>(-0.2499714735),
    static_cast<float>(0.3744644353),
    static_cast<float>(-0.24997147355),
    static_cast<float>(0.062740231119)
};
static float anotch[7]={
    1.000000000000000,
    static_cast<float>(-1.699163423921474),
    static_cast<float>(3.464263380095651),
    static_cast<float>(-3.035006841250400),
    static_cast<float>(2.930889612822229),
    static_cast<float>(-1.213689963509197),
    static_cast<float>(0.604109699507278)
};
static float bnotch[7]={
    static_cast<float>(0.777337677403281),
    static_cast<float>(-1.441206975301750),
    static_cast<float>(3.222510786578553),
    static_cast<float>(-3.065671614896859),
    static_cast<float>(3.222258852356618),
    static_cast<float>(-1.440981638482467),
    static_cast<float>(0.777155376086710)
};
static float anotch1[3]={
    static_cast<float>(0.990498466402),
    static_cast<float>(-0.6121617180411),
    static_cast<float>(0.990498466402)
};
static float bnotch1[3]={
    1,
    static_cast<float>(-0.6121617180411),
    static_cast<float>(0.980996932804)
};



MainWindow::MainWindow(QWidget *parent):         // the Constuctor
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    handleFreshPort();

    fftDialog = new FFTDialog(this,SAMPLE_Freq);

    connect(&replotTimer,SIGNAL(timeout()),this,SLOT(handleReplotTimerTimeout()));

    ledIcon = QPixmap(":/LED.png");
    ledIcon = ledIcon.scaled(ui->label_Icon->size(),Qt::KeepAspectRatio);
    connect(ui->lineEdit_Dir,SIGNAL(textChanged(QString)),this,SLOT(handleRecordNameChanged()));
    connect(ui->lineEdit_fileName,SIGNAL(textChanged(QString)),this,SLOT(handleRecordNameChanged()));
    connect(ui->spinBox_fileName,SIGNAL(valueChanged(QString)),this,SLOT(handleRecordNameChanged()));
    connect(ui->checkBox_overwrite,SIGNAL(stateChanged(int)),this,SLOT(handleRecordNameChanged()));
    handleRecordNameChanged();
    isRecording = false;
}

MainWindow::~MainWindow()                        // the Destructor
{
    int chNum = module.length()*CH_NUM;
    for(int i=0;i<chNum;i++)
    {
        delete notchfilters_50[i];
        delete notchfilters_100[i];
        delete hpfilters[i];
        delete rawData[i];
        delete filterData[i];
    }
    
    delete ui;
}


void MainWindow::on_pushButton_open_clicked()
{
    int baudRate = ui->comboBox_baudRate->currentText().toInt();
    QString com = ui->portBox->currentText();
    Ads1298Decoder* ads = new Ads1298Decoder(com,baudRate,0,this);
    module.append(ads);
    //connect(发送对象,发送对象的信号,接收对象,接收对象的槽函数)
    connect(ads,SIGNAL(hasNewDataPacket(int,double*)),this,SLOT(handleHasNewDataPacket(int,double*)));
    connect(ads,SIGNAL(hasNewCmdReply(int,char)),this,SLOT(handleHasNewCmdReply(int,char)));
    setCustomPlotPattern();
    refreshChannelLabels();
    refreshIIRFilters();
    refreshDataBuffer();
    plotCounter = timeCounter = 0;      //initialize plotCounter and timeCounter to zero
    replotTimer.start(100);             //10fps,replot the graph per 0.1s
    ui->statusBar->showMessage(tr("Open successed"));
}

void MainWindow::on_pushButton_connectWifi_clicked()
{
    QString s = ui->lineEdit_port->text();
    QStringList ports = s.split(";");
    for(int i=0;i<ports.length();i++)
    {
        Ads1298Decoder* ads = new Ads1298Decoder(static_cast<quint16>(ports[i].toUInt()),i,this);//create a pointer pointing to the class Ads1298Decoder
        module.append(ads);
        connect(module[i],SIGNAL(hasNewDataPacket(int,double*)),this,SLOT(handleHasNewDataPacket(int,double*)));
        connect(module[i],SIGNAL(hasNewCmdReply(int,char)),this,SLOT(handleHasNewCmdReply(int,char)));
        connect(module[i],SIGNAL(hasNewWifiConnection(int)),this,SLOT(handleHasNewWifiConnection(int)));
    }
    setCustomPlotPattern();
    refreshChannelLabels();
    refreshIIRFilters();
    refreshDataBuffer();
    plotCounter = timeCounter = 0;    // initialize
    replotTimer.start(100); //10fps
    ui->statusBar->showMessage(tr("Server Open successed"));
}

void MainWindow::on_pushButton_close_clicked()
{
    for(int i=0;i<module.length();i++)
        module[i]->disconnect();
    ui->statusBar->showMessage(tr("Close successed"));
}

void MainWindow::handleReplotTimerTimeout()
{   
    ui->customPlot->replot();

    plots[0]=ui->customPlot0;
    plots[1]=ui->customPlot1;
    plots[2]=ui->customPlot2;
    plots[3]=ui->customPlot3;
    plots[4]=ui->customPlot4;
    plots[5]=ui->customPlot5;
    plots[6]=ui->customPlot6;
    plots[7]=ui->customPlot7;
    plots[8]=ui->customPlot8;
    plots[9]=ui->customPlot9;
    plots[10]=ui->customPlot10;
    plots[11]=ui->customPlot11;
    plots[12]=ui->customPlot12;
    plots[13]=ui->customPlot13;
    plots[14]=ui->customPlot14;
    plots[15]=ui->customPlot15;

    for (int i=0;i<16;i++)
    {
        plots[i]->replot();
    }
}

void MainWindow::handleFreshPort()
{
    QSerialPortInfo portinfo;
    QList<QSerialPortInfo> ports=portinfo.availablePorts();
    ui->portBox->clear();
    for (int i=0;i<ports.length();i++)
    {
        ui->portBox->addItem(ports.at(i).portName());
    }
}

void MainWindow::refreshChannelLabels()
{
    //Accordig to the number of modules, determine the name Channels
    int chNum = module.length()*CH_NUM;
    for(int i=1; i<=chNum; i++)
        ui->comboBox_channel->addItem(QString("Channel %1").arg(i));
    ui->comboBox_channel->setCurrentIndex(0);  //set the Items[0] as Index
}

void MainWindow::refreshIIRFilters()
{
    int chNum = module.length()*CH_NUM;
    //Every module has 16 channels
    //There are 4 modules in typical instance of experiment
    //There are 4*16=64 channels in total
    //According to the number of modules
    for(int i=0;i<chNum;i++)//for every channel (64)
    {
        IIRFilter* nf_50 = new IIRFilter();
        IIRFilter* nf_100 = new IIRFilter();
        IIRFilter* hp = new IIRFilter();
        
        hp->initFilter(ahp,bhp,1,5);
        nf_50->initFilter(anotch,bnotch,7,7);
        nf_100->initFilter(anotch1,bnotch1,3,3);
        
        notchfilters_50.append(nf_50);
        notchfilters_100.append(nf_100);
        hpfilters.append(hp);
    }
}

void MainWindow::refreshDataBuffer()
{
    int chNum = module.length()*CH_NUM;

   //Accroding to chNum to assign the buffer size
    for(int i=0;i<chNum;i++)
    {
        QList<double>* rd = new QList<double>();
        QList<double>* fd = new QList<double>();
        rawData.append(rd);
        filterData.append(fd);
    }
}

void MainWindow::updatePlotData(int chIndex)
{
    double t = double(plotCounter++)/SAMPLE_Freq;

    // plot rawData
    double rData = rawData[chIndex]->back();
    rawPlotData.push_back(rData);
    if(rawPlotData.size()>PlotNum)
    rawPlotData.pop_front();

    double rMax = getPlotMax(rawPlotData);
    double rMin = getPlotMin(rawPlotData);
    double rRange = rMax - rMin;
    ui->customPlot->graph(0)->addData(t, rData);
    if(t<TIME_SPAN)
        ui->customPlot->xAxis->setRange(0,TIME_SPAN+TIME_BORDER);
    else
    {
        ui->customPlot->graph(0)->removeDataBefore(t-TIME_SPAN);
        ui->customPlot->xAxis->setRange(t-TIME_SPAN,t+TIME_BORDER);
    }
    ui->customPlot->yAxis->setRange(rMin-0.2*rRange,rMax+0.2*rRange);
    ui->lineEdit_channel->setText(QString("%1").arg(rData,0,'g',2));


    //Plot fData
    plots[0]=ui->customPlot0;
    plots[1]=ui->customPlot1;
    plots[2]=ui->customPlot2;
    plots[3]=ui->customPlot3;
    plots[4]=ui->customPlot4;
    plots[5]=ui->customPlot5;
    plots[6]=ui->customPlot6;
    plots[7]=ui->customPlot7;
    plots[8]=ui->customPlot8;
    plots[9]=ui->customPlot9;
    plots[10]=ui->customPlot10;
    plots[11]=ui->customPlot11;
    plots[12]=ui->customPlot12;
    plots[13]=ui->customPlot13;
    plots[14]=ui->customPlot14;
    plots[15]=ui->customPlot15;
    int moIndex = int(chIndex/CH_NUM);
    for(int i= moIndex*CH_NUM; i < (moIndex+1)*CH_NUM; i++)
    {
        double fData = filterData[i]->back();
        filterPlotData.push_back(fData);
        if(filterPlotData.size()>PlotNum)    //the Plot number is 250*5.
            filterPlotData.pop_front();

        double fMax = getPlotMax(filterPlotData);
        double fMin = getPlotMin(filterPlotData);
        double fRange = fMax - fMin;
        int j = int(i % CH_NUM);
        plots[j]->graph(0)->addData(t, fData);
        if(t<TIME_SPAN)
            plots[j]->xAxis->setRange(0,TIME_SPAN+TIME_BORDER);
        else
        {
            plots[j]->graph(0)->removeDataBefore(t-TIME_SPAN);
            plots[j]->xAxis->setRange(t-TIME_SPAN,t+TIME_BORDER);
        }
        plots[j]->yAxis->setRange(fMin-0.2*fRange,fMax+0.2*fRange);
//      channel[i]->setText(QString("%1").arg(fData,0,'g',2));
    }

}

double MainWindow::getPlotMax(QQueue<double> &plotData)
{
    double max = plotData.first();
    for (QQueue<double>::iterator i = plotData.begin(); i != plotData.end(); ++i)
        if(*i>max)
            max = *i;
    return max;
}

double MainWindow::getPlotMin(QQueue<double> &plotData)
{
    double min = plotData.first();
    for (QQueue<double>::iterator i = plotData.begin(); i != plotData.end(); ++i)
        if(*i<min)
            min = *i;
    return min;
}

void MainWindow::handleHasNewDataPacket(int module_index, double *newDP)      // the slot signal
{
    double fdata[CH_NUM];
    for(int i=0; i<CH_NUM; i++)
    {
        // i+index*CH_NUM---the data index associated with the Module index
        float fd;
        fd = notchfilters_50[i+module_index*CH_NUM]->filter(static_cast<float>(newDP[i]));                  //50Hz_notchfilters
        fd = notchfilters_100[i+module_index*CH_NUM]->filter(fd);                                           //100Hz_notchfilter
        fd = hpfilters[i+module_index*CH_NUM]->filter(fd);                                                  //20Hz_HighPassfilter
        filterData[i+module_index*CH_NUM]->append(static_cast<const double>(fd));                           //filterData
        rawData[i+module_index*CH_NUM]->append(newDP[i]);                                                   //rawData
        fdata[i] = static_cast<double>(fd);
    }
    if(isRecording)                                        //isRecording is True, Then record the data
    {
        for(int i=0; i<CH_NUM; i++)
        {
            *rOut_raw[module_index]<<newDP[i]<<'\t';
        }
        *rOut_raw[module_index]<<'\n';                            // Module---index
        for(int i=0; i<CH_NUM; i++)
        {
            *rOut[module_index]<<fdata[i]<<'\t';
        }
        *rOut[module_index]<<'\n';                                // Module---index
    }

    int chIndex = ui->comboBox_channel->currentIndex();
    if(int(chIndex/CH_NUM)==module_index)
    {
        updatePlotData(chIndex);
    }

}

void MainWindow::handleHasNewCmdReply(char cmdR)
{
    QString cmdR_str=QString::number(cmdR,16);
    log(cmdR_str);
    //返回命令根据16进制转化为字符串
}


void MainWindow::handleHasNewWifiConnection(int index)
{
    QString successInfo=QString("Module %1 connected!").arg(index);
    log(successInfo);
}

void MainWindow::setCustomPlotPattern()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->legend->setFont(QFont("Helvetica",9));
    ui->customPlot->legend->setMaximumSize(20,20);
    ui->customPlot->yAxis->setRange(-0.04,0);
    ui->customPlot->yAxis->setAutoTicks(true);
    ui->customPlot->yAxis->setLabel("Voltage(V)");
    ui->customPlot->xAxis->setRange(0,TIME_SPAN+TIME_BORDER);
    ui->customPlot->xAxis->setLabel("time(s)");
    ui->customPlot->replot();


    plots[0]=ui->customPlot0;
    plots[1]=ui->customPlot1;
    plots[2]=ui->customPlot2;
    plots[3]=ui->customPlot3;
    plots[4]=ui->customPlot4;
    plots[5]=ui->customPlot5;
    plots[6]=ui->customPlot6;
    plots[7]=ui->customPlot7;
    plots[8]=ui->customPlot8;
    plots[9]=ui->customPlot9;
    plots[10]=ui->customPlot10;
    plots[11]=ui->customPlot11;
    plots[12]=ui->customPlot12;
    plots[13]=ui->customPlot13;
    plots[14]=ui->customPlot14;
    plots[15]=ui->customPlot15;

    for (int i=0;i<16;i++)
    {
        plots[i]->clearGraphs();
        plots[i]->addGraph();
        plots[i]->graph(0)->setPen(QPen(Qt::red));
        plots[i]->graph(0)->setLineStyle(QCPGraph::lsLine);
        plots[i]->legend->setFont(QFont("Helvetica",9));
        plots[i]->legend->setMaximumSize(20,20);
        plots[i]->yAxis->setRange(-0.04,0);
        plots[i]->yAxis->setAutoTicks(true);
        plots[i]->yAxis->setLabel("Voltage(V)");
        plots[i]->xAxis->setRange(0,TIME_SPAN+TIME_BORDER);
        plots[i]->xAxis->setLabel("time(s)");
        plots[i]->replot();
    }
}


void MainWindow::log(QString &info)
{
    ui->textBrowser_log->append(info);
}

bool MainWindow::isFileNameValid()      //defined the name of the file, used for record the emg data
{
    QString dir = ui->lineEdit_Dir->text();
    if(dir.isEmpty())
        return false;
    QString fileName = ui->lineEdit_fileName->text();
    if(fileName.isEmpty())
        return false;
    QString n = ui->spinBox_fileName->text();
    QString fn = QString("%1/%2%3.txt").arg(dir).arg(fileName).arg(n);
    QFile f(fn);
    bool isOverwrite = ui->checkBox_overwrite->isChecked();
    if((!isOverwrite)&&(f.exists()))
        return false;
    return true;                        // the defined file name is valid, reture true.
}

void MainWindow::on_pushButton_fresh_clicked()
{
    handleFreshPort();
}

void MainWindow::on_pushButton_squareWaveTest_clicked()
{
    char cmd[] = {0x02};
    for(int i=0;i<module.length();i++)
    {
    module[i]->sendCmd(cmd,1);
    }
}

void MainWindow::on_pushButton_noiseTest_clicked()
{
    char cmd[] = {0x03};
    for(int i=0;i<module.length();i++)
    {
    module[i]->sendCmd(cmd,1);
    }
}

void MainWindow::on_pushButton_reset_clicked()
{
    char cmd[] = {0x01};
    for(int i=0;i<module.length();i++)
    {
    module[i]->sendCmd(cmd,1);
    }
}

void MainWindow::on_pushButton_beginReadC_clicked()
{
    char cmd[] = {0x05};
    for(int i=0;i<module.length();i++)
    {
    module[i]->sendCmd(cmd,1);
    }
}

void MainWindow::on_pushButton_stopReadC_clicked()
{
    char cmd[] = {0x06};
    for(int i=0;i<module.length();i++)
    {
    module[i]->sendCmd(cmd,1);
    }
}

void MainWindow::on_pushButton_clearLog_clicked()
{
    ui->textBrowser_log->clear();
}

void MainWindow::on_pushButton_normalTest_clicked()
{
    char cmd[] = {0x04};
    for(int i=0;i<module.length();i++)
    {
    module[i]->sendCmd(cmd,1);
    }
}

void MainWindow::on_pushButton_ClearPlot_clicked()
{
    plots[0]=ui->customPlot0;
    plots[1]=ui->customPlot1;
    plots[2]=ui->customPlot2;
    plots[3]=ui->customPlot3;
    plots[4]=ui->customPlot4;
    plots[5]=ui->customPlot5;
    plots[6]=ui->customPlot6;
    plots[7]=ui->customPlot7;
    plots[8]=ui->customPlot8;
    plots[9]=ui->customPlot9;
    plots[10]=ui->customPlot10;
    plots[11]=ui->customPlot11;
    plots[12]=ui->customPlot12;
    plots[13]=ui->customPlot13;
    plots[14]=ui->customPlot14;
    plots[15]=ui->customPlot15;

    rawPlotData.clear();
    ui->customPlot->graph(0)->clearData();
    ui->customPlot->replot();

    filterPlotData.clear();
    for (int i=0;i<16;i++)
    {
        plots[i]->graph(0)->clearData();
        plots[i]->replot();
    }

}

void MainWindow::on_pushButton_FFT_clicked()
{
    fftDialog->addFFTPlot(&filterPlotData); //get the filter data from
    if(!fftDialog->isVisible())
        fftDialog->show();
}

void MainWindow::on_pushButton_workBench_clicked()
{
    QString Dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(Dir.isEmpty())
        return;
    ui->lineEdit_Dir->setText(Dir);
}

void MainWindow::on_pushButton_record_clicked()
{
    if(ui->pushButton_record->text()=="Record")             // when the button show "record" and putting down the button
    {
        if(isFileNameValid())                               // The defined record file name is valid
        {
            QString dir = ui->lineEdit_Dir->text();
            QString fileName = ui->lineEdit_fileName->text();
            QString n = ui->spinBox_fileName->text();
            rFile.clear();
            rFile_raw.clear();
            rOut.clear();
            rOut_raw.clear();
            int num = module.length();

            for(int i=0; i<num; i++)
            {
                QString fn = QString("%1/%2%3_%4.txt").arg(dir).arg(fileName).arg(n).arg(i);  // i symbles the module.[i]
                QFile * rf = new QFile(fn);
                rf->open(QIODevice::WriteOnly);

                QTextStream* ro = new QTextStream(rf);
                //*ro<<"Time\t";
                for(int i=0; i<CH_NUM; i++)
                    *ro<<"EMG"<<i<<'\t';
                *ro<<'\n';
                rFile.append(rf);
                rOut.append(ro);

                QString fn_raw = QString("%1/%2%3_%4_raw.txt").arg(dir).arg(fileName).arg(n).arg(i);
                QFile * rf_r = new QFile(fn_raw);
                rf_r->open(QIODevice::WriteOnly);

                QTextStream* ro_r = new QTextStream(rf_r);
                *ro_r<<"Time\t";
                for(int i=0; i<CH_NUM; i++)
                    *ro_r<<"EMG"<<i<<'\t';
                *ro_r<<'\n';
                rFile_raw.append(rf_r);
                rOut_raw.append(ro_r);
            }
            isRecording = true;
            ui->pushButton_record->setText("Stop");
        }
		else 
		{
                        //log(tr("File name invalid!"));
		}

    }
    else if(ui->pushButton_record->text()=="Stop")                           // when the button show "stop" and putting down the button
    {
        isRecording = false;
        ui->label_Icon->clear();
        for(int i=0; i<rFile.length(); i++)
        {
            rFile[i]->close();
            rFile_raw[i]->close();
        }
        ui->pushButton_record->setText("Record");
        int n = ui->spinBox_fileName->value();
        ui->spinBox_fileName->setValue(n+1);
    }

}

void MainWindow::handleRecordNameChanged()
{
    ui->pushButton_record->setEnabled(isFileNameValid());
}
