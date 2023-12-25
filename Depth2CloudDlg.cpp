#include "Depth2CloudDlg.h"
#include <QFileDialog>

Depth2CloudDlg::Depth2CloudDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Depth2CloudDlg::~Depth2CloudDlg()
{}

//确认
void Depth2CloudDlg::on_btn_ok_clicked()
{
	emit Depth2Cloud();
}

//取消
void Depth2CloudDlg::on_btn_cancel_clicked()
{
	close();
}

void Depth2CloudDlg::on_BrowseDepth_clicked()
{
	//获取深度图路径
	pathDepth = QFileDialog::getOpenFileName(this, "选择深度图", ".//", "文件(*.png);;所有文件(*.*)");
	ui.pathDepth->setText(pathDepth);
}
void Depth2CloudDlg::on_BrowseRGB_clicked()
{
	//获取彩色图路径
	pathRGB = QFileDialog::getOpenFileName(this, "选择彩色图", ".//", "文件(*.bmp);;所有文件(*.*)");
	ui.pathRGB->setText(pathRGB);
}
void Depth2CloudDlg::on_BrowseCameraData_clicked()
{
	//获取相机参数文件路径
	pathCameraData = QFileDialog::getOpenFileName(this, "选择相机参数文件", ".//", "文件(*.xml);;所有文件(*.*)");
	ui.pathCameraData->setText(pathCameraData);
}
