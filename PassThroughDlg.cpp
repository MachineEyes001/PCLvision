#include "PassThroughDlg.h"

PassThroughDlg::PassThroughDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PassThroughDlg::~PassThroughDlg()
{}

//确认
void PassThroughDlg::on_btn_ok_clicked()
{
	int dir = ui.Dir_comboBox->currentIndex();
	int KeepOrganized = ui.KeepOrganized->currentIndex();
	bool ok;
	//获取lineedit数据
	QString min_s = ui.min->text();
	double min_d = min_s.toDouble(&ok);
	//if (min_d < 0.0)
	//{
	//	ui.min->setStyleSheet("QLineEdit { color: red; }");
	//}
	//else
	//	ui.min->setStyleSheet("QLineEdit{border:1px solid black }");
	QString max_s = ui.max->text();
	double max_d = max_s.toDouble(&ok);
	//if (max_d < 0.0)
	//{
	//	ui.max->setStyleSheet("QLineEdit { color: red; }");
	//}
	//else
	//	ui.max->setStyleSheet("QLineEdit{border:1px solid black }");
	emit PassThrough(dir, min_d, max_d, KeepOrganized);
}

//取消
void PassThroughDlg::on_btn_cancel_clicked()
{
	close();
}