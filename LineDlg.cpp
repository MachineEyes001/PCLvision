#include "LineDlg.h"

LineDlg::LineDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

LineDlg::~LineDlg()
{}

//ȷ��
void LineDlg::on_btn_ok_clicked()
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

//ȡ��
void LineDlg::on_btn_cancel_clicked()
{
	close();
}
