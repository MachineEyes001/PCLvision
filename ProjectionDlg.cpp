#include "ProjectionDlg.h"

ProjectionDlg::ProjectionDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ProjectionDlg::~ProjectionDlg()
{}

//确认
void ProjectionDlg::on_btn_ok_clicked()
{
	int KeepOrganized = ui.KeepOrganized->currentIndex();
	int dir = ui.Plane->currentIndex();
	emit Projection(dir, KeepOrganized);
}

//取消
void ProjectionDlg::on_btn_cancel_clicked()
{
	close();
}
