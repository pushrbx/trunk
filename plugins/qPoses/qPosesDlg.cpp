#include "qPosesDlg.h"
//qCC_db
#include <cc2DViewportObject.h>
//qCC_gl
#include <ccGLWindow.h>
#include <CCTypes.h>
#include <CCGeom.h>
#include <ccGLMatrix.h>
#include <ccCameraSensor.h>
#include "dispatcher.h"
#include "fileLoadModal.h"

//Qt
#include <QtGui>
#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

//standard includes
#include <vector>
#include <functional>

//System
#include <algorithm>
#if defined(CC_WINDOWS)
#include "Windows.h"
#else
#include <unistd.h>
#endif

qPosesDlg::qPosesDlg(ccGLWindow* view3d, ccMainAppInterface* app, QWidget* parent)
	: QDialog(parent, Qt::Tool)
	, Ui::PosesDialog()
	, m_view3d(view3d)
	, m_app(app)
	, m_fileLoadWatcher(this)
	, m_fileLoadModal(new FileLoadDialog(this))
{
	setupUi(this);

	m_currentIndex = 0;
	// events
	connect (openFileBtn, SIGNAL(clicked()), this, SLOT(onBrowseButtonClicked()));
	connect (loadPosesFileBtn, SIGNAL(clicked()), this, SLOT(onLoadPosesFileBtnClicked()));
	connect (prevPoseBtn, SIGNAL(clicked()), this, SLOT(onPrevPoseBtnClicked()));
	connect (nextPoseBtn, SIGNAL(clicked()), this, SLOT(onNextPoseBtnClicked()));
	connect (buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
	connect (&m_fileLoadWatcher, SIGNAL(finished()), this, SLOT(handleFileLoadFinished()));
}
qPosesDlg::~qPosesDlg()
{
	if (m_app != nullptr)
		m_app = nullptr;

	if (m_view3d != nullptr)
		m_view3d = nullptr;

	if (m_poses.length() > 0)
	{
		for (QList<Pose *>::iterator i = m_poses.begin(); i != m_poses.end(); ++i)
		{
			Pose *entry = *i;
			if (entry != nullptr)
				delete entry;
		}
		m_poses.clear();
	}

	if (m_poseObjects.length() > 0)
	{
		for (QList<cc2DViewportObject *>::iterator i = m_poseObjects.begin(); i != m_poseObjects.end(); ++i)
		{
			cc2DViewportObject *entry = *i;
			if (entry != nullptr)
				delete entry;
		}
		m_poseObjects.clear();
	}

	if (m_fileLoadModal != nullptr)
	{
		delete m_fileLoadModal;
		m_fileLoadModal = nullptr;
	}
}

void qPosesDlg::onBrowseButtonClicked()
{
	m_posesFilePath = QFileDialog::getOpenFileName(this, tr("Open .poses file"),
		QDir::currentPath(), tr("poses files (*.poses)"));
	this->poseFilePathBox->setText(m_posesFilePath);
}


void qPosesDlg::onLoadPosesFileBtnClicked()
{
	if (this->poseFilePathBox->text() != m_posesFilePath)
		m_posesFilePath = this->poseFilePathBox->text();

	if (!m_posesFilePath.isNull())
	{
		QFuture<bool> result = QtConcurrent::run(QThreadPool::globalInstance(), this, &qPosesDlg::loadFile, m_posesFilePath);
		m_fileLoadWatcher.setFuture(result);
		m_fileLoadModal->show();
	}
}

void qPosesDlg::onPrevPoseBtnClicked()
{

}

void qPosesDlg::onNextPoseBtnClicked()
{

}

void qPosesDlg::onAccept()
{

}

bool qPosesDlg::loadFile(QString fileName)
{
	PoseFileReader reader;
	m_poses = reader.load(fileName);
	m_fileLoadResult = true;
	m_fileLoadModal->onProgressValueChanged(50);
	if (m_poses.length() > 0)
	{
		int idx = 0;
		for (QList<Pose *>::iterator i = m_poses.begin(); i != m_poses.end(); ++i, idx++)
		{
			Pose *entry = *i;
			cc2DViewportObject *pViewPort = new cc2DViewportObject(QString("Generated_Pose_#%1").arg(idx));
			ccViewportParameters params(m_view3d->getViewportParameters());

			//orientation matrix
			ccGLMatrixd rot;
			{
				double *mat = rot.data();
				// for now just an identity matrix:
				mat[0] = 1; // x
				mat[1] = 0;
				mat[2] = 0;

				mat[4] = 0;
				mat[5] = 1; // y
				mat[6] = 0;

				mat[8] = 0;
				mat[9] = 0;
				mat[10] = 1; // z
			}

			params.viewMat = rot;
			CCVector3d C(entry->X,
						entry->Y,
						entry->Z);
			params.cameraCenter = C;

			pViewPort->setParameters(params);
			pViewPort->setDisplay(m_view3d);

			m_poseObjects.append(pViewPort);
		}
		m_fileLoadModal->onProgressValueChanged(100);
		return true;
	}
	return false;
}

void qPosesDlg::handleFileLoadFinished()
{
	if (m_fileLoadResult)
	{
		QMessageBox::information(this, tr("Info - Poses plugin"), tr("Poses file loaded."), QMessageBox::Ok);
	}

	m_fileLoadModal->close();
}

void qPosesDlg::applyViewport(const cc2DViewportObject* viewport)
{
	if (viewport == nullptr)
		return;

	if (m_view3d)
	{
		m_view3d->setViewportParameters( viewport->getParameters() );
		m_view3d->redraw();
	}
}