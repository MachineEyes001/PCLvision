#include "RadiusFilterDlg.h"

RadiusFilterDlg::RadiusFilterDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

RadiusFilterDlg::~RadiusFilterDlg()
{}

//确认
void RadiusFilterDlg::on_btn_ok_clicked()
{
	int KeepOrganized = ui.KeepOrganized->currentIndex();
	bool ok;
	//获取lineedit数据
	QString minKnum_s = ui.minKnum->text();
	int minKnum_i = minKnum_s.toInt(&ok);
	if (minKnum_i < 0)
	{
		ui.minKnum->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.minKnum->setStyleSheet("QLineEdit{border:1px solid black }");
	QString Radius_s = ui.Radius->text();
	double Radius_d = Radius_s.toDouble(&ok);
	if (Radius_d <= 0.0)
	{
		ui.Radius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Radius->setStyleSheet("QLineEdit{border:1px solid black }");
	emit RadiusFilter(Radius_d, minKnum_i, KeepOrganized);
}

//取消
void RadiusFilterDlg::on_btn_cancel_clicked()
{
	close();
}