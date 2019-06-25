#include "iirfilter.h"

IIRFilter::IIRFilter()
{


}

int IIRFilter::initFilter(float atmp[], float btmp[], int nal, int nbl)
{

    al=nal;
    bl=nbl;

    a.clear();
    b.clear();
    xtmp.clear();
    ytmp.clear();

    for (int i=0;i<al;i++)
        a.append(atmp[i]);

    for (int i=0;i<bl;i++)
        b.append(btmp[i]);
    return 0;
}

float IIRFilter::filter(float data)
{
    ytmp.push_front(0);

    xtmp.insert(0,data);
    if (xtmp.length()>bl)
        xtmp.pop_back();
    for (int i=0;i<bl;i++)
    {
        if (i==xtmp.length())
            break;
        ytmp[0]+=xtmp[i]*b[i];
    }

    if (ytmp.length()>al)
        ytmp.pop_back();
    for (int i=1;i<al;i++)
    {
        if (i==ytmp.length())
            break;
        ytmp[0]-=ytmp[i]*a[i];
    }
    ytmp[0]/=a[0];
    return ytmp[0];
}

int IIRFilter::ResetFilter()
{
    xtmp.clear();
    ytmp.clear();
    return 0;
}

