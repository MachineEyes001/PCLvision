#include "PlaneDlg.h"

PlaneDlg::PlaneDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PlaneDlg::~PlaneDlg()
{}

//ȷ��
void PlaneDlg::on_btn_ok_clicked()
{
	bool ok;
	//��ȡlineedit����
	QString LocalPoint_s = ui.LocalPoint->text();
	double LocalPoint_d = LocalPoint_s.toDouble(&ok);
	if (LocalPoint_d < 0.0)
	{
		ui.LocalPoint->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.LocalPoint->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡlineedit����
	QString Iteration_s = ui.Iteration->text();
	int Iteration_i = Iteration_s.toInt(&ok);
	if (Iteration_i < 0)
	{
		ui.Iteration->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Iteration->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡlineedit����
	QString minPointNum_s = ui.minPointNum->text();
	int minPointNum_i = minPointNum_s.toInt(&ok);
	if (minPointNum_i < 0)
	{
		ui.minPointNum->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.minPointNum->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡlineedit����
	QString Confidence_s = ui.Confidence->text();
	double Confidence_d = Confidence_s.toDouble(&ok);
	if (Confidence_d < 0.0)
	{
		ui.Confidence->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Confidence->setStyleSheet("QLineEdit{border:1px solid black }");
	int ifReverse = ui.ifReverse->currentIndex();

	emit RANSAC(0, LocalPoint_d, Iteration_i, minPointNum_i, Confidence_d,0,0,0.0, ifReverse);
}

//ȡ��
void PlaneDlg::on_btn_cancel_clicked()
{
	close();
}
