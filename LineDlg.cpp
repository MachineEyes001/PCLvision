#include "LineDlg.h"

LineDlg::LineDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

LineDlg::~LineDlg()
{}

//确认
void LineDlg::on_btn_ok_clicked()
{
	bool ok;
	//获取lineedit数据
	QString LocalPoint_s = ui.LocalPoint->text();
	double LocalPoint_d = LocalPoint_s.toDouble(&ok);
	if (LocalPoint_d < 0.0)
	{
		ui.LocalPoint->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.LocalPoint->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString Iteration_s = ui.Iteration->text();
	int Iteration_i = Iteration_s.toInt(&ok);
	if (Iteration_i < 0)
	{
		ui.Iteration->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Iteration->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString Confidence_s = ui.Confidence->text();
	double Confidence_d = Confidence_s.toDouble(&ok);
	if (Confidence_d < 0.0)
	{
		ui.Confidence->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Confidence->setStyleSheet("QLineEdit{border:1px solid black }");

	int ifReverse = ui.ifReverse->currentIndex();

	emit RANSAC(1, LocalPoint_d, Iteration_i, 0, Confidence_d,0,0,0.0, ifReverse);
}

//取消
void LineDlg::on_btn_cancel_clicked()
{
	close();
}
