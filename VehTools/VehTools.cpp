#include "VehTools.h"


VehTools::VehTools(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	Logger logger(ui.logTextBox);
	connect(ui.selectVehicleButton, &QPushButton::clicked, this, &VehTools::OpenFile);
}

void VehTools::OpenFile()
{
	QString filename = QFileDialog::getOpenFileName(this, QObject::tr("Select vehicle file"), QString(""), QObject::tr("OMSI vehicle file (*.bus *.ovh);;All files (*.*)"));

	if(filename == "") return;

	ui.filePath->setText(filename);

	clearTabs();

	ProcessFile(filename);
}

void VehTools::ProcessFile(QString fileName)
{
	KeywordFinder finder = KeywordFinder(fileName);
	QString modelFile = finder.GetFileByKeyword(QString("[model]"));
	QString soundFile = finder.GetFileByKeyword(QString("[sound]"));
	QList<QString> varnameFiles = finder.GetMultilineFileByKeyword(QString("[varnamelist]"), false);
	QList<QString> scriptFiles = finder.GetMultilineFileByKeyword(QString("[script]"), false);
	QList<QString> constFiles = finder.GetMultilineFileByKeyword(QString("[constfile]"), false);
	QList<QString> stringVarnameFiles = finder.GetMultilineFileByKeyword(QString("[stringvarnamelist]"), false);
	
	QFileInfo info(fileName);
	QString textureDir = info.absolutePath().append("/Texture");
	Logger::instance()->Print(QString("Scanning %1").arg(textureDir));
	QStringList presentTextures = ScanDir(textureDir, { "*.jpg", "*.png", "*.dds", "*.tga", "*.bmp", "*.jpeg" });

	Logger::instance()->Print(QString("Searching for textures in meshes"));

	QStringList modelTextures = QStringList();

	KeywordFinder modelFinder = KeywordFinder(modelFile);
	QList<QString> meshesList = modelFinder.GetFilesByKeyword(QString("[mesh]"), false);
	QString modelFileDir = modelFinder.GetAbsolutePath();
	QStringList missingMeshesList = QStringList();

	for (int i = 0; i < meshesList.length(); i++)
	{
		QFileInfo meshInfo(modelFileDir + QDir::separator() + meshesList[i]);
		if (meshInfo.exists() && meshInfo.suffix() == "o3d")
		{
			o3dfile file;
			file.Load(modelFileDir + QDir::separator() + meshesList[i]);
			modelTextures.append(file.GetTexturesList());
		}
		else if(meshInfo.exists() && meshInfo.suffix() == "x")
		{
			xfile file;
			file.Load(modelFileDir + QDir::separator() + meshesList[i]);
			modelTextures.append(file.GetTexturesList());
		}
		else
		{
			missingMeshesList.append(meshesList[i]);
			Logger::instance()->Print(QString("Can't find mesh: %1").arg(meshInfo.filePath()));
		}
	}

	modelTextures.append(modelFinder.GetFilesByKeyword({ "[matl_envmap]", "[matl_transmap]", "[matl_envmap_mask]", "[matl_lightmap]", "[matl_nightmap]", "[matl_bumpmap]" }, false));
	modelTextures.removeDuplicates();

	removeFromList(modelTextures, presentTextures);

	Logger::instance()->Print(QString("Searching for missing sounds"));

	KeywordFinder soundFinder = KeywordFinder(soundFile);
	QStringList soundsList = soundFinder.GetFilesByKeyword({ "[sound]", "[loopsound]" }, false);
	QString soundFolderDir = soundFinder.GetAbsolutePath();
	QStringList missingSoundsList = QStringList();

	for(int i = 0; i< soundsList.count(); i++)
	{
		QFileInfo fileInfo(soundFolderDir + soundsList[i]);
		//Logger::instance()->Print(fileInfo.filePath());
		if(!fileInfo.exists())
		{
			missingSoundsList.append(soundsList[i]);
		}
	}

	Logger::instance()->Print(QString("Searching for missing script files"));

	QString scriptFolderDir = finder.GetAbsolutePath();
	QStringList missingScriptsList = QStringList();

	bool missedScripts = false; //TODO: Name this variable somehow better

	for (int i = 0; i < varnameFiles.count(); i++)
	{
		QFileInfo fileInfo(scriptFolderDir + varnameFiles[i]);
		//Logger::instance()->Print(fileInfo.filePath());
		if (!fileInfo.exists())
		{
			if(!missedScripts)
			{
				missingScriptsList.append("Missing varname files:");
				missedScripts = true;
			}
			missingScriptsList.append(varnameFiles[i]);
		}
	}

	missedScripts = false;

	for (int i = 0; i < stringVarnameFiles.count(); i++)
	{
		QFileInfo fileInfo(scriptFolderDir + stringVarnameFiles[i]);
		//Logger::instance()->Print(fileInfo.filePath());
		if (!fileInfo.exists())
		{
			if (!missedScripts)
			{
				missingScriptsList.append("Missing string variable files:");
				missedScripts = true;
			}
			missingScriptsList.append(stringVarnameFiles[i]);
		}
	}

	missedScripts = false;

	for (int i = 0; i < constFiles.count(); i++)
	{
		QFileInfo fileInfo(scriptFolderDir + constFiles[i]);
		//Logger::instance()->Print(fileInfo.filePath());
		if (!fileInfo.exists())
		{
			if (!missedScripts)
			{
				missingScriptsList.append("Missing constant files:");
				missedScripts = true;
			}
			missingScriptsList.append(constFiles[i]);
		}
	}

	missedScripts = false;

	for (int i = 0; i < scriptFiles.count(); i++)
	{
		QFileInfo fileInfo(scriptFolderDir + scriptFiles[i]);
		//Logger::instance()->Print(fileInfo.filePath());
		if (!fileInfo.exists())
		{
			if (!missedScripts)
			{
				missingScriptsList.append("Missing script files:");
				missedScripts = true;
			}
			missingScriptsList.append(scriptFiles[i]);
		}
	}

	missedScripts = false;

	//Fill tabs

	for (int i = 0; i < modelTextures.length(); i++)
	{
		ui.missingTexturesBox->append(modelTextures[i]);
	}

	for (int i = 0; i < missingMeshesList.length(); i++)
	{
		ui.missingMeshesTextBox->append(missingMeshesList[i]);
	}

	for (int i = 0; i < missingSoundsList.length(); i++)
	{
		ui.missingSoundsBox->append(missingSoundsList[i]);
	}

	for (int i = 0; i < missingScriptsList.length(); i++)
	{
		ui.missingScriptsBox->append(missingScriptsList[i]);
	}

}

void VehTools::clearTabs()
{
	ui.missingMeshesTextBox->clear();
	ui.missingSoundsBox->clear();
	ui.missingTexturesBox->clear();
	ui.missingScriptsBox->clear();
	Logger::instance()->Clear();

}

QStringList VehTools::ScanDir(const QString& folderName, const QStringList& ext)
{
	QDir dir(folderName);

	dir.setNameFilters(ext);

	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	return dir.entryList();
}



VehTools::~VehTools()
{}
