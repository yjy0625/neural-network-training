# neural-network-training

## Introduction
A Mac software that visualizes BP Neural Network progress.

## Features
* Accept files as data input and trains a neural network to fit the data
* Provide user interface to customize neural network's architecture, learning rate, lambda, and number of iteration
* Show change of cost function, training set precision and test set precision throughout the training process  
* Support parameter update (learning rate, lambda, and number of iteration) during training  

## User Interface

![Program Snapshot](Executable\ File/snapshot.png "Program Snapshot")

* Cost Function Tab: Shows change of cost function with x-axis as number of iteration and y-axis as value (with lowest side 0 and highest side cost function before the first iteration)  
* Precision Tab: Shows change of precision throughout training process. Red line denotes training set precision; blue line denotes test set precision  
* Test Set Tab: Visualizes which data in the whole dataset is identified correctly. White box indicates correct and black means incorrect  
* Table: Presents cost function and precision in raw data format  
* Network: Provides UI to change the architecture of the neural network  
* Data: Provides UI to browse data from files and specify file format  
* Training: Provides UI to customize parameters related to training process  
* Start and Stop Buttons: Allows user to start or terminate the training

## Limitations
* Size of data and dataset cannot be automatically read; incorrectly inputing values might cause program error  
* Does not support training for regression problems yet  
* Any other errors that are yet to be found  

## Disclaimer
This project is for self-learning purposes only and is not guaranteed correct in all cases. The author of this project is not responsible for any negative outcome that using this project gives rise to.
