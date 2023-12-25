#include "BOX3DDlg.h"

BOX3DDlg::BOX3DDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.show->setEnabled(false);
}

BOX3DDlg::~BOX3DDlg()
{}
void BOX3DDlg::on_BOXtype_currentIndexChanged(int nIndex)
{
	if (nIndex == 3)
	{
		ui.show->setEnabled(true);
	}
	else{ ui.show->setEnabled(false); }
}
//确认
void BOX3DDlg::on_btn_ok_clicked()
{
	int BOXtype = ui.BOXtype->currentIndex();
	int show = ui.show->currentIndex();
	
	emit  BOX3D(BOXtype, show);
}
//取消
void BOX3DDlg::on_btn_cancel_clicked()
{
	close();
}


