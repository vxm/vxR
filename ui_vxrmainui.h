/********************************************************************************
** Form generated from reading UI file 'vxrmainui.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VXRMAINUI_H
#define UI_VXRMAINUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_vxRMainUI
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QWidget *centralWidget;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuMain_Menu;
    QMenu *menuEdit;
    QMenu *menuOptions;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *vxRMainUI)
    {
        if (vxRMainUI->objectName().isEmpty())
            vxRMainUI->setObjectName(QStringLiteral("vxRMainUI"));
        vxRMainUI->resize(400, 300);
        actionOpen = new QAction(vxRMainUI);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(vxRMainUI);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_As = new QAction(vxRMainUI);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        centralWidget = new QWidget(vxRMainUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 10, 381, 141));
        vxRMainUI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(vxRMainUI);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 20));
        menuMain_Menu = new QMenu(menuBar);
        menuMain_Menu->setObjectName(QStringLiteral("menuMain_Menu"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        vxRMainUI->setMenuBar(menuBar);
        mainToolBar = new QToolBar(vxRMainUI);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        vxRMainUI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(vxRMainUI);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        vxRMainUI->setStatusBar(statusBar);

        menuBar->addAction(menuMain_Menu->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuMain_Menu->addAction(actionOpen);
        menuMain_Menu->addAction(actionSave);
        menuMain_Menu->addAction(actionSave_As);

        retranslateUi(vxRMainUI);

        QMetaObject::connectSlotsByName(vxRMainUI);
    } // setupUi

    void retranslateUi(QMainWindow *vxRMainUI)
    {
        vxRMainUI->setWindowTitle(QApplication::translate("vxRMainUI", "vxRMainUI", 0));
        actionOpen->setText(QApplication::translate("vxRMainUI", "Open", 0));
        actionSave->setText(QApplication::translate("vxRMainUI", "Save", 0));
        actionSave_As->setText(QApplication::translate("vxRMainUI", "Save As...", 0));
        menuMain_Menu->setTitle(QApplication::translate("vxRMainUI", "Main Menu", 0));
        menuEdit->setTitle(QApplication::translate("vxRMainUI", "Edit", 0));
        menuOptions->setTitle(QApplication::translate("vxRMainUI", "Options", 0));
        menuHelp->setTitle(QApplication::translate("vxRMainUI", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class vxRMainUI: public Ui_vxRMainUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VXRMAINUI_H
