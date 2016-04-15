#ifndef FILELOADMODAL_H
#define FILELOADMODAL_H

#include <QDialog>
#include "dispatcher.h"

namespace Ui
{
	class FileLoadDialog;
}

class FileLoadDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FileLoadDialog(QWidget *parent = 0);
	~FileLoadDialog();

public slots:
	void onProgressValueChanged(int progressValue);
	void onProgressTextChanged(QString text);

private:
	Ui::FileLoadDialog *ui;
	Dispatcher m_dispatcher;

};

#endif