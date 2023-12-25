#include "RemoveOutliersDlg.h"

RemoveOutliersDlg::RemoveOutliersDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

RemoveOutliersDlg::~RemoveOutliersDlg()
{}

//确认
void RemoveOutliersDlg::on_btn_ok_clicked()
{
	int KeepOrganized = ui.KeepOrganized->currentIndex();
	bool ok;
	//获取lineedit数据
	QString Knum_s = ui.Knum->text();
	int Knum_i = Knum_s.toInt(&ok);
	if (Knum_i < 0)
	{
		ui.Knum->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Knum->setStyleSheet("QLineEdit{border:1px solid black }");
	QString Tresh_s = ui.Tresh->text();
	double Tresh_d = Tresh_s.toDouble(&ok);
	if (Tresh_d < 0.0)
	{
		ui.Tresh->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Tresh->setStyleSheet("QLineEdit{border:1px solid black }");
	emit RemoveOutliers(Knum_i, Tresh_d, KeepOrganized);
}

//取消
void RemoveOutliersDlg::on_btn_cancel_clicked()
{
	close();
}
