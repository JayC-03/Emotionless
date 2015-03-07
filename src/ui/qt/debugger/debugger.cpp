#include "debugger.h"

#include "core/memmap.h"
#include "core/ee/ee.h"
#include "core/main.h"

DisassemblerModel::DisassemblerModel(QObject* parent) : QAbstractListModel(parent), base_address(0), code_size(0), program_counter(0), selection(QModelIndex())
{
}

int DisassemblerModel::columnCount(const QModelIndex& parent) const
{
	return 3;
}

int DisassemblerModel::rowCount(const QModelIndex& parent) const
{
	return code_size;
}

QVariant DisassemblerModel::data(const QModelIndex& index, int role) const
{
	switch(role)
	{
		case Qt::DisplayRole:
		{
			u32 address = base_address + index.row() * 4;
			u32 instr = Memory::Read32(address);
			std::string disasm = disassemble(instr);

			switch(index.column())
			{
				case 0:
				{
					return QString("0x%1").arg((uint)address, 8, 16, QLatin1Char('0'));
					break;
				}
				case 1:
				{
					return QString("0x%1").arg((uint)instr, 8, 16, QLatin1Char('0'));
					break;
				}
				case 2:
				{
					return QString::fromStdString(disasm);
					break;
				}
			}
			break;
		}
		case Qt::BackgroundRole:
		{
			u32 address = base_address + index.row() * 4;
			if(address == program_counter) return QBrush(QColor(0xC0, 0xC0, 0xFF));
			break;
		}
	}

	return QVariant();
}

QModelIndex DisassemblerModel::IndexFromAbsoluteAddress(unsigned int address) const
{
	return index((address - base_address) / 4, 0);
}

void DisassemblerModel::ParseFromAddress(unsigned int address)
{

    // NOTE: A too large value causes lagging when scrolling the disassembly
    const unsigned int chunk_size = 1000*500;

    // If we haven't loaded anything yet, initialize base address to the parameter address
    if (code_size == 0)
        base_address = address;

    // If the new area is already loaded, just continue
    if (base_address + code_size > address + chunk_size && base_address <= address)
        return;

    // Insert rows before currently loaded data
    if (base_address > address)
	{
        unsigned int num_rows = (address - base_address) / 4;

        beginInsertRows(QModelIndex(), 0, num_rows);
        code_size += num_rows;
        base_address = address;

        endInsertRows();
    }

    // Insert rows after currently loaded data
    if (base_address + code_size < address + chunk_size)
	{
        unsigned int num_rows = (base_address + chunk_size - code_size - address) / 4;

        beginInsertRows(QModelIndex(), 0, num_rows);
        code_size += num_rows;
        endInsertRows();
    }

    SetNextInstruction(address);
}

void DisassemblerModel::SetNextInstruction(unsigned int address)
{
	QModelIndex cur_index = IndexFromAbsoluteAddress(program_counter);
	QModelIndex prev_index = IndexFromAbsoluteAddress(address);

	program_counter = address;

	emit dataChanged(cur_index, cur_index);
	emit dataChanged(prev_index, prev_index);
}

std::string DisassemblerModel::disassemble(u32 instr)
{
	return "Unknown";
}

void DisassemblerModel::OnSelectionChanged(const QModelIndex& new_selection)
{
	selection = new_selection;
}

DebuggerWidget::DebuggerWidget(QWidget* parent) : QDockWidget(parent), base_addr(0)
{
	disasm_ui.setupUi(this);

	model = new DisassemblerModel(this);
	disasm_ui.treeView->setModel(model);

	connect(disasm_ui.treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), model, SLOT(OnSelectionChanged(const QModelIndex&)));
}

void DebuggerWidget::Init()
{
	model->ParseFromAddress(EE::ee_state.pc);

	disasm_ui.treeView->resizeColumnToContents(0);
	disasm_ui.treeView->resizeColumnToContents(1);
	disasm_ui.treeView->resizeColumnToContents(2);

	QModelIndex model_index = model->IndexFromAbsoluteAddress(EE::ee_state.pc);
	disasm_ui.treeView->scrollTo(model_index);
	disasm_ui.treeView->selectionModel()->setCurrentIndex(model_index, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
}

void DebuggerWidget::OnStep()
{
	model->SetNextInstruction(EE::ee_state.pc);

	QModelIndex model_index = model->IndexFromAbsoluteAddress(EE::ee_state.pc);
	disasm_ui.treeView->scrollTo(model_index);
	disasm_ui.treeView->selectionModel()->setCurrentIndex(model_index, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
}
