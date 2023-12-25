#include "QHeightRampDlg.h"

QHeightRampDlg::QHeightRampDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowModality(Qt::ApplicationModal);
}

QHeightRampDlg::~QHeightRampDlg()
{}

//ȷ��
void QHeightRampDlg::on_btn_ok_clicked()
{
	int dir = ui.comboBox_direction->currentIndex();
	double height = ui.spinBox_period->value();
	emit setHeightRamp(dir, height);
	close();
}

//ȡ��
void QHeightRampDlg::on_btn_cancel_clicked()
{
	close();
}
