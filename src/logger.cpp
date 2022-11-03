#include "logger.h"

Logger* Logger::log_instance = nullptr;

Logger::Logger(QObject* parent) : QObject(parent)
{
    QString workDirectory;
#ifdef Q_OS_ANDROID
    workDirectory = "/sdcard/RSTabsData/";
#else
    workDirectory = "./RSTabsData/";
#endif
    m_logFileName = workDirectory + "RSTabs.log";

    QFile::remove(m_logFileName);
}

Logger& Logger::GetInstance()
{
    static Logger the_instance;
    return the_instance;
}

QString Logger::Add(const QString& type, const char* function, const QString& msg, const char* file, int line)
{
    QString date_time_string = Logger::GetInstance().m_dateTime.currentDateTime().toString("<dd-MM-yyyy hh:mm:ss>");
    QString out              = QString("%1 %2: [%3] \"%4\" at file: %5:%6")
                      .arg(date_time_string, type, function, msg, file, QString::number(line));

    // TODO: to constructor
    QFile logFile(Logger::GetInstance().m_logFileName);
    if (logFile.open(QIODevice::Append))
    {
        // TODO: WTF is that?
        QTextStream stream(&logFile);
        stream << out << Qt::endl;
    }
    // TODO: flush instead open/close
    logFile.close();

    return out;
}

void Logger::myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // TODO: fprintf only in debug
    switch (type)
    {
        case QtDebugMsg:
            // TODO: skip n release mod
            fprintf(stdout,
                    "%s\n",
                    Logger::Add("Debug", context.function, msg, context.file, context.line).toLocal8Bit().data());
            break;
        case QtInfoMsg:
            fprintf(stdout,
                    "%s\n",
                    Logger::Add("Info", context.function, msg, context.file, context.line).toLocal8Bit().data());
            break;
        case QtWarningMsg:
            fprintf(stderr,
                    "%s\n",
                    Logger::Add("Warning", context.function, msg, context.file, context.line).toLocal8Bit().data());
            break;
        case QtCriticalMsg:
            fprintf(stderr,
                    "%s\n",
                    Logger::Add("Critical", context.function, msg, context.file, context.line).toLocal8Bit().data());
            break;
        case QtFatalMsg:
            fprintf(stderr,
                    "%s\n",
                    Logger::Add("Fatal", context.function, msg, context.file, context.line).toLocal8Bit().data());
            abort();
    }
    fflush(stderr);
}
