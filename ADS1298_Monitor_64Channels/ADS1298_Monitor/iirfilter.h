#ifndef IIRFILTER_H
#define IIRFILTER_H

#include <QList>


class IIRFilter
{
public:
    IIRFilter();
    int initFilter(float atmp[],float btmp[],int nal,int nbl);
    float filter(float data);
    int ResetFilter();


private:
    QList<float> a,b;
    int al,bl;
    QList<float> xtmp,ytmp;
};

#endif // IIRFILTER_H
