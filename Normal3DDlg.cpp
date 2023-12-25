#include "Normal3DDlg.h"

Normal3DDlg::Normal3DDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Normal3DDlg::~Normal3DDlg()
{}

//确认
void Normal3DDlg::on_btn_ok_clicked()
{
	bool ok;
	QString SearchRadius_s = ui.SearchRadius->text();
	double SearchRadius_d = SearchRadius_s.toDouble(&ok);
	if (SearchRadius_d <= 0.0)
	{
		ui.SearchRadius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.SearchRadius->setStyleSheet("QLineEdit{border:1px solid black }");
	QString Level_s = ui.Level->text();
	int Level_i = Level_s.toInt(&ok);
	if (Level_i <= 0)
	{
		ui.Level->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Level->setStyleSheet("QLineEdit{border:1px solid black }");
	QString Scale_s = ui.Scale->text();
	double Scale_d = Scale_s.toDouble(&ok);
	if (Scale_d <= 0.0)
	{
		ui.Scale->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Scale->setStyleSheet("QLineEdit{border:1px solid black }");

	emit Normal3D(SearchRadius_d, Level_i, Scale_d);
}

//取消
void Normal3DDlg::on_btn_cancel_clicked()
{
	close();
}