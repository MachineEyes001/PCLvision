#include "GP3Dlg.h"

GP3Dlg::GP3Dlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

GP3Dlg::~GP3Dlg()
{}

//确认
void GP3Dlg::on_btn_ok_clicked()
{
	int NormalConsistency = ui.NormalConsistency->currentIndex();
	bool ok;
	//获取KSearch数据
	QString KSearch_s = ui.KSearch->text();
	int KSearch_i = KSearch_s.toInt(&ok);
	if (KSearch_i < 0)
	{
		ui.KSearch->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.KSearch->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取数据
	QString SearchRadius_s = ui.SearchRadius->text();
	double SearchRadius_d = SearchRadius_s.toDouble(&ok);
	if (SearchRadius_d < 0.0)
	{
		ui.SearchRadius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.SearchRadius->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取数据
	QString Mu_s = ui.Mu->text();
	double Mu_d = Mu_s.toDouble(&ok);
	if (Mu_d < 0.0)
	{
		ui.Mu->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Mu->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取MaximumNearestNeighbors数据
	QString MaximumNearestNeighbors_s = ui.MaximumNearestNeighbors->text();
	int MaximumNearestNeighbors_i = MaximumNearestNeighbors_s.toInt(&ok);
	if (MaximumNearestNeighbors_i < 0)
	{
		ui.MaximumNearestNeighbors->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaximumNearestNeighbors->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取数据
	QString MaximumSurfaceAngle_s = ui.MaximumSurfaceAngle->text();
	double MaximumSurfaceAngle_d = MaximumSurfaceAngle_s.toDouble(&ok);
	if (MaximumSurfaceAngle_d < 0.0)
	{
		ui.MaximumSurfaceAngle->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaximumSurfaceAngle->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取数据
	QString MinimumAngle_s = ui.MinimumAngle->text();
	double MinimumAngle_d = MinimumAngle_s.toDouble(&ok);
	if (MinimumAngle_d < 0.0)
	{
		ui.MinimumAngle->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MinimumAngle->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取数据
	QString MaximumAngle_s = ui.MaximumAngle->text();
	double MaximumAngle_d = MaximumAngle_s.toDouble(&ok);
	if (MaximumAngle_d < 0.0)
	{
		ui.MaximumAngle->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaximumAngle->setStyleSheet("QLineEdit{border:1px solid black }");

	emit  GP3(KSearch_i, SearchRadius_d, Mu_d, MaximumNearestNeighbors_i, MaximumSurfaceAngle_d, MinimumAngle_d, MaximumAngle_d, NormalConsistency);
}
//取消
void GP3Dlg::on_btn_cancel_clicked()
{
	close();
}
