#ifndef DATAOUT_H
#define DATAOUT_H

#include <QPair>
#include <QByteArray>
#include <QJsonObject>
#include <QThread>
#include <QObject>
#include <QVector>


class DataOut : public QThread
{
    Q_OBJECT

public:
    DataOut(QVector<QPair<QString,QVector<double> > > data, bool toServer);
    void run() Q_DECL_OVERRIDE;
    ~DataOut();

private:
    QByteArray ToByteArray(QJsonObject input);
    void Save(QByteArray toSend);
    void Send(QByteArray toSend);
    QJsonObject Collector();

    QVector<QPair<QString,QVector<double> > > input;
    bool toServer;

};

#endif // DATAOUT_H
