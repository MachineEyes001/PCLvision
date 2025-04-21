#pragma once

#include <QWidget>
#include "ui_VoxelGridDlg.h"
#include <pcl/filters/voxel_grid.h>

class VoxelGridDlg : public QWidget
{
	Q_OBJECT

public:
	VoxelGridDlg(QWidget *parent = nullptr);
	~VoxelGridDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void VoxelGrid(double L, double W, double H, int KeepOrganized);

private:
	Ui::VoxelGridDlgClass ui;
};
