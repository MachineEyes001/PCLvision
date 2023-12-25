#include "PolygonDlg.h"

PolygonDlg::PolygonDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PolygonDlg::~PolygonDlg()
{}

//确认
void PolygonDlg::on_btn_ok_clicked()
{
	bool ok;
	QString Alpha_s = ui.Alpha->text();
	double Alpha_d = Alpha_s.toDouble(&ok);
	if (Alpha_d < 0.0)
	{
		ui.Alpha->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Alpha->setStyleSheet("QLineEdit{border:1px solid black }");

	emit  Polygon(Alpha_d);
}
//取消
void PolygonDlg::on_btn_cancel_clicked()
{
	close();
}
