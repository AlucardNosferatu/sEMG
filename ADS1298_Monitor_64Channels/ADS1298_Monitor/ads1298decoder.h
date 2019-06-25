#ifndef ADS1298DECODER_H        //conditional compilation command
#define ADS1298DECODER_H        //macro define command

#include <QObject>              //files include command
#include <QTcpServer>           //include the head files associated the TCP server
#include <QTcpSocket>           //
#include <QtSerialPort/QSerialPort>
#include <QList>
#include <QMainWindow>

typedef unsigned char byte;     // use typedef to redefine existing type. byte = unsignes char

#define ADS_NUM 2 //twoADS1298 in every board
#define CH_NUM (ADS_NUM*8)  //CH_NUM = 16


// use typedef to redefine the existing enum as ParseState
// The enum elements are const, and PS_IDLE =0,PS_HEADER =1,PS_CMD =2,PS_DATA =3,PS_CHECK =4


typedef  enum {PS_IDLE, PS_HEADER, PS_CMD, PS_DATA, PS_CHECK} ParseState;



class Ads1298Decoder: public QObject
{
    Q_OBJECT

public:
    Ads1298Decoder(quint16 port,int index=-1,QObject* parent=0); // the wifi mode
    Ads1298Decoder(const QString &com, qint32 baudRate,int index=-1,QObject* parent=0); // the com mode
    void disconnect();
    void sendCmd(char* cmd, int len);
private:
    int index; //
    int mode;  // 0-Wifi, 1-Com
    QTcpServer* server;
    QTcpSocket* socket;           //
    QSerialPort* serialPort;

    void parse(byte dat);
    ParseState parseState;
    byte parseCMD;                //command
    byte parseDataLen;            //Data length
    byte parseDataSumCheck;       //
    int parseDataIndex;           //
    QList<byte> dataBuffer;       //unsigned char list
    void decodingNewData();

private slots:
    void handleWifiNewConnection();
    void handleWifiReceiveNewData();
    void handleComReceiveNewData();
signals:
    void hasNewDataPacket(int index, double* newDP);// ADS1_CH1, ADS1_CH2...ADS1_CH8,ADS2_CH1...ADS2_CH8
    void hasNewCmdReply(int index, char cmdR);
    void hasNewWifiConnection(int index);

};

#endif // ADS1298DECODER_H
