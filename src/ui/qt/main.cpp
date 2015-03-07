#include <QtGui>
#include <QFileDialog>
#include "main.h"

#include <unistd.h>

#include "common/common.h"
#include "core/main.h"

EMainWindow::EMainWindow()
{
	ui.setupUi(this);
	statusBar()->hide();

	debuggerWidget = new DebuggerWidget(this);
	addDockWidget(Qt::RightDockWidgetArea, debuggerWidget);

	registerWidget = new RegisterWidget(this);
	addDockWidget(Qt::RightDockWidgetArea, registerWidget);

	connect(ui.action_Load_File, SIGNAL(triggered()), this, SLOT(OnMenuLoadFile()));
	connect(ui.action_Start, SIGNAL(triggered()), this, SLOT(OnStartGame()));
	connect(ui.action_Stop, SIGNAL(triggered()), this, SLOT(OnStopGame()));
	connect(ui.button_step, SIGNAL(clicked()), this, SLOT(OnStep()));
}

EMainWindow::~EMainWindow()
{
}

void EMainWindow::BootGame()
{
	OnStartGame();
}

void EMainWindow::OnMenuLoadFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load File"), QString(), tr("All files (*.*)"));
	game_fn = filename.toLatin1().data();
	BootGame();
}

void EMainWindow::OnStartGame()
{
	main_quit = false;
	main_break = true;
	main_thread = new std::thread(main_loop, game_fn);

	ui.action_Start->setEnabled(false);
	ui.action_Stop->setEnabled(true);

	usleep(50000);

	debuggerWidget->Init();
}

void EMainWindow::OnStopGame()
{
	main_quit = true;
	main_thread->join();
	ui.action_Start->setEnabled(false);
	ui.action_Stop->setEnabled(true);
}

void EMainWindow::OnStep()
{
	main_break_steps = 1;
	debuggerWidget->OnStep();
	registerWidget->OnStep();
}

#ifdef main
#undef main
#endif

int main(int ac, char** av)
{
	QApplication app(ac, av);
	EMainWindow main_window;
	
	main_window.show();

	return app.exec();	
}
