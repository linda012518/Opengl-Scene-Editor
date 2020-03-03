#include "Editor.h"
#include "Transform.h"

#include "qfiledialog.h"

#pragma comment(lib, "qtpropertybrowser_release.lib")
//#pragma comment(lib, "qtpropertybrowser.lib")

enum {
	Node_Pos_X,
	Node_Pos_Y,
	Node_Pos_Z,

	Node_Scale_X,
	Node_Scale_Y,
	Node_Scale_Z,
};

Editor::Editor(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	ui._modelList->load("data/model/modelList.xml");
	ui._glWidget->setTreeWidget(ui._sceneTreeWidget);

	_modelAttrMgr		= new QtVariantPropertyManager(ui._proptertyTree);
	_modelEditFactory	= new QtVariantEditorFactory(ui._proptertyTree);
	ui._proptertyTree->setFactoryForManager(_modelAttrMgr, _modelEditFactory);

	ui._glWidget->setPropertyWidget(ui._proptertyTree, _modelAttrMgr);

	QObject::connect(ui._modelList, &ModelWidget::itemPressed, this, &Editor::slotItemPressed);
	QObject::connect(ui._sceneTreeWidget, &QTreeWidget::itemClicked, this, &Editor::slotItemClicked);
	QObject::connect(_modelAttrMgr, &QtVariantPropertyManager::valueChanged, this, &Editor::slotValueChanged);

	QObject::connect(ui.actionOpen, &QAction::triggered, this, &Editor::slotOpenScene);
	QObject::connect(ui.actionSave, &QAction::triggered, this, &Editor::slotSaveScene);

	QObject::connect(ui.actionPoint, &QAction::triggered, this, &Editor::renderPoint);
	QObject::connect(ui.actionLine, &QAction::triggered, this, &Editor::renderLine);
	QObject::connect(ui.actionFace, &QAction::triggered, this, &Editor::renderFace);
}

void Editor::slotItemPressed(QListWidgetItem* item)
{
	QString file = item->data(Qt::UserRole + 1).toString();

	ui._proptertyTree->clear();//清空一下
	_modelAttrMgr->clear();

	//先通过模型管理创建一个属性
	QtVariantProperty*	prop = _modelAttrMgr->addProperty(QVariant::String, "FileName");
	prop->setValue(file);
	ui._proptertyTree->addProperty(prop);//再加到ui的树里
}
void Editor::slotItemClicked(QTreeWidgetItem *item, int column)
{
	ui._proptertyTree->clear();//清空一下
	_modelAttrMgr->clear();

	lyd::Transform* node = (lyd::Transform*)item->data(0, Qt::UserRole).toLongLong();
	if (node == 0)
	{
		return;
	}

	ui._glWidget->resetSelect(node);

	//先通过模型管理创建一个属性
	QtVariantProperty*	prop = _modelAttrMgr->addProperty(QVariant::String, "Pos");
	QtVariantProperty*	propX = _modelAttrMgr->addProperty(QVariant::Double, "x");
	QtVariantProperty*	propY = _modelAttrMgr->addProperty(QVariant::Double, "y");
	QtVariantProperty*	propZ = _modelAttrMgr->addProperty(QVariant::Double, "z");
	prop->addSubProperty(propX);
	prop->addSubProperty(propY);
	prop->addSubProperty(propZ);
	propX->setValue(node->getPosition().x);
	propY->setValue(node->getPosition().y);
	propZ->setValue(node->getPosition().z);
	propX->setUserData(node);
	propY->setUserData(node);
	propZ->setUserData(node);
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
	scaleX->setValue(node->getScale().x);
	scaleY->setValue(node->getScale().y);
	scaleZ->setValue(node->getScale().z);
	scaleX->setUserData(node);
	scaleY->setUserData(node);
	scaleZ->setUserData(node);
	scaleX->setUser1Data((void*)Node_Scale_X);
	scaleY->setUser1Data((void*)Node_Scale_Y);
	scaleZ->setUser1Data((void*)Node_Scale_Z);

	//prop->setModified(false);

	ui._proptertyTree->clear();//清空一下
	ui._proptertyTree->addProperty(prop);//再加到ui的树里
	ui._proptertyTree->addProperty(scale);//再加到ui的树里

}
void Editor::slotValueChanged(QtProperty *prop, const QVariant &val)
{
	QtVariantProperty*	p = dynamic_cast<QtVariantProperty*>(prop);
	lyd::Transform*		node = (lyd::Transform*)p->getUserData();
	if (p == 0 || node == 0)
	{
		return;
	}

	lyd::float3 trans = node->getPosition();
	lyd::float3 scale = node->getScale();

	switch ((int)p->getUser1Data())
	{
	case Node_Pos_X:
	{
		trans.x = val.toReal();
	}
	break;
	case Node_Pos_Y:
	{
		trans.y = val.toReal();
	}
	break;
	case Node_Pos_Z:
	{
		trans.z = val.toReal();
	}
	break;
	case Node_Scale_X:
	{
		scale.x = val.toReal();
	}
	break;
	case Node_Scale_Y:
	{
		scale.y = val.toReal();
	}
	break;
	case Node_Scale_Z:
	{
		scale.z = val.toReal();
	}
	break;
	}
	node->setPosition(trans);
	node->setScale(scale);
}

void Editor::slotOpenScene()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Scene File");
	ui._glWidget->openScene(fileName.toStdString().c_str());
}
void Editor::slotSaveScene()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save Scene");
	ui._glWidget->saveScene(fileName.toStdString().c_str());
}

void Editor::renderPoint()
{
	ui._glWidget->setRenderMode(0);
}
void Editor::renderLine()
{
	ui._glWidget->setRenderMode(1);
}
void Editor::renderFace()
{
	ui._glWidget->setRenderMode(2);
}

