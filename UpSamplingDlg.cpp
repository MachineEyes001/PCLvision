#include "UpSamplingDlg.h"

UpSamplingDlg::UpSamplingDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

UpSamplingDlg::~UpSamplingDlg()
{}

//确认
void UpSamplingDlg::on_btn_ok_clicked()
{
	int KeepOrganized = ui.KeepOrganized->currentIndex();
	bool ok;
	QString SearchRadius_s = ui.SearchRadius->text();
	double SearchRadius_d = SearchRadius_s.toDouble(&ok);
	if (SearchRadius_d <= 0.0)
	{
		ui.SearchRadius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.SearchRadius->setStyleSheet("QLineEdit{border:1px solid black }");
	QString SamplingRadius_s = ui.SamplingRadius->text();
	double SamplingRadius_d = SamplingRadius_s.toDouble(&ok);
	if (SamplingRadius_d <= 0.0)
	{
		ui.SamplingRadius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.SamplingRadius->setStyleSheet("QLineEdit{border:1px solid black }");
	QString SamplingStep_s = ui.SamplingStep->text();
	double SamplingStep_d = SamplingStep_s.toDouble(&ok);
	if (SamplingStep_d <= 0.0)
	{
		ui.SamplingStep->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.SamplingStep->setStyleSheet("QLineEdit{border:1px solid black }");
	emit UpSampling(SearchRadius_d, SamplingRadius_d, SamplingStep_d, KeepOrganized);
}

//取消
void UpSamplingDlg::on_btn_cancel_clicked()
{
	close();
}