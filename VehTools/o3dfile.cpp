#include "o3dfile.h"


QStringList o3dfile::GetTexturesList()
{
	return texturesList;
}

void o3dfile::Load(QString filePath)
{

	if(!QFile::exists(filePath))
	{
		return;
		//TODO: Handle file not exists error
	}

	QFile meshFile(filePath);
	
	if(!meshFile.open(QIODevice::ReadOnly))
	{
		return;
	}

	// Read magic number for o3d 
	qint16 magic;
	meshFile.read(reinterpret_cast<char*>(&magic), 2);

	if(magic != O3D_MAGIC) //Wrong file
	{
		return;
	}
	
	// Determine version
	meshFile.read(&fileVersion, 1);

	if (fileVersion > 7) //File is too new
	{
		return;
	}

	if (fileVersion > 3)
	{
		// OMSI 2 Format

		//TODO: Resolve correct reading of bit mask (for long indices support and equality bit)  
		char bitMask;
		meshFile.read(&bitMask, 1);

		longIndicesSupport = (bitMask & 1) == 1;
		equalityBitSupport = (bitMask & 2) == 2;

		qint32 productId;

		meshFile.read(reinterpret_cast<char*>(&productId), 4);
	}
	else
	{
		// OMSI 1 Format

		longIndicesSupport = false;
		equalityBitSupport = false;
	}

	while (!meshFile.atEnd())
	{
		char keywordChar;

		meshFile.read(&keywordChar, 1);

		switch (keywordChar)
		{
		case O3D_VERTEXDATA:
			skipVerts(&meshFile);
			break;
		case O3D_TRIDATA:
			skipTris(&meshFile);
			break;
		case O3D_TRANSFORM:
			skipTransform(&meshFile);
			break;
		case O3D_BONEDATA:
			skipBones(&meshFile);
			break;
		case O3D_MATERIAL:
			readMaterials(&meshFile);
			break;
		}
	}

	isLoaded = true;
}

void o3dfile::skipVerts(QFile* meshFile)
{
	quint32 vertexCount;
	if (fileVersion >= 3)
	{
		meshFile->read(reinterpret_cast<char*>(&vertexCount), sizeof(quint32));
	}
	else
	{
		quint16 shortCount;
		meshFile->read(reinterpret_cast<char*>(&shortCount), sizeof(quint16));
		vertexCount = shortCount;
	}

	qint64 currentPosition = meshFile->pos();

	currentPosition += vertexCount * 32ll; //skip reading vertexStruct size * vertexCount bytes

	meshFile->seek(currentPosition);
}

void o3dfile::skipTris(QFile * meshFile)
{
	quint32 indexCount;
	if (fileVersion >= 3)
	{
		meshFile->read(reinterpret_cast<char*>(&indexCount), sizeof(quint32));
	}
	else
	{
		quint16 shortCount;
		meshFile->read(reinterpret_cast<char*>(&shortCount), sizeof(quint16));
		indexCount = shortCount;
	}

	qint64 currentPosition = meshFile->pos();

	qint64 structSize = longIndicesSupport ? 14 : 8; //long indices - 4 bytes for index, short - 2 bytes, plus textureId - 2 bytes 

	currentPosition += indexCount * structSize; //skip reading indexStruct size * vertexCount bytes

	meshFile->seek(currentPosition);
}

void o3dfile::readMaterials(QFile* meshFile)
{
	quint16 materialsCount;
	meshFile->read(reinterpret_cast<char*>(&materialsCount), sizeof(quint16));

	texturesList = QStringList();

	for (quint16 m = 0; m < materialsCount; m++)
	{
		qint64 currentPosition = meshFile->pos();

		currentPosition += 11ll * 4ll; 

		meshFile->seek(currentPosition);

		uint8_t textureNameLength;
		meshFile->read(reinterpret_cast<char*>(&textureNameLength), 1);

		char* textureName = new char[textureNameLength+1];
		textureName[textureNameLength] = '\0'; //Make string null-terminated

		meshFile->read(textureName, textureNameLength);

		texturesList.append(QString(textureName));
	}
}

//usually materials are before these two sections, but what if...
void o3dfile::skipTransform(QFile* meshFile)
{
	qint64 currentPosition = meshFile->pos();

	currentPosition += 16ll * 4ll; //skip reading 16 * 4 bytes of matrix

	meshFile->seek(currentPosition);
}

void o3dfile::skipBones(QFile * meshFile)
{
	quint16 boneCount;
	meshFile->read(reinterpret_cast<char*>(&boneCount), sizeof(quint16));

	for (quint16 boneIndex = 0; boneIndex < boneCount; boneIndex++)
	{
		char boneNameLength;
		meshFile->read(&boneNameLength, 1);

		char buffer[1024];

		meshFile->read(buffer, boneNameLength);

		quint16 weightsCount;
		meshFile->read(reinterpret_cast<char*>(&weightsCount), sizeof(quint16));

		qint64 currentPosition = meshFile->pos();

		qint64 structSize = longIndicesSupport ? 8 : 6; 

		currentPosition += weightsCount * structSize; 

		meshFile->seek(currentPosition);
	}
}

