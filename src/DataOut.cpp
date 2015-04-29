#include "DataOut.h"
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>

using namespace std;

DataOut::DataOut(QVector<QPair<QString,QVector<double> > > data){
    this->input = data;

}

void DataOut::run(){

    Collector();
}


void DataOut::Collector(){
    //TODO: Create data collection function for app. Figure out how to seperate data required. Maybe do it in
    //      different class -> pass finished QJsonObject to here.

    QJsonObject CollectedData;

    foreach (const auto &data, this->input){
        QJsonArray tempArray;
        foreach(const auto &values, data.second){
            tempArray.append(values);
        }
        CollectedData[data.first] = tempArray;
        qDebug() << ToByteArray(CollectedData);
    }



}


QByteArray DataOut::ToByteArray(QJsonObject input){
    QJsonDocument doc(input);
    QByteArray array = doc.toJson();
    return array;
}

void DataOut::Send(QByteArray toSend){
    qDebug() << "\n TEST \n";
}



DataOut::~DataOut()
{

}

