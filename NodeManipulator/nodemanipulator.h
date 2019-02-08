#pragma once


#include <QtWidgets/QMainWindow>

#include "ui_nodemanipulator.h"

class ItemTreeModel;

class NodeManipulator : public QMainWindow
{
	Q_OBJECT

public:
	NodeManipulator(QWidget *parent = nullptr);
	~NodeManipulator();

private slots:
	void load();
	bool save();
	bool saveAs();
	void removeNode();

private:
	void createActions();
	void initalizeModel();
	bool saveFile(const QString &fileName);
	
	Ui::NodeManipulatorClass ui;
	ItemTreeModel* _model;

	QString currentFile;

	void clear();
};
