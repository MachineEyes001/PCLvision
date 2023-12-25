#include "DepthMapDlg.h"
#include <QFileDialog>
#include <pcl/visualization/common/float_image_utils.h>//保存深度图像
#include <pcl/io/png_io.h>//保存深度图像
#include <QMessageBox>

DepthMapDlg::DepthMapDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

DepthMapDlg::~DepthMapDlg()
{}

//确认
void DepthMapDlg::on_btn_ok_clicked()
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
	QString X_s = ui.X->text();
	double  X_d = X_s.toDouble(&ok);
	if (X_d < 0)
	{
		ui.X->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.X->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString Y_s = ui.Y->text();
	double Y_d = Y_s.toDouble(&ok);
	if (Y_d < 0)
	{
		ui.Y->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Y->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString Z_s = ui.Z->text();
	double Z_d = Z_s.toDouble(&ok);
	if (Z_d < 0)
	{
		ui.Z->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.Z->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString noiseLevel_s = ui.noiseLevel->text();
	double noiseLevel_d = noiseLevel_s.toDouble(&ok);
	if (noiseLevel_d < 0)
	{
		ui.noiseLevel->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.noiseLevel->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString minRange_s = ui.minRange->text();
	double minRange_d = minRange_s.toDouble(&ok);
	if (minRange_d < 0)
	{
		ui.minRange->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.minRange->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString borderSize_s = ui.borderSize->text();
	int borderSize_i = borderSize_s.toInt(&ok);
	if (borderSize_i < 0)
	{
		ui.borderSize->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.borderSize->setStyleSheet("QLineEdit{border:1px solid black }");
	emit DepthMap(angularResolution_d, maxAngleWidth_d, maxAngleHeight_d, X_d, Y_d, Z_d, noiseLevel_d, minRange_d, borderSize_i);
}

//取消
void DepthMapDlg::on_btn_cancel_clicked()
{
	close();
}

// 浏览
void DepthMapDlg::on_Browse_clicked()
{
	//获取路径
	DepthMappath = QFileDialog::getSaveFileName(this, "选择存储路径", "depthImg.png", "图片(*.png);;所有文件(*.*)");
	ui.Path->setText(DepthMappath);
}
