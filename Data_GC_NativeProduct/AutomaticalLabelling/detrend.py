import pandas as pd
import numpy as np

def list_detrend(input):
    output=[0]
    for index in range(1,len(input)):
        output.append(input[index]-input[index-1])
    return output

def channel_detrend(channel):
    output=channel.copy()
    output_List=output.values.tolist()
    output_List=list_detrend(output_List)
    output_Detrended=np.array(output_List)
    output=pd.DataFrame(output_Detrended)
    return output