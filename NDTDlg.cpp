#include "NDTDlg.h"
#include <QFileDialog>

NDTDlg::NDTDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

NDTDlg::~NDTDlg()
{}

//确认
void NDTDlg::on_btn_ok_clicked()
{
	int Axis = ui.Axis->currentIndex();
	bool ok;
	QString TransformationEpsilon_s = ui.TransformationEpsilon->text();
	double TransformationEpsilon_d = TransformationEpsilon_s.toDouble(&ok);
	if (TransformationEpsilon_d <= 0.0)
	{
		ui.TransformationEpsilon->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.TransformationEpsilon->setStyleSheet("QLineEdit{border:1px solid black }");
	QString StepSize_s = ui.StepSize->text();
	double StepSize_d = StepSize_s.toDouble(&ok);
	if (StepSize_d <= 0.0)
	{
		ui.StepSize->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.StepSize->setStyleSheet("QLineEdit{border:1px solid black }");
	QString Resolution_s = ui.Resolution->text();
	double Resolution_d = Resolution_s.toDouble(&ok);
	if (Resolution_d <= 0.0)
	{
		ui.Resolution->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Resolution->setStyleSheet("QLineEdit{border:1px solid black }");
	QString MaximumIterations_s = ui.MaximumIterations->text();
	int MaximumIterations_i = MaximumIterations_s.toInt(&ok);
	if (MaximumIterations_i <= 0)
	{
		ui.MaximumIterations->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaximumIterations->setStyleSheet("QLineEdit{border:1px solid black }");
	QString AngleRotation_s = ui.AngleRotation->text();
	double AngleRotation_d = AngleRotation_s.toDouble(&ok);
	if (AngleRotation_d < 0.0)
	{
		ui.AngleRotation->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.AngleRotation->setStyleSheet("QLineEdit{border:1px solid black }");
	QString X_s = ui.X->text();
	double X_d = X_s.toDouble(&ok);
	if (X_d < 0.0)
	{
		ui.X->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.X->setStyleSheet("QLineEdit{border:1px solid black }");
	QString Y_s = ui.Y->text();
	double Y_d = Y_s.toDouble(&ok);
	if (Y_d < 0.0)
	{
		ui.Y->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Y->setStyleSheet("QLineEdit{border:1px solid black }");
	QString Z_s = ui.Z->text();
	double Z_d = Z_s.toDouble(&ok);
	if (Z_d < 0.0)
	{
		ui.Z->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Z->setStyleSheet("QLineEdit{border:1px solid black }");
	QString leaf_s = ui.leaf->text();
	double leaf_d = leaf_s.toDouble(&ok);
	if (leaf_d <= 0.0)
	{
		ui.leaf->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.leaf->setStyleSheet("QLineEdit{border:1px solid black }");

	emit NDT(TransformationEpsilon_d, StepSize_d, Resolution_d, MaximumIterations_i, Axis, AngleRotation_d, X_d, Y_d, Z_d, leaf_d);
}

//取消
void NDTDlg::on_btn_cancel_clicked()
{
	close();
}

// 浏览
void NDTDlg::on_BrowseTarget_clicked()
{
	//获取点云路径
	pathtarget = QFileDialog::getOpenFileName(this, "选择点云文件", ".//", "点云文件(*.txt *.pcd);;所有文件(*.*)");
	ui.PathTarget->setText(pathtarget);
}
// 浏览
void NDTDlg::on_BrowseScene_clicked()
{
	//获取点云路径
	pathscene = QFileDialog::getOpenFileName(this, "选择点云文件", ".//", "点云文件(*.txt *.pcd);;所有文件(*.*)");
	ui.PathScene->setText(pathscene);
}