#include "IntegralImageDlg.h"

IntegralImageDlg::IntegralImageDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

IntegralImageDlg::~IntegralImageDlg()
{}

//确认
void IntegralImageDlg::on_btn_ok_clicked()
{
	int EstimationMethod = ui.EstimationMethod->currentIndex();
	bool ok;
	QString DepthChangeFactor_s = ui.DepthChangeFactor->text();
	double DepthChangeFactor_d = DepthChangeFactor_s.toDouble(&ok);
	if (DepthChangeFactor_d <= 0.0)
	{
		ui.DepthChangeFactor->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.DepthChangeFactor->setStyleSheet("QLineEdit{border:1px solid black }");
	QString NormalSmoothingSize_s = ui.NormalSmoothingSize->text();
	double NormalSmoothingSize_d = NormalSmoothingSize_s.toDouble(&ok);
	if (NormalSmoothingSize_d <= 0.0)
	{
		ui.NormalSmoothingSize->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.NormalSmoothingSize->setStyleSheet("QLineEdit{border:1px solid black }");
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

	emit IntegralImage(EstimationMethod, DepthChangeFactor_d, NormalSmoothingSize_d, Level_i, Scale_d);
}

//取消
void IntegralImageDlg::on_btn_cancel_clicked()
{
	close();
}
