#include "ConditionalDlg.h"

ConditionalDlg::ConditionalDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ConditionalDlg::~ConditionalDlg()
{}

//确认
void ConditionalDlg::on_btn_ok_clicked()
{
	int KeepOrganized = ui.KeepOrganized->currentIndex();
	bool ok;
	//获取lineedit数据
	QString GT_X_s = ui.GT_X->text();
	double GT_X_d = GT_X_s.toDouble(&ok);
	QString LT_X_s = ui.LT_X->text();
	double LT_X_d = LT_X_s.toDouble(&ok);
	QString GT_Y_s = ui.GT_Y->text();
	double GT_Y_d = GT_Y_s.toDouble(&ok);
	QString LT_Y_s = ui.LT_Y->text();
	double LT_Y_d = LT_Y_s.toDouble(&ok);
	QString GT_Z_s = ui.GT_Z->text();
	double GT_Z_d = GT_Z_s.toDouble(&ok);
	QString LT_Z_s = ui.LT_Z->text();
	double LT_Z_d = LT_Z_s.toDouble(&ok);
	emit Conditional(KeepOrganized, GT_X_d, LT_X_d, GT_Y_d, LT_Y_d, GT_Z_d, LT_Z_d);
}

//取消
void ConditionalDlg::on_btn_cancel_clicked()
{
	close();
}
