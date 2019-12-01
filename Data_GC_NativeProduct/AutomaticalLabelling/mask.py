import detrend
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate
from scipy import fftpack

def freq_spec(input_df):
    # TODO(1641367382@qq.com):need to analyze fs of detrended.abs() and test the solution combining filter with hilbert transformation
    input_array=input_df.values
    output=fftpack.fft(input_array)
    output_magn=abs(output)
    output_norm=output_magn/(len(output.tolist())/2)
    output_norm=output_norm[range(int(len(output.tolist())/2))]
    output_df=pd.DataFrame(output_norm)
    return output_df


def interp(input_df,output_length):
    input_array=input_df.values[:,0]
    input_length=len(input_array.tolist())
    
    input_ls=np.linspace(0,input_length-1,input_length)
    output_ls=np.linspace(0,input_length-1,output_length)
    
    interp = interpolate.interp1d(input_ls, input_array,kind='linear',fill_value="extrapolate")
    output_array=interp(output_ls)
    
    return pd.DataFrame(output_array)

def window_detection(input_df,window_size=400,step=100):
    input_array=input_df.values
    total_length=len(input_array.tolist())#39968
    max_list=[]
    for index in range(0,total_length,step):
        if(index+window_size>=total_length):
            pass
        else:
            temp=input_array[index:index+window_size]
            max_list.append(np.max(temp))
    return pd.DataFrame(np.array(max_list))

def manual_mask(input_df,index):
    temp=input_df.loc[:,'EMG'+str(index)].abs()
    mask=window_detection(temp,window_size=500,step=20)
    mask=interp(mask,len(temp.tolist()))
    mask.plot(figsize=(20,5))
    return mask

def average_mask(input_df,cc,cl):    
    for index in range(0,cc):
        if(index==0):
            temp=input_df.loc[:,'EMG'+str(index)].abs()
        else:
            temp+=input_df.loc[:,'EMG'+str(index)].abs()
    temp/=cc
    mask=window_detection(temp,window_size=500,step=20)
    mask=interp(mask,cl)
    mask.plot(figsize=(20,5))
    return mask

def smart_mask(input_df):
    length_collection=[]
    low_collection=[]
    sum_collection=[]
    for index in range(0,16):
        EMG_channel = input_df.loc[:,'EMG'+str(index)]
        EMG_channel = detrend.channel_detrend(EMG_channel)
        EMG_channel.plot(figsize=(20,5))
        print(str(index)+"_raw")
        plt.show()
        
        EMG_channel = freq_spec(EMG_channel)
        EMG_channel = freq_spec(EMG_channel)
        EMG_channel = freq_spec(EMG_channel)
        
        EMG_channel.plot(figsize=(20,5))
        print(str(index)+"_fft")
        plt.show()
        
        m = window_detection(EMG_channel,window_size=200,step=10)
        m = interp(m,EMG_channel.size)
        m.plot(figsize=(20,5))
        print(str(index)+"_mask")
        plt.show()
        #TODO
        threshold = (3*(m.max()-m.min())/5)+m.min()
        threshold = threshold.values[0]
        #TODO
        lower = ((m.max()-m.min())/5)+m.min()
        lower = lower.values[0]
        maxlength = 0
        templength = 0
        templow = 0
        maxlow = 0
        for time_index in range(0,EMG_channel.size):
            if time_index==(EMG_channel.size-1):
                if templength > maxlength:
                    maxlength=templength
                else:
                    pass
                if templow > maxlow:
                    maxlow=templow
                else:
                    pass
                templow=0
                templength=0
            else:
                if m.loc[time_index,:].values[0] >= threshold:
                    templength+=1
                else:
                    if templength > maxlength:
                        maxlength=templength
                    else:
                        pass
                    templength=0
                if m.loc[time_index,:].values[0] <= lower:
                    templow+=1
                else:
                    if templow > maxlow:
                        maxlow=templow
                    else:
                        pass
                    templow=0
        sum_collection.append(maxlength+maxlow)                    
        low_collection.append(maxlow)
        length_collection.append(maxlength)

    print("fuck")