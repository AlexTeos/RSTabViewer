#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QtCore>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject* parent = nullptr);
    ~Logger();

    static Logger& GetInstance();
    QString        Add(const QString& type, const char* function, const QString& msg, const char* file, int line);
    static void    myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private:
    static Logger* log_instance;

    QFile m_logFile;
};

#endif // LOGGER_H
