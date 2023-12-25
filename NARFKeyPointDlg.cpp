#include "NARFKeyPointDlg.h"
#include <QFileDialog>

NARFKeyPointDlg::NARFKeyPointDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

NARFKeyPointDlg::~NARFKeyPointDlg()
{}

//确认
void NARFKeyPointDlg::on_btn_ok_clicked()
{
	bool ok;
	//获取lineedit数据
	QString angularResolution_s = ui.angularResolution->text();
	double angularResolution_d = angularResolution_s.toDouble(&ok);
	if (angularResolution_d < 0)
	{
		ui.angularResolution->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.angularResolution->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString maxAngleWidth_s = ui.maxAngleWidth->text();
	double maxAngleWidth_d = maxAngleWidth_s.toDouble(&ok);
	if (maxAngleWidth_d < 0)
	{
		ui.maxAngleWidth->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.maxAngleWidth->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString maxAngleHeight_s = ui.maxAngleHeight->text();
	double maxAngleHeight_d = maxAngleHeight_s.toDouble(&ok);
	if (maxAngleHeight_d < 0)
	{
		ui.maxAngleHeight->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.maxAngleHeight->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString Support_size_s = ui.Support_size->text();
	double  Support_size_d = Support_size_s.toDouble(&ok);
	if (Support_size_d < 0)
	{
		ui.Support_size->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Support_size->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString Distance_for_additional_points_s = ui.Distance_for_additional_points->text();
	double Distance_for_additional_points_d = Distance_for_additional_points_s.toDouble(&ok);
	if (Distance_for_additional_points_d < 0)
	{
		ui.Distance_for_additional_points->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Distance_for_additional_points->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取QComBox数据
	int add_points_on_straight_edges = ui.Add_points_on_straight_edges->currentIndex();
	int CAMERAorLASER = ui.CAMERAorLASER->currentIndex();
	int unseenSet = ui.UnseenSet->currentIndex();
	//获取lineedit数据
	QString noise_level_s = ui.noiseLevel->text();
	double noise_level_d = noise_level_s.toDouble(&ok);
	if (noise_level_d < 0)
	{
		ui.noiseLevel->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.noiseLevel->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString min_range_s = ui.minRange->text();
	double min_range_d = min_range_s.toDouble(&ok);
	if (min_range_d < 0)
	{
		ui.minRange->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.minRange->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString border_size_s = ui.borderSize->text();
	int border_size_i = border_size_s.toInt(&ok);
	if (border_size_i < 0)
	{
		ui.borderSize->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.borderSize->setStyleSheet("QLineEdit{border:1px solid black }");
	emit NARFKeyPoint(angularResolution_d, maxAngleWidth_d, maxAngleHeight_d, Support_size_d, add_points_on_straight_edges, Distance_for_additional_points_d, CAMERAorLASER, unseenSet, noise_level_d, min_range_d, border_size_i);
}

//取消
void NARFKeyPointDlg::on_btn_cancel_clicked()
{
	close();
}

// 浏览
void NARFKeyPointDlg::on_Browse_clicked()
{
	//获取路径
	_far_ranges_pcd_path = QFileDialog::getSaveFileName(this, "选择远距离深度数据文件", "_far_ranges.pcd", "文件(*.pcd);;所有文件(*.*)");
	ui.Path->setText(_far_ranges_pcd_path);
}