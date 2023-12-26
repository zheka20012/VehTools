#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VehTools.h"
#include <QFileDialog>
#include <QTranslator>
#include "Logger.h"
#include "KeywordFinder.h"
#include "o3dfile.h"
#include "xfile.h"
#include "IMeshFile.h"

class VehTools : public QMainWindow
{
    Q_OBJECT

public:
    VehTools(QWidget *parent = nullptr);
    ~VehTools();

	void OpenFile();
	QStringList ScanDir(const QString& folderName, const QStringList& ext);

private:
    Ui::VehToolsClass ui;

	void VehTools::ProcessFile(QString fileName);
	void clearTabs();
	void removeFromList(QStringList& list, const QStringList& toDelete) {
		QStringListIterator i(toDelete);
		while (i.hasNext()) {
			list.removeAll(i.next());
		}
	}
};
