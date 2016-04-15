#ifndef POSEFILE_H
#define POSEFILE_H

#include <QObject>

struct Pose
{
	unsigned __int64 TimeStamp;
	double X;
	double Y;
	double Z;
	double Roll;
	double Pitch;
	double Yaw;
	ulong Reserved;
};

class PoseFileReader : QObject
{
	Q_OBJECT

public:
	explicit PoseFileReader(QObject *parent = 0);
	~PoseFileReader();

	QList<Pose *> load(QString filename) const;
};

#endif
