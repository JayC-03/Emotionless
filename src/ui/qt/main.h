#pragma once

#include <QMainWindow>
#include <thread>

#include "ui_main.h"
#include "debugger/debugger.h"
#include "debugger/register.h"
#include "common/common.h"

class EMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	EMainWindow();
	~EMainWindow();

private:
	void BootGame();

private slots:
	void OnStartGame();
	void OnStopGame();
	void OnMenuLoadFile();
	void OnStep();

private:
	Ui::MainWindow ui;

	DebuggerWidget* debuggerWidget;
	RegisterWidget* registerWidget;

	std::string game_fn;
	std::thread* main_thread;
};
