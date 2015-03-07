#include "register.h"

#include "core/ee/ee.h"

RegisterWidget::RegisterWidget(QWidget* parent) : QDockWidget(parent)
{
	ui.setupUi(this);

	tree = ui.treeWidget;
	tree->addTopLevelItem(gprs = new QTreeWidgetItem(QStringList("Registers")));

	gprs->setExpanded(true);

	for(int i = 0; i < 32; i++)
	{
		QTreeWidgetItem* reg = new QTreeWidgetItem(QStringList(QString("R[%1]").arg(i,2,10,QLatin1Char('0'))));
		gprs->addChild(reg);
	}
}

void RegisterWidget::OnStep()
{
	for(int i = 0; i < 32; i++)
	{
		gprs->child(i)->setText(1, QString("0x%1 %2").arg(EE::ee_state.gpr[i].ud[1],16,16,QLatin1Char('0')).arg(EE::ee_state.gpr[i].ud[0],16,16,QLatin1Char('0')));
	}
}
