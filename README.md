# IoT-Twin-Digital-Bridge

## Project for the Internet of Things course 2020/2021 Universita' di Genova
#### By Arezou Ranjbarpour Maralani & Lorenzo Tibaldi 

## Introduction
The goal of this project was to create a prototype for a realtime infrastructure monitoring system.
In particular the focus was on the implementation of an IoT system that would let the user monitor readings coming from an array of sensors attached to a bridge and at the same time offering an interface that could let them customize alert notifications based on these readings.

The project can be divided into 4 parts:
* The system backbone
* The sensors' design
* The system dashboard
* The system persistency


## The system backbone
The backbone has been developed with [Node-red](https://nodered.org/), a visual programming tool that makes creating projects like this much easier.
When the Node-red builtin nodes wouldn't work for a specific use case (and that's, most of the time, inevitable) it is possible to write javascript (Nodejs) code in the builtin `function node`.

![Node-red_example](/Media/Main_node-red.png)


ARDUINO SCHEMATICS AND SKETCH ON TINKERCAD:
https://www.tinkercad.com/things/iA9xcBoBmd5-twindigitalbridge-sensor

 
