#include "Logger.h"

Logger* Logger::w_instance = Q_NULLPTR;

Logger::Logger(QTextEdit *logWindow)
{
	textBox = logWindow;
	w_instance = this;
}

Logger* Logger::instance()
{
	return w_instance;
}


void Logger::Clear()
{
	Logger::textBox->clear();
}

void Logger::Print(QString message)
{
	Logger::textBox->append(QString("[%1] %2\r\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss"), message));
}

