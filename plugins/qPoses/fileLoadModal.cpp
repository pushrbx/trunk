#include "fileLoadModal.h"
#include "ui_fileLoadModal.h"

FileLoadDialog::FileLoadDialog(QWidget* parent) : QDialog(parent), ui(new Ui::FileLoadDialog), m_dispatcher(this)
{
	ui->setupUi(this);
}

FileLoadDialog::~FileLoadDialog()
{
}

void FileLoadDialog::onProgressTextChanged(QString text)
{
	m_dispatcher.fireAndForget([=] {
		ui->label->setText(text);
	});
}

void FileLoadDialog::onProgressValueChanged(int progressValue)
{
	m_dispatcher.fireAndForget([=] {
		ui->progressBar->setValue(progressValue);
	});
}

