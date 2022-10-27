#include "filesave.h"

FileSave::FileSave(QObject* parent)
    : QObject{parent}
{

}

FileSave::~FileSave()
{
    StopRecord();
}

void FileSave::SetFileSavingPath(QString path)
{
    m_root_path = path;
}

void FileSave::StartRecord()
{
    //创建保存文件夹
    static QDir dir;
    QString current_file_path = m_root_path + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh.mm.ss");
    if(!dir.exists(current_file_path))
    {
        dir.mkpath(current_file_path);
    }

    //文件打开
    for(int i = 0; i < TOTAL_CH_NUM; i++)
    {
        m_qfile[i].setFileName(current_file_path + "/ch_" + QString::number(i + 1));
        m_qfile[i].open(QIODevice :: WriteOnly | QIODevice :: Truncate);
    }
}

void FileSave::StopRecord()
{
    for(int i = 0; i < TOTAL_CH_NUM; i++)
    {
        if(m_qfile[i].isOpen())
        {
            m_qfile[i].close();
        }
    }
}

void FileSave::SaveData(int index, QByteArray data)
{
    if(m_qfile[index].isOpen())
    {
        m_qfile[index].write(data);
    }
}
