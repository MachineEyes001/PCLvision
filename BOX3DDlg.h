#pragma once

#include <QWidget>
#include "ui_BOX3DDlg.h"
#include <pcl/features/moment_of_inertia_estimation.h>
#include <pcl/surface/convex_hull.h>

class BOX3DDlg : public QWidget
{
	Q_OBJECT

public:
	BOX3DDlg(QWidget *parent = nullptr);
	~BOX3DDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

	void on_BOXtype_currentIndexChanged(int nIndex);

signals:
	void BOX3D(int BOXtype, int show);


private:
	Ui::BOX3DDlgClass ui;
};
