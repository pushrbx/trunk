#ifndef POSES_DLG_HEADER
#define POSES_DLG_HEADER

#include <QDialog>
#include <QtConcurrent/QtConcurrent>
#include <vector>

#include "ui_PosesDlg.h"
#include "posefile.h"

class ccGLWindow;
class cc2DViewportObject;
class ccMainAppInterface;
class FileLoadDialog;

//! Dialog for qPoses plugin
class qPosesDlg : public QDialog, public Ui::PosesDialog
{
	Q_OBJECT

public:

	//! Default constructor
	qPosesDlg(ccGLWindow* view3d, ccMainAppInterface* app, QWidget* parent = 0);
	~qPosesDlg();

protected slots:

	void onBrowseButtonClicked();
	void onLoadPosesFileBtnClicked();
	void onPrevPoseBtnClicked();
	void onNextPoseBtnClicked();
	void onAccept();

	bool loadFile(QString fileName);
	void handleFileLoadFinished();

protected:
	ccMainAppInterface* m_app;
	ccGLWindow* m_view3d;
	QList<Pose *> m_poses;
	QList<cc2DViewportObject *> m_poseObjects;
	QString m_posesFilePath;
	QFutureWatcher<bool> m_fileLoadWatcher;
	FileLoadDialog *m_fileLoadModal;

	bool m_fileLoadResult;
	int m_currentIndex;

	void applyViewport(const cc2DViewportObject* viewport);
};

#endif