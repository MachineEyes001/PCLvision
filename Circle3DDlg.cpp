#include "Circle3DDlg.h"

Circle3DDlg::Circle3DDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Circle3DDlg::~Circle3DDlg()
{}

//ȷ��
void Circle3DDlg::on_btn_ok_clicked()
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
	//��ȡKdTreeSearhNum����
	QString KdTreeSearhNum_s = ui.KdTreeSearhNum->text();
	int KdTreeSearhNum_i = KdTreeSearhNum_s.toInt(&ok);
	if (KdTreeSearhNum_i < 0)
	{
		ui.KdTreeSearhNum->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.KdTreeSearhNum->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡNormalWeight����
	QString NormalWeight_s = ui.NormalWeight->text();
	double NormalWeight_d = NormalWeight_s.toDouble(&ok);
	if (NormalWeight_d < 0.0)
	{
		ui.NormalWeight->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.NormalWeight->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡMinR����
	QString MinR_s = ui.MinR->text();
	double MinR_d = MinR_s.toDouble(&ok);
	if (MinR_d < 0.0)
	{
		ui.MinR->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MinR->setStyleSheet("QLineEdit{border:1px solid black }");
	//��ȡMaxR����
	QString MaxR_s = ui.MaxR->text();
	double MaxR_d = MaxR_s.toDouble(&ok);
	if (MaxR_d < 0.0)
	{
		ui.MaxR->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaxR->setStyleSheet("QLineEdit{border:1px solid black }");

	int ModelOPTI = ui.ModelOPTI->currentIndex();
	int ifReverse = ui.ifReverse->currentIndex();

	emit RANSAC(3, LocalPoint_d, Iteration_i, 0, Confidence_d, KdTreeSearhNum_i, ModelOPTI, NormalWeight_d, ifReverse, MinR_d, MaxR_d);
}

//ȡ��
void Circle3DDlg::on_btn_cancel_clicked()
{
	close();
}
