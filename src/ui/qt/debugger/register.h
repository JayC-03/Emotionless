#pragma once

#include <QDockWidget>
#include <QTreeWidgetItem>

#include "ui_register.h"

class RegisterWidget : public QDockWidget
{
	Q_OBJECT

public:
	RegisterWidget(QWidget* parent = NULL);

	void OnStep();

private:
	Ui::Registers ui;

	QTreeWidget* tree;

	QTreeWidgetItem* gprs;
};
