# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
#这些import会大大影响运行速度
import os
import numpy as np
import pandas as pd
from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.model_selection import StratifiedShuffleSplit
from keras.models import Sequential,load_model
from keras.layers import Dense, Activation, Flatten, Convolution1D, Dropout
from keras.optimizers import SGD
from keras.utils import np_utils


train=pd.read_csv('Data_LeafClassification/train.csv')
test=pd.read_csv('Data_LeafClassification/test.csv')

def encode(train,test):
    label_encoder = LabelEncoder().fit(train.species)
    #对species进行了标准化
    labels = label_encoder.transform(train.species)
    #LabelEncoder函数的作用是将标签值统一转换成range(标签值个数-1)范围内
    classes = list(label_encoder.classes_)
    #classes_获得所有分类标签
    train = train.drop(['species','id'],axis=1)
    #去掉了species和id列
    test = test.drop('id',axis=1)
    #去掉了id列
    return train, labels, test, classes
    #train-全部的训练数据
    #labels-训练数据的标记
    #test-测试数据
    #classes-所有的分类

#对训练数据用StandardScaler标准化
train, labels, test, classes = encode(train, test)
scaler = StandardScaler().fit(train.values)
scaled_train = scaler.transform(train.values)
scaled_test = scaler.transform(test.values)

sss=StratifiedShuffleSplit(test_size=0.1,random_state=23)
#利用交叉验证的办法，把训练数据+标签分成验证集和训练集
for train_index, valid_index in sss.split(scaled_train,labels):
    #问题：为什么要用for循环来进行赋值？
    X_train, X_valid = scaled_train[train_index], scaled_train[valid_index]
    y_train, y_valid = labels[train_index], labels[valid_index]

X_test=scaled_test

nb_features=64
#问题：这个64是怎么来的？（需要参考数据集定义，若是sEMG信号该如何输入？）
#貌似这个64是用来定义一个分类有几种特征
nb_class=len(classes)

X_train_r = np.zeros((len(X_train),nb_features,3))
#建立了一个三特征、长度为训练集个数，宽度为特征数量的空矩阵
X_train_r[:,:,0]=X_train[:,:nb_features]#第一个特征
X_train_r[:,:,1]=X_train[:,nb_features:nb_features*2]#第二个特征
X_train_r[:,:,2]=X_train[:,nb_features*2:]#第三个特征

X_valid_r = np.zeros((len(X_valid), nb_features, 3))
X_valid_r[:, :, 0] = X_valid[:, :nb_features]
X_valid_r[:, :, 1] = X_valid[:, nb_features:nb_features*2]
X_valid_r[:, :, 2] = X_valid[:, nb_features*2:]

X_test_r = np.zeros((len(X_test), nb_features, 3))
X_test_r[:, :, 0] = X_test[:, :nb_features]
X_test_r[:, :, 1] = X_test[:, nb_features:nb_features*2]
X_test_r[:, :, 2] = X_test[:, nb_features*2:]

#把标记的格式变成神经网络输出对应的格式（这样才能计算误差）
y_train = np_utils.to_categorical(y_train, nb_class)
y_valid = np_utils.to_categorical(y_valid, nb_class)

#一开始X_train的三个特征是连在一起的（64+64+64）
#之后把这三个分开，放到一个独立的维度里（64*3）

#正餐：Keras一维卷积模型（需要改善精度）

if(os.path.exists('1dconv.h5')):
    model=load_model('1dconv.h5')
    predict = model.predict(X_test_r[0:1,:,:])
    print(predict)
else:
    model = Sequential()
#一维卷积层，输入的shape和X_train_r的一个batch一致（64特征内容*3通道指标）
#问题：512和1分别代表什么？
#nb_filter:卷积核的数目（输出维度）out_shape=[64,512]
#filter_length:卷积核长度（时域长度）（sEMG的话这个长度要拉长）
    model.add(Convolution1D(nb_filter=512,filter_length=1,input_shape=(nb_features,3)))
    model.add(Activation('relu'))
    model.add(Flatten())#32768*1，因为后面要接入一维神经网络（MLP）层
    model.add(Dropout(0.5))#16384*1
    model.add(Dense(2048,activation='relu'))
    model.add(Dense(1024,activation='relu'))
    model.add(Dense(nb_class))
    model.add(Activation('softmax'))


#采用SGD优化器进行梯度下降训练，初始学习率0.01
sgd = SGD(lr=0.01,nesterov=True,decay=1e-6,momentum=0.9)

model.compile(loss='categorical_crossentropy',optimizer=sgd,metrics=['accuracy'])

nb_epoch = 15
model.fit(X_train_r,y_train,epochs=nb_epoch, validation_data=(X_valid_r, y_valid), batch_size=16)

predict = model.predict(X_test_r[0:1,:,:])
print(predict)

model.save('1dconv.h5')
