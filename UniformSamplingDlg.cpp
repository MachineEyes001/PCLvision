#include "UniformSamplingDlg.h"

UniformSamplingDlg::UniformSamplingDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

UniformSamplingDlg::~UniformSamplingDlg()
{}

//确认
void UniformSamplingDlg::on_btn_ok_clicked()
{
	int KeepOrganized = ui.KeepOrganized->currentIndex();
	bool ok;
	QString Radius_s = ui.Radius->text();
	double Radius_d = Radius_s.toDouble(&ok);
	if (Radius_d <= 0.0)
	{
		ui.Radius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Radius->setStyleSheet("QLineEdit{border:1px solid black }");
	emit UniformSampling(Radius_d, KeepOrganized);
}

//取消
void UniformSamplingDlg::on_btn_cancel_clicked()
{
	close();
}
