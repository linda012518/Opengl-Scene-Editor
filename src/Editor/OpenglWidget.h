#pragma once

#include <QWidget>
#include <qtimer.h>
#include <qtreewidget.h>
#include "Scene.h"
#include "qttreepropertybrowser.h"
#include "qtvariantproperty.h"
#include "xml\rapidxml.hpp"

namespace lyd
{
	class GLContext;
	class Transform;
}

class OpenglWidget : public QWidget, public lyd::Scene
{
	Q_OBJECT

public:
	OpenglWidget(QWidget *parent = Q_NULLPTR);
	~OpenglWidget();

	QPaintEngine *paintEngine() const Q_DECL_OVERRIDE;

	lyd::Transform* pickObject(int x, int y);

	void setTreeWidget(QTreeWidget* widget);
	void setPropertyWidget(QtTreePropertyBrowser* widget, QtVariantPropertyManager* mgr);

	void resetSelect(lyd::Transform* cur = nullptr);

	bool openScene(const char* filename);
	bool saveScene(const char* filename);

	void setRenderMode(char mode);

protected:
	virtual void dragEnterEvent(QDragEnterEvent* event);

	virtual void dragMoveEvent(QDragMoveEvent* event);

	virtual void dropEvent(QDropEvent* event);

	virtual void resizeEvent(QResizeEvent* event);

	virtual void mousePressEvent(QMouseEvent *event);//按下
	virtual void mouseReleaseEvent(QMouseEvent *event);//抬起
	virtual void mouseMoveEvent(QMouseEvent *event);//移动
	virtual void wheelEvent(QWheelEvent *event);//轮

	void createSceneNode(rapidxml::xml_node<>* xmlNode);

	void openPropertyNode();

public slots:
	void render();


private:
	QtTreePropertyBrowser*		_property = nullptr;
	QtVariantPropertyManager*	_modelAttrMgr = nullptr;
	lyd::GLContext*				_context = nullptr;
	lyd::Transform*				_curObj = nullptr;
	QTreeWidget*				_treeWidget = nullptr;
	QTimer						_renderTimer;
	QPoint						_mousePos;
	char						_renderMode = 2;
	bool						_leftBtnFlag;
	bool						_rightBtnFlag;
	bool						_middleBtnFlag;
};
