#include "posefile.h"
#include <QtWidgets/QFileDialog>
#include <boost/predef.h>
#include <basetsd.h>

#if BOOST_ENDIAN_LITTLE_BYTE
void readDouble(char *buf, int shift, double *result)
{
	char *p = buf + shift;
	uchar *resultBytes = reinterpret_cast<uchar *>(result);

	for (int i = 0; i < sizeof(double); ++i)
	{
		resultBytes[i] = p[sizeof(double) - 1 - i];
	}
}
#endif

PoseFileReader::PoseFileReader(QObject* parent) : QObject(parent)
{
}

PoseFileReader::~PoseFileReader()
{
}

QList<Pose *> PoseFileReader::load(QString filename) const
{
	QList<Pose *> poses;
	if (!QFile::exists(filename))
		return poses;

	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
		return poses;

	QByteArray blob = file.readAll();
	char *pData = blob.data();
	// the data is in big-endian
	ulong dataLength = static_cast<ulong>((pData[0] << 24) | (pData[1] << 16) | (pData[2] << 8) | pData[3] & 0xff);
	int shift;
	for (ulong clen = 0; clen < dataLength; clen++)
	{
		shift = 4 + (58 * clen);
		Pose *p = new Pose();
		memcpy_s(&p->TimeStamp, 8, pData + shift, 8);

#if BOOST_ENDIAN_LITTLE_BYTE
		p->TimeStamp = _byteswap_uint64(p->TimeStamp);
		readDouble(pData, shift + sizeof(INT64), &p->X);
		readDouble(pData, shift + sizeof(double) * 2, &p->Y);
		readDouble(pData, shift + sizeof(double) * 3, &p->Z);
		readDouble(pData, shift + sizeof(double) * 4, &p->Roll);
		readDouble(pData, shift + sizeof(double) * 5, &p->Pitch);
		readDouble(pData, shift + sizeof(double) * 6, &p->Yaw);
#elif BOOST_ENDIAN_BIG_BYTE
		memcpy_s(&p->X, 8, pData + shift + sizeof(INT64), 8);
		memcpy_s(&p->Y, 8, pData + shift + sizeof(double) * 2, 8);
		memcpy_s(&p->Z, 8, pData + shift + sizeof(double) * 3, 8);
		memcpy_s(&p->Roll, 8, pData + shift + sizeof(double) * 4, 8);
		memcpy_s(&p->Pitch, 8, pData + shift + sizeof(double) * 5, 8);
		memcpy_s(&p->Yaw, 8, pData + shift + sizeof(double) * 6, 8);
#endif
		poses.append(p);
	}

	return poses;
}
