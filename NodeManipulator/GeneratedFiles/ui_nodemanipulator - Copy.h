/********************************************************************************
** Form generated from reading UI file 'nodemanipulator.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NODEMANIPULATOR_H
#define UI_NODEMANIPULATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NodeManipulatorClass
{
public:
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *RemoveActionNode;
    QAction *actionExit;
    QWidget *centralWidget;
    QTreeView *treeView;
    QMenuBar *menuBar;
    QMenu *FileMenu;
    QMenu *ActionsMenu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *NodeManipulatorClass)
    {
        if (NodeManipulatorClass->objectName().isEmpty())
            NodeManipulatorClass->setObjectName(QString::fromUtf8("NodeManipulatorClass"));
        NodeManipulatorClass->resize(574, 400);
        actionLoad = new QAction(NodeManipulatorClass);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionSave = new QAction(NodeManipulatorClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSaveAs = new QAction(NodeManipulatorClass);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        RemoveActionNode = new QAction(NodeManipulatorClass);
        RemoveActionNode->setObjectName(QString::fromUtf8("RemoveActionNode"));
        actionExit = new QAction(NodeManipulatorClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(NodeManipulatorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(0, 0, 573, 427));
        treeView->setDragEnabled(true);
        treeView->setDragDropMode(QAbstractItemView::DragDrop);
        treeView->setDefaultDropAction(Qt::MoveAction);
        treeView->setAlternatingRowColors(true);
        treeView->setAllColumnsShowFocus(true);
        NodeManipulatorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(NodeManipulatorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 574, 21));
        FileMenu = new QMenu(menuBar);
        FileMenu->setObjectName(QString::fromUtf8("FileMenu"));
        ActionsMenu = new QMenu(menuBar);
        ActionsMenu->setObjectName(QString::fromUtf8("ActionsMenu"));
        NodeManipulatorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(NodeManipulatorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        NodeManipulatorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(NodeManipulatorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        NodeManipulatorClass->setStatusBar(statusBar);

        menuBar->addAction(FileMenu->menuAction());
        menuBar->addAction(ActionsMenu->menuAction());
        FileMenu->addAction(actionLoad);
        FileMenu->addAction(actionSave);
        FileMenu->addAction(actionSaveAs);
        FileMenu->addSeparator();
        FileMenu->addAction(actionExit);
        ActionsMenu->addAction(RemoveActionNode);

        retranslateUi(NodeManipulatorClass);

        QMetaObject::connectSlotsByName(NodeManipulatorClass);
    } // setupUi

    void retranslateUi(QMainWindow *NodeManipulatorClass)
    {
        NodeManipulatorClass->setWindowTitle(QApplication::translate("NodeManipulatorClass", "NodeManipulator", nullptr));
        actionLoad->setText(QApplication::translate("NodeManipulatorClass", "L&oad", nullptr));
        actionSave->setText(QApplication::translate("NodeManipulatorClass", "S&ave", nullptr));
        actionSaveAs->setText(QApplication::translate("NodeManipulatorClass", "Sa&veAs", nullptr));
        RemoveActionNode->setText(QApplication::translate("NodeManipulatorClass", "Remove Node", nullptr));
        actionExit->setText(QApplication::translate("NodeManipulatorClass", "E&xit", nullptr));
        FileMenu->setTitle(QApplication::translate("NodeManipulatorClass", "&File", nullptr));
        ActionsMenu->setTitle(QApplication::translate("NodeManipulatorClass", "&Action", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NodeManipulatorClass: public Ui_NodeManipulatorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NODEMANIPULATOR_H
