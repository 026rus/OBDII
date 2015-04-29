#include "DataOut.h"
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>

using namespace std;

DataOut::DataOut(QVector<QPair<QString,QVector<double> > > data, bool toServer){
    this->input = data;
    this->toServer = toServer;
}

void DataOut::run(){

   QJsonObject collectedData = Collector();
    if (toServer) Send(ToByteArray(collectedData));
    else if(!toServer) Save(ToByteArray(collectedData));
}


QJsonObject DataOut::Collector(){
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

    return CollectedData;

}


QByteArray DataOut::ToByteArray(QJsonObject input){
    QJsonDocument doc(input);
    QByteArray array = doc.toJson();
    return array;
}

void DataOut::Save(QByteArray toSave){
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-mm-dd_h_m_s");
    QFile saveFile(currentDate.append(".json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    }


    QJsonDocument saveDoc = QJsonDocument::fromJson(toSave);
    saveFile.write(saveDoc.toJson());


}

void DataOut::Send(QByteArray toSend){
    qDebug() << "\n TEST \n";
}


DataOut::~DataOut()
{

}

