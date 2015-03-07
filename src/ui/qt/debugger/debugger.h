#pragma once

#include <QAbstractListModel>
#include <QDockWidget>
#include <string>

#include "ui_disassembler.h"

#include "register.h"

#include "common/common.h"

class QAction;

class DisassemblerModel : public QAbstractListModel
{
	Q_OBJECT

public:
	DisassemblerModel(QObject* parent);
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	QModelIndex IndexFromAbsoluteAddress(unsigned int address) const;
	
	std::string disassemble(u32 instr);

public slots:
	void ParseFromAddress(unsigned int address);
	void OnSelectionChanged(const QModelIndex&);
	void SetNextInstruction(unsigned int address);

private:

	u32 base_address;
	u32 code_size;
	u32 program_counter;

	QModelIndex selection;
};

class DebuggerWidget : public QDockWidget
{
	Q_OBJECT

public:
	DebuggerWidget(QWidget* parent);

	void Init();

	void OnStep();

private:
	Ui::DockWidget disasm_ui;

	DisassemblerModel* model;

	u32 base_addr;
};
