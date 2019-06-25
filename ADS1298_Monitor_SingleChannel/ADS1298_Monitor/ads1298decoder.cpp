#include "ads1298decoder.h"

Ads1298Decoder::Ads1298Decoder(quint16 port,int index, QObject *parent):   // the wifi mode
    index(index),QObject(parent)
{
    mode = 0;
    parseState = PS_IDLE;   //parse the wifi state, PS_IDLE--the idle mode
    socket = 0;
    server = new QTcpServer(this);  //create the TCP server
    server->listen(QHostAddress::Any,port);   //listen the port number
    connect(server,SIGNAL(newConnection()),this,SLOT(handleWifiNewConnection()));   //
}

Ads1298Decoder::Ads1298Decoder(const QString &com, qint32 baudRate, int index, QObject *parent): //the com mode
    index(index),QObject(parent)
{
    mode = 1;
    parseState = PS_IDLE;
    serialPort = new QSerialPort(this);
    serialPort->setBaudRate(baudRate);
    serialPort->setPortName(com);
    serialPort->open(QIODevice::ReadWrite);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(handleComReceiveNewData()));
}

void Ads1298Decoder::disconnect()
{
    if(mode)//Com
    {
        serialPort->close();
    }
    else // Wifi
    {
        server->close();
    }
}

void Ads1298Decoder::sendCmd(char *cmd, int len)
{
    if(mode)
        serialPort->write(cmd,len);
    else
        socket->write(cmd,len);               //write command to TCP server
}

void Ads1298Decoder::parse(byte dat)           //
{
    //qDebug()<<QString::number(dat,16);
    switch(parseState)
    {
    case PS_IDLE:                               //PS- the parseState
        if(dat==0xff)
        {
            parseState = PS_HEADER;             //
            return;
        }
        break;
    case PS_HEADER:
        if(dat==0xff)
        {
            parseState = PS_CMD;
            return;
        }
        break;
    case PS_CMD:
        parseCMD = dat;                         //save the CMD value in parseCMD
        parseState = PS_DATA;
        parseDataSumCheck = 0;
        parseDataIndex = 0;
        dataBuffer.clear();                     //clear the list, waite to receive the new data.
        switch(parseCMD)
        {
        case 0x01:                              // the data
            parseDataLen = ADS_NUM*27;
            return;
        case 0x03:                              // the command
            parseDataLen = 1;
            return;
        }
        break;
    case PS_DATA:
        dataBuffer.append(dat);
        parseDataSumCheck += dat;
        parseDataIndex++;                       //record the length of received data
        if(parseDataIndex==parseDataLen)        // if command the parseDataIndex=1,then go to check, if data the parseDataIndex = 27
            parseState = PS_CHECK;
        return;
    case PS_CHECK:                              // check the sum of all data
        if(parseDataSumCheck==dat)              // the sumcheck success, according the data or command to decoding data or implement the command oparation
        {
            switch(parseCMD)
            {
            case 0x01:                          // the data
                decodingNewData();              // decoding data, call the function
                break;
            case 0x03:                          // the command
                emit hasNewCmdReply(index,dataBuffer[0]);  // complement the command ,emit the signal
                break;
            }
        }
        break;
    }

    parseState = PS_IDLE;
}

void Ads1298Decoder::decodingNewData()
{
    int dataIndex = 0;
    double channelVol[CH_NUM];
    int chIndex = 0;
    for(int i=0; i<ADS_NUM; i++)
    {
        dataIndex += 3;

        unsigned char data[3];
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<3;j++)
            {
                data[j] = dataBuffer[dataIndex++];
            }

            int val;
            if( (data[0]&0x80)!=0x80 )
//                val = (unsigned int(data[0])<<16) | (unsigned int(data[1])<<8) | (unsigned int(data[2]));
                val = ((unsigned int)(data[0]<<16)) | ((unsigned int)(data[1]<<8)) | ((unsigned int)data[2]);
                else
//                val = 0xff000000 | (unsigned int(data[0])<<16) | (unsigned int(data[1])<<8) | (unsigned int(data[2]));
                val = 0xff000000 | ((unsigned int)(data[0]<<16)) | ((unsigned int)(data[1]<<8)) | ((unsigned int)data[2]);

            channelVol[chIndex++] = double(val) / 0x7FFFFE * 3.3;
        }
    }
    emit hasNewDataPacket(index,channelVol);  //pass on module index and channelVol, emit signal to active the slot:handleHasNewDataPacket(int,double*)
}

void Ads1298Decoder::handleWifiNewConnection()
{
    socket = server->nextPendingConnection();  // the socket of the connected TCP server
    connect(socket,SIGNAL(readyRead()),this,SLOT(handleWifiReceiveNewData()));
    emit hasNewWifiConnection(index);
}

void Ads1298Decoder::handleWifiReceiveNewData()
{
    QByteArray r = socket->readAll();   //define r in the type of QByteArray, and save all the read data to it.
    for(int i=0; i<r.size(); i++)
        parse(r.at(i));
}

void Ads1298Decoder::handleComReceiveNewData()
{
    QByteArray r = serialPort->readAll();
    for(int i=0; i<r.size(); i++)
        parse(r.at(i));
}
