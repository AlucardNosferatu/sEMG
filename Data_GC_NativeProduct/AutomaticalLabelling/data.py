import pandas as pd
import numpy as np
import math
import os
import matplotlib.pyplot as plt

import mask
import fileparser
import detrend

#ATTENTION: All function returning data must return data in form of DataFrame


def state_spliter(signal_df,mask_df,upper,lower):
    signal_array=signal_df.values
    mask_array=mask_df.values
    
    high_state=[]
    low_state=[]
    
    length=len(signal_array.tolist())
    for index in range(0,length):
        if(mask_array[index]>=upper):
            high_state.append(signal_array[index])
        elif(mask_array[index]<=lower):
            low_state.append(signal_array[index])
        else:
            pass
        #ignore intermediate state (mask between upper and lower threshold)
    high_df=pd.DataFrame(np.array(high_state))
    low_df=pd.DataFrame(np.array(low_state))
    return high_df,low_df

def batch_spliter_channel(input_df,window_size=200):
    # TODO(1641367382@qq.com):negative-index need to be fixed
    #split a sequence of same label into batch-size sequences
    total_length=np.size(input_df)
    step=int(window_size/2)
    temp_array=np.zeros((window_size,math.floor(total_length/window_size)*2-1))
    #temp_array[:,index] correspond to each segments
    #AAAABBBBCCCCD|=3=floor(13/4)
    #  AAAABBBB   |=2=floor(13/4)-1
    for index in range(0,total_length,step):
        if(index+window_size>=total_length or int(index/step)>=temp_array.shape[1]):
            pass
        else:
            temp_array[:,int(index/step)]=input_df[index:index+window_size].values[:,0]
    temp_array=np.array(temp_array,ndmin=3)
    return temp_array
    #adjust output to array for 3-dimensional processing
    
def load_data(filepath,mask_channel,sel_lb,sel_ub,mask_lb,mask_ub):
    start=sel_lb+1
    train,train_abs=fileparser.load_file(filepath,sel_lb,sel_ub)
    train=train_abs
    channel_length=len(train.index)
    channel_counts=int(np.size(train)/channel_length)
    m=mask.manual_mask(train,mask_channel)
    high_state=np.zeros((1,2))
    low_state=np.zeros((1,2))

    for index in range(start,start+channel_counts):
        temp=train.loc[:,'EMG'+str(index)].values
        high_temp,low_temp=state_spliter(pd.DataFrame(temp.abs()),m,upper=mask_ub,lower=mask_lb)
        if(index==start):
            high_state=high_temp
            low_state=low_temp
        else:
            high_state=np.concatenate((high_state,high_temp),axis=1)
            low_state=np.concatenate((low_state,low_temp),axis=1)
    return high_state,low_state,train

def show_all(filepath):
    pathExists=os.path.exists(filepath.replace('.csv',''))
    if(pathExists):
        pass
    else:
        os.mkdir(filepath.replace('.csv',''))
    fileparser.replace_tab(filepath)
    train=pd.read_csv(filepath)
    for index in range(0,16):
        EMG_channel = train.loc[:,'EMG'+str(index)]
        EMG_channel = detrend.channel_detrend(EMG_channel)
#        EMG_channel=glitch_filter(EMG_channel)
        train.loc[:,'EMG'+str(index)]=EMG_channel+index*0.01
    train.plot(figsize=(20,30))
    plt.show()
#    mask.smart_mask(train)
    
def glitch_filter(channel):
    data_array=channel.values
    data_list=data_array.tolist()
    for index in range(0,len(data_list)):
        if(data_list[index][0]>0.004 or data_list[index][0]<-0.004):
            data_list[index][0]=data_list[index-1][0]
    data_array=np.array(data_list)
    return pd.DataFrame(data_array)
    
    
    
sh='lx'
show_all('10_3.csv')
#hs_sh,ls_sh,all_sh=load_data('10_0.csv',mask_channel=12,sel_lb=7,sel_ub=15,mask_lb=0.00007,mask_ub=0.00007)
#np.save("Processed\\all_"+sh+".npy",all_sh)