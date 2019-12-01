from scipy import signal,fftpack
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import math

def concat_state_deprecated(sample):
    length=len(sample)
    loops=math.floor(sample.size/length)
    even_loops=math.ceil(loops/2)
    odd_loops=math.floor(loops/2) 
    sample_even=np.zeros((even_loops*length))
    sample_odd=np.zeros((odd_loops*length))
    for index in range(0,loops):
        i=math.floor(index/2)
        start=i*length
        end=start+length
        if((index%2)==0):
            sample_even[start:end]=sample[:,index]
        else:
            sample_odd[start:end]=sample[:,index]
    return sample_even, sample_odd

def resampler_deprecated(train,channel='EMG3',sr=10):
    emg=train.loc[:,channel].copy()
    res=[]
    res_ext=[]
    for index in range(0,math.ceil(len(emg.values)/sr)):
        res.append(emg.values[index*sr])
    for index in range(0,len(res)):
        res_ext.append(res[index])
        if(index!=len(res)-1):
            THIS=res[index]
            NEXT=res[index+1]
            INTERP=np.linspace(THIS,NEXT,sr).tolist()
            del(INTERP[-1])
            res_ext+=INTERP
        else:
            pass     
    for index in range(len(res_ext),len(emg.values.tolist())):
        res_ext.append(emg.values.tolist()[index])
    emg_res=pd.DataFrame(np.array(res_ext))    
    return emg, emg_res

def state_spliter_deprecated(train,skip=500,length=2500):
    channel=len(train.columns)
    loops=math.floor(((train.size/channel)-skip)/length)
    train_split=np.zeros((length,channel,loops))
    train.loc[:,'EMG3'].plot(figsize=(20,5)) 
    plt.show()
    for index in range(0,loops):
        start=skip+index*length
        end=start+length
        train.loc[start:end,'EMG3'].plot(figsize=(2,5))
        plt.show()
        train_split[:,:,index]=train[start:end].values
    return train_split

def get_envelop(emg):
    raw_sig=emg.values
    hx=fftpack.hilbert(raw_sig)
    env=np.sqrt(hx**2+raw_sig**2)
    emg_env=pd.DataFrame(env)
    return emg_env

def lowpass(input_df,cutoff=0.0055):
    input_array=input_df.values
    b,a=signal.butter(8,cutoff,'lowpass')
    output_array=signal.filtfilt(b,a,np.ravel(input_array))
    output_df=pd.DataFrame(output_array)
    return output_df

def highpass(input_df):
    input_array=input_df.values
    b,a=signal.butter(8,0.0054,'highpass')
    output_array=signal.filtfilt(b,a,np.ravel(input_array))
    output_df=pd.DataFrame(output_array)
    return output_df

def bandpass(input_df,upper=0.65,lower=0.64):
    input_array=input_df.values
    b,a=signal.butter(8,[lower,upper],'bandpass')
    output_array=signal.filtfilt(b,a,np.ravel(input_array))
    output_df=pd.DataFrame(output_array)
    return output_df

def integration(input_df):
    input_array=input_df.values
    output_array=np.zeros((len(input_array.tolist())))
    int_temp=0
    for index in range(0,len(input_array.tolist())):
        temp=abs(input_array[index])
        ave=np.mean(np.abs(input_array))
        temp=temp-ave
        int_temp+=temp
        output_array[index]=int_temp
    output_df=pd.DataFrame(output_array)
    return output_df