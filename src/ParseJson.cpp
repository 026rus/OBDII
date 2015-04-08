#include "ParseJson.h"
#include <QFile>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>


ParseJson::ParseJson()
{

    LoadFile("B0001");

}

ParseJson::ParseJson(QString code){
    LoadFile(code);
}


void ParseJson::LoadFile(QString code){

    QChar letter = code.at(0);
    QString path;

    switch(letter.toLatin1()){
    case 'B':
        path = "data/Bcodes.json";
        break;
    case 'C':
        path = "data/ccodes.json";
           break;
    case 'U':
        path = "data/ucodes.json";
        break;
    case 'P':
        path = "data/pcodes.json";
        break;
    default:
        path = "data/sample.json";
        break;

    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        printf("Failed to read\n");
        return;
    }
    QByteArray parsedJson = file.readAll();
    /*while (!file.atEnd()) {
        parsedJson.append(file.readLine());

    }*/

   // printf(parsedJson.constData());
    SearchFile(parsedJson, code);
}

void ParseJson::SearchFile(QByteArray parsedJson, QString code){

    QJsonDocument jsonDoc = QJsonDocument::fromJson(parsedJson);
    QJsonObject jsonObj = jsonDoc.object();

    if (jsonDoc.isObject()){

        QJsonObject::iterator itr = jsonObj.find(code);
        if (itr == jsonObj.end()){
            qDebug() << "Code Not Found";
        }else{

            QJsonValue jsonVal = jsonObj.value(code);


          //QJsonValue jsonVal = *itr.value()->toObject()->find(code);
            qDebug() << jsonVal.toString();

        }


    }
}

ParseJson::~ParseJson()
{

}

