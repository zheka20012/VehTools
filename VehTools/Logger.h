#pragma once
#include <QTextEdit>
#include <QDateTime>

class Logger
{
public:
	Logger(QTextEdit* logWindow = Q_NULLPTR);

	static Logger* instance();

	void Print(QString message);
	void Print(QString message, QColor textColor);

	void Clear();

private:
	static Logger* w_instance;
	QTextEdit* textBox = NULL;
};
