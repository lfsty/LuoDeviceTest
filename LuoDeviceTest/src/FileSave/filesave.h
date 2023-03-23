#ifndef FILESAVE_H
#define FILESAVE_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QByteArray>

#include "../ConstData.h"

class FileSave : public QObject
{
    Q_OBJECT
public:
    explicit FileSave(QObject* parent = nullptr);
    ~FileSave();

public:
    void SetFileSavingPath(QString path);
    void StartRecord();
    void StopRecord();
public slots:
    void SaveData(QVector<float> data);

private:
    QString m_root_path = "./LuoData";
    QFile m_qfile[TOTAL_CH_NUM];

signals:

};

#endif // FILESAVE_H
