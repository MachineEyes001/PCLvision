#pragma once

#include <QWidget>
#include "ui_PolygonDlg.h"
#include <pcl/surface/concave_hull.h>                 //������������ඨ��ͷ�ļ�

class PolygonDlg : public QWidget
{
	Q_OBJECT

public:
	PolygonDlg(QWidget *parent = nullptr);
	~PolygonDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void Polygon(double Alpha);

private:
	Ui::PolygonDlgClass ui;
};
