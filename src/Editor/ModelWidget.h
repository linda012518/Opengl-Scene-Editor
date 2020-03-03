#pragma once

#include <qlistwidget.h>

class ModelWidget : public QListWidget
{
	Q_OBJECT

public:
	ModelWidget(QWidget *parent = Q_NULLPTR);
	virtual ~ModelWidget();

	virtual void load(const char* xmlfile);

protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);
	void startDrag(Qt::DropActions supportedActions);

};

