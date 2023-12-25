#include "RegionGrowDlg.h"

RegionGrowDlg::RegionGrowDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

RegionGrowDlg::~RegionGrowDlg()
{}

//ȷ��
void RegionGrowDlg::on_btn_ok_clicked()
{
	bool ok;
	//��ȡSmoothnessThreshold����
	QString SmoothnessThreshold_s = ui.SmoothnessThreshold->text();
	double SmoothnessThreshold_d = SmoothnessThreshold_s.toDouble(&ok);
	if (SmoothnessThreshold_d < 0.0)
	{
		ui.SmoothnessThreshold->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.SmoothnessThreshold->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡCurvatureThreshold����
	QString CurvatureThreshold_s = ui.CurvatureThreshold->text();
	double CurvatureThreshold_d = CurvatureThreshold_s.toDouble(&ok);
	if (CurvatureThreshold_d < 0.0)
	{
		ui.CurvatureThreshold->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.CurvatureThreshold->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡKSearch����
	QString KSearch_s = ui.KSearch->text();
	int KSearch_i = KSearch_s.toInt(&ok);
	if (KSearch_i < 0)
	{
		ui.KSearch->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.KSearch->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡNumberOfNeighbours����
	QString NumberOfNeighbours_s = ui.NumberOfNeighbours->text();
	int NumberOfNeighbours_i = NumberOfNeighbours_s.toInt(&ok);
	if (NumberOfNeighbours_i < 0)
	{
		ui.NumberOfNeighbours->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.NumberOfNeighbours->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡMinClusterSize����
	QString MinClusterSize_s = ui.MinClusterSize->text();
	int MinClusterSize_i = MinClusterSize_s.toInt(&ok);
	if (MinClusterSize_i < 0)
	{
		ui.MinClusterSize->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MinClusterSize->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡMaxClusterSize����
	QString MaxClusterSize_s = ui.MaxClusterSize->text();
	int MaxClusterSize_i = MaxClusterSize_s.toInt(&ok);
	if (MaxClusterSize_i < 0)
	{
		ui.MaxClusterSize->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaxClusterSize->setStyleSheet("QLineEdit{border:1px solid black }");

	emit  RegionGrow(KSearch_i, NumberOfNeighbours_i, SmoothnessThreshold_d, CurvatureThreshold_d, MinClusterSize_i, MaxClusterSize_i);
}

//ȡ��
void RegionGrowDlg::on_btn_cancel_clicked()
{
	close();
}