#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Editor.h"
#include "qtvariantproperty.h"

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    Editor(QWidget *parent = Q_NULLPTR);

public slots:
	void slotItemPressed(QListWidgetItem* item);
	void slotItemClicked(QTreeWidgetItem *item, int column);
	void slotValueChanged(QtProperty *prop, const QVariant &val);

	void slotOpenScene();
	void slotSaveScene();

	void renderPoint();
	void renderLine();
	void renderFace();

private:
    Ui::EditorClass ui;

	QtVariantPropertyManager*	_modelAttrMgr;
	QtVariantEditorFactory*		_modelEditFactory;
};
