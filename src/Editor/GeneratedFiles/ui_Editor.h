/********************************************************************************
** Form generated from reading UI file 'Editor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include <qttreepropertybrowser.h>
#include "ModelWidget.h"
#include "OpenglWidget.h"

QT_BEGIN_NAMESPACE

class Ui_EditorClass
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionPoint;
    QAction *actionLine;
    QAction *actionFace;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    OpenglWidget *_glWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    ModelWidget *_modelList;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout;
    QTreeWidget *_sceneTreeWidget;
    QDockWidget *dockWidget_3;
    QtTreePropertyBrowser *_proptertyTree;

    void setupUi(QMainWindow *EditorClass)
    {
        if (EditorClass->objectName().isEmpty())
            EditorClass->setObjectName(QStringLiteral("EditorClass"));
        EditorClass->resize(1090, 788);
        actionOpen = new QAction(EditorClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(EditorClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionPoint = new QAction(EditorClass);
        actionPoint->setObjectName(QStringLiteral("actionPoint"));
        actionLine = new QAction(EditorClass);
        actionLine->setObjectName(QStringLiteral("actionLine"));
        actionFace = new QAction(EditorClass);
        actionFace->setObjectName(QStringLiteral("actionFace"));
        centralWidget = new QWidget(EditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        _glWidget = new OpenglWidget(centralWidget);
        _glWidget->setObjectName(QStringLiteral("_glWidget"));

        gridLayout_2->addWidget(_glWidget, 0, 0, 1, 1);

        EditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1090, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        EditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        EditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(EditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EditorClass->setStatusBar(statusBar);
        dockWidget = new QDockWidget(EditorClass);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(200, 38));
        _modelList = new ModelWidget();
        _modelList->setObjectName(QStringLiteral("_modelList"));
        dockWidget->setWidget(_modelList);
        EditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        dockWidget_2 = new QDockWidget(EditorClass);
        dockWidget_2->setObjectName(QStringLiteral("dockWidget_2"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        gridLayout = new QGridLayout(dockWidgetContents_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        _sceneTreeWidget = new QTreeWidget(dockWidgetContents_2);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        _sceneTreeWidget->setHeaderItem(__qtreewidgetitem);
        _sceneTreeWidget->setObjectName(QStringLiteral("_sceneTreeWidget"));

        gridLayout->addWidget(_sceneTreeWidget, 0, 0, 1, 1);

        dockWidget_2->setWidget(dockWidgetContents_2);
        EditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_2);
        dockWidget_3 = new QDockWidget(EditorClass);
        dockWidget_3->setObjectName(QStringLiteral("dockWidget_3"));
        dockWidget_3->setMinimumSize(QSize(200, 38));
        _proptertyTree = new QtTreePropertyBrowser();
        _proptertyTree->setObjectName(QStringLiteral("_proptertyTree"));
        dockWidget_3->setWidget(_proptertyTree);
        EditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_3);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuView->addAction(actionPoint);
        menuView->addAction(actionLine);
        menuView->addAction(actionFace);

        retranslateUi(EditorClass);

        QMetaObject::connectSlotsByName(EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorClass)
    {
        EditorClass->setWindowTitle(QApplication::translate("EditorClass", "Editor", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("EditorClass", "Open", Q_NULLPTR));
        actionSave->setText(QApplication::translate("EditorClass", "Save", Q_NULLPTR));
        actionPoint->setText(QApplication::translate("EditorClass", "Point", Q_NULLPTR));
        actionLine->setText(QApplication::translate("EditorClass", "Line", Q_NULLPTR));
        actionFace->setText(QApplication::translate("EditorClass", "Face", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("EditorClass", "File", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("EditorClass", "View", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class EditorClass: public Ui_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITOR_H
