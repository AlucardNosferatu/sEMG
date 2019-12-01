import pandas as pd

def read_and_drop(filepath,drop):
    train=pd.read_csv(filepath)
    if(drop):
        for each in drop:
            train=train.drop([each],axis=1)
    return train

def load_file(filepath,sel_lb,sel_ub):
    replace_tab(filepath)
    #only select channels sel_lb<x<sel_ub
    drop=['Unnamed: 16']
    for index in range(0,sel_lb+1):
        drop.append('EMG'+str(index))
    for index in range(sel_ub,16):
        drop.append('EMG'+str(index))        
    train=read_and_drop(filepath=filepath,drop=drop)
    return train,train.abs()

def replace_tab(filepath):
    f=open(filepath,"r+")
    new_content=[]
    for line in f:
        line=line.replace('\t',',')
        new_content.append(line)
    f.seek(0)
    f.writelines(new_content)
    f.close()