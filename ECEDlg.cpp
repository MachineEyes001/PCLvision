#include "ECEDlg.h"

ECEDlg::ECEDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ECEDlg::~ECEDlg()
{}

//ȷ��
void ECEDlg::on_btn_ok_clicked()
{
	bool ok;
	//��ȡClusterTolerance����
	QString ClusterTolerance_s = ui.ClusterTolerance->text();
	double ClusterTolerance_d = ClusterTolerance_s.toDouble(&ok);
	if (ClusterTolerance_d < 0.0)
	{
		ui.ClusterTolerance->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.ClusterTolerance->setStyleSheet("QLineEdit{border:1px solid black }");
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

	emit  ECE(ClusterTolerance_d, MinClusterSize_i, MaxClusterSize_i);
}

//ȡ��
void ECEDlg::on_btn_cancel_clicked()
{
	close();
}
