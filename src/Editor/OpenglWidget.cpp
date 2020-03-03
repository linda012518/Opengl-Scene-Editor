#include "OpenglWidget.h"
#include "GLContext.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderable.h"
#include "CELLMath.hpp"
#include "glew\glew.h"
#include "xml\rapidxml_print.hpp"

#include "qevent.h"
#include "qmimedata.h"
#include "qtvariantproperty.h"

enum {
	Node_Pos_X,
	Node_Pos_Y,
	Node_Pos_Z,

	Node_Scale_X,
	Node_Scale_Y,
	Node_Scale_Z,
};

OpenglWidget::OpenglWidget(QWidget *parent) : QWidget(parent, Qt::MSWindowsOwnDC)//这个qwidget组件有自己的dc，因为Opengl绘制要有自己的dc
{
	_leftBtnFlag = false;
	_rightBtnFlag = false;
	_middleBtnFlag = false;

	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NoSystemBackground);//窗口都有个背景，不需要了
	setAutoFillBackground(true);
	setAcceptDrops(true);//设置是否可以接收拖放

	_context = new lyd::GLContext();
	_context->setup((HWND)winId(), GetDC((HWND)winId()));
	glewInit();

	initScene();

	QObject::connect(&_renderTimer, &QTimer::timeout, this, &OpenglWidget::render);
	_renderTimer.setInterval(16);
	_renderTimer.start();

	//r摄像机初始化
	_camera->setViewSize(width(), height());
	_camera->perspective(60, (float)width() / (float)height(), 0.1f, 1000.0f);
	_camera->setEye(lyd::float3(100, 100, 100));
	_camera->setTarget(lyd::float3(0, 0, 0));
	_camera->setRight(lyd::float3(1, 0, 0));
	_camera->setUp(lyd::float3(0, 1, 0));
	_camera->update();
}

OpenglWidget::~OpenglWidget()
{
	if (_context != nullptr)
	{
		_context->shutdown();
		delete _context;
		_context = nullptr;
	}
}

void OpenglWidget::dragEnterEvent(QDragEnterEvent* event)
{
	event->accept();
}

void OpenglWidget::dragMoveEvent(QDragMoveEvent* event)
{
	event->accept();
}

void OpenglWidget::dropEvent(QDropEvent* event)
{
	if (!_treeWidget) return;

	QString strFileName;
	QByteArray data = event->mimeData()->data("image/x-puzzle-piece");
	QDataStream dataStream(&data, QIODevice::ReadOnly);

	dataStream >> strFileName;

	lyd::Transform* node = createNode(strFileName.toStdString().c_str(), event->pos().x(), event->pos().y());
	if (!node) return;

	resetSelect();
	_curObj = node;
	_curObj->setFlag(lyd::Transform::State_Select);

	int amount = _treeWidget->topLevelItemCount();

	QTreeWidgetItem* pFind = 0;
	QTreeWidgetItem* pItem = 0;

	for (size_t i = 0; i < amount; i++)
	{
		QTreeWidgetItem*	pTop = _treeWidget->topLevelItem(i);
		QString				text = pTop->text(0);
		if (text == strFileName)
		{
			pFind = pTop;
			break;
		}
	}
	if (pFind)
	{
		pItem = new QTreeWidgetItem(pFind);
		pItem->setData(0, Qt::UserRole, QVariant((qlonglong)node));//保存节点指针到这个节点的用户数据上
		pItem->setText(0, node->getName());
	}
	else
	{
		pFind = new QTreeWidgetItem(_treeWidget);
		pFind->setText(0, strFileName);
		pItem = new QTreeWidgetItem(pFind);
		pItem->setData(0, Qt::UserRole, QVariant((qlonglong)node));//保存节点指针到这个节点的用户数据上
		pItem->setText(0, node->getName());
	}
	event->accept();
}

void OpenglWidget::resizeEvent(QResizeEvent* event)
{
	glViewport(0, 0, width(), height());
	_camera->setViewSize(width(), height());
	_camera->perspective(60, (float)width() / (float)height(), 0.1f, 1000.0f);
}

void OpenglWidget::mousePressEvent(QMouseEvent *event)//按下
{
	switch (event->button())
	{
	case Qt::LeftButton:
		{
			_leftBtnFlag = true;
			_mousePos = QPoint(event->x(), event->y());

			_curObj = pickObject(event->x(), event->y());

			openPropertyNode();
		}
		break;
	case Qt::RightButton:
		{
			_rightBtnFlag = true;
			_mousePos = QPoint(event->x(), event->y());
		}
		break;
	case Qt::MiddleButton:
		{
			_middleBtnFlag = true;
			_mousePos = QPoint(event->x(), event->y());
		}
		break;
	}
}
void OpenglWidget::mouseReleaseEvent(QMouseEvent *event)//抬起
{
	switch (event->button())
	{
	case Qt::LeftButton:
		{
			_leftBtnFlag = false;
		}
		break;
	case Qt::RightButton:
		{
			_rightBtnFlag = false;
		}
		break;
	case Qt::MiddleButton:
		{
			_middleBtnFlag = false;
		}
		break;
	}
}
void OpenglWidget::mouseMoveEvent(QMouseEvent *event)//移动
{
	if (_leftBtnFlag)
	{
		if (!_curObj) return;

		//鼠标按下的射线
		lyd::Ray	ray0 = _camera->createRayFromScreen(_mousePos.x(), _mousePos.y());
		//鼠标当前的射线
		lyd::Ray	ray1 = _camera->createRayFromScreen(event->x(), event->y());

		lyd::float3	pos1 = _camera->calcIntersectPoint(ray0);
		lyd::float3	pos2 = _camera->calcIntersectPoint(ray1);
		_mousePos = QPoint(event->x(), event->y());

		lyd::float3	offset = pos2 - pos1;

		_curObj->setPosition(_curObj->getPosition() + offset);


		QSet<QtProperty*> properties = _modelAttrMgr->properties();
		for (auto itr = properties.begin(); itr != properties.end(); itr++)
		{
			QtProperty * p = *itr;
			QList<QtProperty *>  list = p->subProperties();
			if (list.size() <= 0) continue;
			if (p->propertyName() != "Pos") continue;

			for (auto i = list.begin(); i != list.end(); i++)
			{
				QtVariantProperty * prop = (QtVariantProperty *)*i;
				QString str = prop->propertyName();
				if (str == "x")
				{
					prop->setValue(_curObj->getPosition().x);
					prop->setUser1Data((void*)Node_Pos_X);
				}
				else if (str == "y")
				{
					prop->setValue(_curObj->getPosition().y);
					prop->setUser1Data((void*)Node_Pos_Y);
				}
				else if (str == "z")
				{
					prop->setValue(_curObj->getPosition().z);
					prop->setUser1Data((void*)Node_Pos_Z);
				}
				prop->setUserData(_curObj);
			}
		}

	}
	else if (_rightBtnFlag)
	{
		QPoint	ptCur = QPoint(event->x(), event->y());
		QPoint	offset = ptCur - _mousePos;
		_mousePos = ptCur;
		_camera->rotateViewY(offset.x() * 0.3f);
		_camera->rotateViewX(offset.y() * 0.3f);
	}
	else if (_middleBtnFlag)
	{
		//鼠标按下的射线
		lyd::Ray		ray0 = _camera->createRayFromScreen(_mousePos.x(), _mousePos.y());
		//鼠标当前的射线
		lyd::Ray		ray1 = _camera->createRayFromScreen(event->x(), event->y());

		lyd::float3	pos1 = _camera->calcIntersectPoint(ray0);
		lyd::float3	pos2 = _camera->calcIntersectPoint(ray1);

		_mousePos = QPoint(event->x(), event->y());

		lyd::float3	offset = pos1 - pos2;

		lyd::float3	newEye = _camera->getEye() + offset;
		lyd::float3	newTgt = _camera->getTarget() + offset;

		_camera->setEye(newEye);
		_camera->setTarget(newTgt);
		_camera->update();
	}
}
void OpenglWidget::wheelEvent(QWheelEvent *event)//轮
{
	float dis = length(_camera->_eye - _camera->_target);
	if (event->delta() > 0)
	{
		dis *= 1.1f;
	}
	else
	{
		dis *= 0.9f;
	}
	lyd::float3 newEye = _camera->_target - _camera->getDir() * dis;
	_camera->setEye(newEye);
	_camera->update();
}

void OpenglWidget::render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	lyd::matrix4 mvp = _camera->getProject() * _camera->getView();
	glLoadMatrixf(mvp.data());

	renderScene(_renderMode);

	_context->swapBuffer();
}

QPaintEngine* OpenglWidget::paintEngine() const
{
	return 0;
}

lyd::Transform* OpenglWidget::pickObject(int x, int y)
{
	resetSelect();

	lyd::Ray ray = _camera->createRayFromScreen(x, y);

	typedef std::pair<bool, float> Result;

	for (auto itr = _vector.begin(); itr != _vector.end(); ++itr)
	{
		lyd::Transform*	node = *itr;
		lyd::aabb3d		aabb = node->getAabb();//没变换过的包围盒
		aabb.transform(node->local());
		Result				res = ray.intersects(aabb);
		if (res.first)
		{
			node->setFlag(lyd::Transform::State_Select);
			return node;
		}
	}
	return 0;
}

void OpenglWidget::openPropertyNode()
{
	_property->clear();
	_modelAttrMgr->clear();

	if (!_curObj) return;

	//先通过模型管理创建一个属性
	QtVariantProperty*	prop = _modelAttrMgr->addProperty(QVariant::String, "Pos");
	QtVariantProperty*	propX = _modelAttrMgr->addProperty(QVariant::Double, "x");
	QtVariantProperty*	propY = _modelAttrMgr->addProperty(QVariant::Double, "y");
	QtVariantProperty*	propZ = _modelAttrMgr->addProperty(QVariant::Double, "z");
	prop->addSubProperty(propX);
	prop->addSubProperty(propY);
	prop->addSubProperty(propZ);
	propX->setValue(_curObj->getPosition().x);
	propY->setValue(_curObj->getPosition().y);
	propZ->setValue(_curObj->getPosition().z);
	propX->setUserData(_curObj);
	propY->setUserData(_curObj);
	propZ->setUserData(_curObj);
	propX->setUser1Data((void*)Node_Pos_X);
	propY->setUser1Data((void*)Node_Pos_Y);
	propZ->setUser1Data((void*)Node_Pos_Z);

	QtVariantProperty*	scale = _modelAttrMgr->addProperty(QVariant::String, "Scale");
	QtVariantProperty*	scaleX = _modelAttrMgr->addProperty(QVariant::Double, "x");
	QtVariantProperty*	scaleY = _modelAttrMgr->addProperty(QVariant::Double, "y");
	QtVariantProperty*	scaleZ = _modelAttrMgr->addProperty(QVariant::Double, "z");
	scale->addSubProperty(scaleX);
	scale->addSubProperty(scaleY);
	scale->addSubProperty(scaleZ);
	scaleX->setValue(_curObj->getScale().x);
	scaleY->setValue(_curObj->getScale().y);
	scaleZ->setValue(_curObj->getScale().z);
	scaleX->setUserData(_curObj);
	scaleY->setUserData(_curObj);
	scaleZ->setUserData(_curObj);
	scaleX->setUser1Data((void*)Node_Scale_X);
	scaleY->setUser1Data((void*)Node_Scale_Y);
	scaleZ->setUser1Data((void*)Node_Scale_Z);

	//prop->setModified(false);

	_property->clear();//清空一下
	_property->addProperty(prop);//再加到ui的树里
	_property->addProperty(scale);//再加到ui的树里

}

void OpenglWidget::setTreeWidget(QTreeWidget* widget)
{
	_treeWidget = widget;
}
void OpenglWidget::setPropertyWidget(QtTreePropertyBrowser* widget, QtVariantPropertyManager* mgr)
{
	_property		= widget;
	_modelAttrMgr	= mgr;
}

void OpenglWidget::resetSelect(lyd::Transform* cur)
{
	if (_curObj)
	{
		_curObj->removeFlag(lyd::Transform::State_Select);
	}
	if (cur)
	{
		_curObj = cur;
		_curObj->setFlag(lyd::Transform::State_Select);
	}
	
}

bool OpenglWidget::openScene(const char* filename)
{
	clear();
	_treeWidget->clear();

	FILE*	pFile = fopen(filename, "rb");
	if (pFile == 0)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	size_t	size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	char*	xml = new char[size + 1];
	fread(xml, 1, size, pFile);
	xml[size] = 0;
	fclose(pFile);

	try
	{
		rapidxml::xml_document<>    doc;
		doc.parse<0>(xml);
		rapidxml::xml_node<>*   root = doc.first_node();
		rapidxml::xml_node<>*   xmlNode = root->first_node();

		rapidxml::xml_attribute<>*  attrUp = root->first_attribute("up");
		rapidxml::xml_attribute<>*  attrEye = root->first_attribute("eye");
		rapidxml::xml_attribute<>*  attrRight = root->first_attribute("right");
		rapidxml::xml_attribute<>*  attrTarget = root->first_attribute("target");

		lyd::float3    up;
		lyd::float3    right;
		lyd::float3    eye;
		lyd::float3    target;

		sscanf(attrUp->value(), "%f %f %f", &up.x, &up.y, &up.z);
		sscanf(attrRight->value(), "%f %f %f", &right.x, &right.y, &right.z);
		sscanf(attrEye->value(), "%f %f %f", &eye.x, &eye.y, &eye.z);
		sscanf(attrTarget->value(), "%f %f %f", &target.x, &target.y, &target.z);

		_camera->setUp(up);
		_camera->setEye(eye);
		_camera->setRight(right);
		_camera->setTarget(target);
		_camera->update();

		for (; xmlNode; xmlNode = xmlNode->next_sibling())
		{
			createSceneNode(xmlNode);
		}
	}
	catch (...)
	{

	}
	return true;

}
bool OpenglWidget::saveScene(const char* filename)
{
	rapidxml::xml_document<>    doc;
	rapidxml::xml_node<>*       version = doc.allocate_node(rapidxml::node_pi, "xml version='1.0' encoding='utf-8'", 0);
	doc.append_node(version);

	rapidxml::xml_node<>*       root = doc.allocate_node(rapidxml::node_element, "root", 0);
	doc.append_node(root);

	char						szBuf[1024] = { 0 };
	sprintf(szBuf, "%f %f %f", _camera->_up.x, _camera->_up.y, _camera->_up.z);
	root->append_attribute(doc.allocate_attribute("up", doc.allocate_string(szBuf)));

	sprintf(szBuf, "%f %f %f", _camera->_eye.x, _camera->_eye.y, _camera->_eye.z);
	root->append_attribute(doc.allocate_attribute("eye", doc.allocate_string(szBuf)));

	sprintf(szBuf, "%f %f %f", _camera->_right.x, _camera->_right.y, _camera->_right.z);
	root->append_attribute(doc.allocate_attribute("right", doc.allocate_string(szBuf)));

	sprintf(szBuf, "%f %f %f", _camera->_target.x, _camera->_target.y, _camera->_target.z);
	root->append_attribute(doc.allocate_attribute("target", doc.allocate_string(szBuf)));

	for (auto itr = _vector.begin(); itr != _vector.end(); ++itr)
	{
		lyd::Transform*         pSceneNode = *itr;
		rapidxml::xml_node<>*   xmlNode = doc.allocate_node(rapidxml::node_element, "node", 0);
		lyd::float3				pos = pSceneNode->getPosition();
		lyd::float3				scale = pSceneNode->getScale();
		lyd::quatr				rot = pSceneNode->getQuat();

		sprintf(szBuf, "%f %f %f", pos.x, pos.y, pos.z);
		//这里szBuf是栈内存，所以要用doc.allocate_string(szBuf))转成堆内存
		xmlNode->append_attribute(doc.allocate_attribute("pos", doc.allocate_string(szBuf)));

		sprintf(szBuf, "%f %f %f", scale.x, scale.y, scale.z);
		xmlNode->append_attribute(doc.allocate_attribute("scale", doc.allocate_string(szBuf)));

		sprintf(szBuf, "%f %f %f %f", rot.x, rot.y, rot.z, rot.w);
		xmlNode->append_attribute(doc.allocate_attribute("rot", doc.allocate_string(szBuf)));


		xmlNode->append_attribute(doc.allocate_attribute("name", pSceneNode->getName()));

		lyd::Renderable* model = pSceneNode->getAttack();
		if (model)
		{
			xmlNode->append_attribute(doc.allocate_attribute("model", model->getFileName()));
		}
		else
		{
			xmlNode->append_attribute(doc.allocate_attribute("model", ""));
		}

		root->append_node(xmlNode);
	}

	std::string xml;
	rapidxml::print(std::back_inserter(xml), doc);

	FILE*   pFile = fopen(filename, "wb+");
	fwrite(xml.c_str(), xml.size(), 1, pFile);
	fclose(pFile);

	return  true;

}

void OpenglWidget::createSceneNode(rapidxml::xml_node<>* xmlNode)
{
	rapidxml::xml_attribute<>*  attrFile = xmlNode->first_attribute("model");
	rapidxml::xml_attribute<>*  attrPos = xmlNode->first_attribute("pos");
	rapidxml::xml_attribute<>*  attrScale = xmlNode->first_attribute("scale");
	rapidxml::xml_attribute<>*  attrRot = xmlNode->first_attribute("rot");
	rapidxml::xml_attribute<>*  attrName = xmlNode->first_attribute("name");

	lyd::float3    pos;
	lyd::float3    scale;
	lyd::quatr     quats;

	sscanf(attrPos->value(), "%f %f %f", &pos.x, &pos.y, &pos.z);
	sscanf(attrScale->value(), "%f %f %f", &scale.x, &scale.y, &scale.z);
	sscanf(attrRot->value(), "%f %f %f %f", &quats.x, &quats.y, &quats.z, &quats.w);

	lyd::Transform* node = createNode(attrFile->value(), 0, 0);
	node->setPosition(pos);
	node->setScale(scale);
	node->setQuat(quats);
	node->setName(attrName->value());

	int             iCnt = _treeWidget->topLevelItemCount();
	QTreeWidgetItem*pFind = 0;
	QTreeWidgetItem*pItem = 0;

	for (int i = 0; i < iCnt; ++i)
	{
		QTreeWidgetItem*    pTop = _treeWidget->topLevelItem(i);
		QString             text = pTop->text(0);
		if (text == attrFile->value())
		{
			pFind = pTop;
			break;
		}
	}

	if (node == 0)
	{
		return;
	}
	if (pFind)
	{
		pItem = new QTreeWidgetItem(pFind);
		pItem->setData(0, Qt::UserRole, QVariant((qlonglong)node));
		pItem->setText(0, node->getName());
	}
	else
	{
		pFind = new QTreeWidgetItem(_treeWidget);
		pFind->setText(0, attrFile->value());
		pItem = new QTreeWidgetItem(pFind);
		pItem->setData(0, Qt::UserRole, QVariant((qlonglong)node));
		pItem->setText(0, node->getName());
	}

}

void OpenglWidget::setRenderMode(char mode)
{
	_renderMode = mode;
}