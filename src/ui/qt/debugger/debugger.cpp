#include "debugger.h"

#include "common/breakpoint.h"

#include "core/memmap.h"
#include "core/ee/ee.h"
#include "core/ee/ee_inst.h"
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
			QString disasm = disassemble(address, instr);

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
					return disasm;
					break;
				}
			}
			break;
		}
		case Qt::BackgroundRole:
		{
			u32 address = base_address + index.row() * 4;
			if(address == program_counter) return QBrush(QColor(0xC0, 0xC0, 0xFF));
			else
			{
				if(address & 4) return QBrush(QColor(0x70,0x70,0x70));
				else return QBrush(QColor(0x00,0x00,0x00));
			}
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

void DisassemblerModel::OnSetOrUnsetBreakpoint()
{
	if (!selection.isValid()) return;

	unsigned int address = base_address + selection.row() * 4;

	bool status = false; //false for nothing, true for erased

	for(int i = 0; i < breakpoints.size(); i++)
	{
		if(breakpoints[i].addr == address)
		{
			breakpoints.erase(breakpoints.begin() + i);
			status = true;
		}
	}

	if(!status)
	{
		breakpoint_t temp = {address, true};
		breakpoints.push_back(temp);
	}

	emit dataChanged(selection, selection);
}

QString DisassemblerModel::disassemble(u32 addr, u32 instr)
{
	ee_inst instr_code;
	instr_code.hex = instr;

	switch(instr_code.opcd)
	{
		case 0:
		{
			switch(instr_code.funct)
			{
				case 0:
				{
					return QString("sll r%1, r%2, %3").arg((uint)instr_code.RD,2,10,QLatin1Char('0')).arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.SA,2,10,QLatin1Char('0'));
					break;
				}
				case 43:
				{
					return QString("ori r%1, r%2, r%3").arg((uint)instr_code.RD,2,10,QLatin1Char('0')).arg((uint)instr_code.RS,2,10,QLatin1Char('0')).arg((uint)instr_code.RT,2,10,QLatin1Char('0'));
					break;
				}
			}
			break;
		}
		case 2:
		{
			u32 jmp_addr = ((addr + 4) & 0xf0000000) | (instr_code.instr_index << 2);
			return QString("j %1").arg((uint)jmp_addr,8,16,QLatin1Char('0'));
			break;
		}
		case 3:
		{
			u32 jmp_addr = ((addr + 4) & 0xf0000000) | (instr_code.instr_index << 2);
			return QString("jal %1").arg((uint)jmp_addr,8,16,QLatin1Char('0'));
			break;
		}
		case 4:
		{
			u32 branch_addr = (addr + 4) + (s32)(instr_code.SIMM_16 << 2);
			return QString("beq r%1, r%2, %3").arg((uint)instr_code.RS,2,10,QLatin1Char('0')).arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)branch_addr,8,16,QLatin1Char('0'));
			break;
		}
		case 5:
		{
			u32 branch_addr = (addr + 4) + (s32)(instr_code.SIMM_16 << 2);
			return QString("bne r%1, r%2, %3").arg((uint)instr_code.RS,2,10,QLatin1Char('0')).arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)branch_addr,8,16,QLatin1Char('0'));
			break;
		}
		case 9:
		{
			return QString("addiu r%1, r%2, %3").arg((uint)instr_code.RS,2,10,QLatin1Char('0')).arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.SIMM_16,4,16,QLatin1Char('0'));
			break;
		}
		case 10:
		{
			return QString("slti r%1, r%2, %3").arg((uint)instr_code.RS,2,10,QLatin1Char('0')).arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.SIMM_16,4,16,QLatin1Char('0'));
			break;
		}
		case 11:
		{
			return QString("sltiu r%1, r%2, %3").arg((uint)instr_code.RS,2,10,QLatin1Char('0')).arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.SIMM_16,4,16,QLatin1Char('0'));
			break;
		}
		case 12:
		{
			return QString("daddi r%1, r%2, %3").arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.RS,2,10,QLatin1Char('0')).arg((uint)instr_code.SIMM_16,4,16,QLatin1Char('0'));
			break;
		}
		case 13:
		{
			return QString("ori r%1, r%2, %3").arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.RS,2,10,QLatin1Char('0')).arg((uint)instr_code.SIMM_16,4,16,QLatin1Char('0'));
			break;
		}
		case 15:
		{
			return QString("lui r%1, %2").arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.SIMM_16,4,16,QLatin1Char('0'));
			break;
		}
		case 31:
		{
			return QString("sq r%1, %2(r%3)").arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.SIMM_16,4,16,QLatin1Char('0')).arg((uint)instr_code.RS,2,10,QLatin1Char('0'));
			break;
		}
		case 43:
		{
			return QString("sw r%1, %2(r%3)").arg((uint)instr_code.RT,2,10,QLatin1Char('0')).arg((uint)instr_code.SIMM_16,4,16,QLatin1Char('0')).arg((uint)instr_code.RS,2,10,QLatin1Char('0'));
			break;
		}
	}

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

	connect(disasm_ui.button_breakpoint, SIGNAL(clicked()), model, SLOT(OnSetOrUnsetBreakpoint()));

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
