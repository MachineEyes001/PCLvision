#include "ICPDlg.h"
#include <QFileDialog>

ICPDlg::ICPDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ICPDlg::~ICPDlg()
{}

//确认
void ICPDlg::on_btn_ok_clicked()
{
	bool ok;
	QString leaf_s = ui.leaf->text();
	double leaf_d = leaf_s.toDouble(&ok);
	if (leaf_d <= 0.0)
	{
		ui.leaf->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.leaf->setStyleSheet("QLineEdit{border:1px solid black }");
	QString NormalRadius_s = ui.NormalRadius->text();
	double NormalRadius_d = NormalRadius_s.toDouble(&ok);
	if (NormalRadius_d <= 0.0)
	{
		ui.NormalRadius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.NormalRadius->setStyleSheet("QLineEdit{border:1px solid black }");
	QString FeatureRadius_s = ui.FeatureRadius->text();
	double FeatureRadius_d = FeatureRadius_s.toDouble(&ok);
	if (FeatureRadius_d <= 0.0)
	{
		ui.FeatureRadius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.FeatureRadius->setStyleSheet("QLineEdit{border:1px solid black }");
	QString RansacNum_s = ui.RansacNum->text();
	int RansacNum_i = RansacNum_s.toInt(&ok);
	if (RansacNum_i <= 0)
	{
		ui.RansacNum->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.RansacNum->setStyleSheet("QLineEdit{border:1px solid black }");
	QString NumberOfSamples_s = ui.NumberOfSamples->text();
	int NumberOfSamples_i = NumberOfSamples_s.toInt(&ok);
	if (NumberOfSamples_i <= 0)
	{
		ui.NumberOfSamples->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.NumberOfSamples->setStyleSheet("QLineEdit{border:1px solid black }");
	QString CorrespondenceRandomness_s = ui.CorrespondenceRandomness->text();
	int CorrespondenceRandomness_i = CorrespondenceRandomness_s.toInt(&ok);
	if (CorrespondenceRandomness_i <= 0)
	{
		ui.CorrespondenceRandomness->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.CorrespondenceRandomness->setStyleSheet("QLineEdit{border:1px solid black }");
	QString SimilarityThreshold_s = ui.SimilarityThreshold->text();
	double SimilarityThreshold_d = SimilarityThreshold_s.toDouble(&ok);
	if (SimilarityThreshold_d < 0.0)
	{
		ui.SimilarityThreshold->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.SimilarityThreshold->setStyleSheet("QLineEdit{border:1px solid black }");
	QString MaxCorrespondenceDistance_s = ui.MaxCorrespondenceDistance->text();
	double MaxCorrespondenceDistance_d = MaxCorrespondenceDistance_s.toDouble(&ok);
	if (MaxCorrespondenceDistance_d < 0.0)
	{
		ui.MaxCorrespondenceDistance->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaxCorrespondenceDistance->setStyleSheet("QLineEdit{border:1px solid black }");
	QString InlierFraction_s = ui.InlierFraction->text();
	double InlierFraction_d = InlierFraction_s.toDouble(&ok);
	if (InlierFraction_d < 0.0)
	{
		ui.InlierFraction->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.InlierFraction->setStyleSheet("QLineEdit{border:1px solid black }");

	emit ICP(leaf_d, NormalRadius_d, FeatureRadius_d, RansacNum_i, NumberOfSamples_i, CorrespondenceRandomness_i, SimilarityThreshold_d, MaxCorrespondenceDistance_d, InlierFraction_d);
}

//取消
void ICPDlg::on_btn_cancel_clicked()
{
	close();
}

// 浏览
void ICPDlg::on_BrowseTarget_clicked()
{
	//获取点云路径
	pathtarget = QFileDialog::getOpenFileName(this, "选择点云文件", ".//", "点云文件(*.txt *.pcd);;所有文件(*.*)");
	ui.PathTarget->setText(pathtarget);
}
// 浏览
void ICPDlg::on_BrowseScene_clicked()
{
	//获取点云路径
	pathscene = QFileDialog::getOpenFileName(this, "选择点云文件", ".//", "点云文件(*.txt *.pcd);;所有文件(*.*)");
	ui.PathScene->setText(pathscene);
}
