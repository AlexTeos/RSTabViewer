#include "logger.h"

Logger* Logger::log_instance = nullptr;

Logger::Logger(QObject* parent) : QObject(parent)
{
#ifdef Q_OS_ANDROID
    m_logFile.setFileName("/sdcard/RSTabsData/RSTabs.log");
#else
    m_logFile.setFileName("./RSTabsData/RSTabs.log");
#endif
    m_logFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
}

Logger::~Logger()
{
    m_logFile.close();
}

Logger& Logger::GetInstance()
{
    static Logger the_instance;
    return the_instance;
}

const std::array<QString, 5> QtMsgTypeString{"Debug", "Warning", "Critical", "Fatal", "Info"};

void Logger::myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
#ifdef QT_NO_DEBUG
    if (type == QtDebugMsg) return;
#endif

    QString date_time_string = QDateTime::currentDateTime().toString("<dd-MM-yyyy hh:mm:ss>");

#ifdef QT_DEBUG
    QByteArray out = QString("%1 %2: [%3] %4 at file: %5:%6\n")
                         .arg(date_time_string,
                              QtMsgTypeString[type],
                              context.function,
                              msg,
                              context.file,
                              QString::number(context.line))
                         .toLatin1();
#else
    QByteArray out = QString("%1 %2: %4\n").arg(date_time_string, QtMsgTypeString[type], msg).toLatin1();
#endif

    if (GetInstance().m_logFile.isOpen())
    {
        GetInstance().m_logFile.write(out);
        GetInstance().m_logFile.flush();
    }

#ifdef QT_DEBUG
    switch (type)
    {
        case QtDebugMsg:
        case QtInfoMsg:
            fprintf(stdout, "%s", out.data());
            break;
        case QtWarningMsg:
        case QtCriticalMsg:
        case QtFatalMsg:
            fprintf(stderr, "%s", out.data());
            break;
        default:
            return;
    }
#endif

    fflush(stderr);
}
