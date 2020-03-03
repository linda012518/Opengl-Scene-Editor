#include "ModelWidget.h"
#include "xml\rapidxml.hpp"
#include "FileSystem.h"
#include "qmimedata.h"
#include "qdrag.h"
#include "qevent.h"

ModelWidget::ModelWidget(QWidget *parent) : QListWidget(parent)
{
	setDragEnabled(true);//设置可以拖动
	setViewMode(QListView::IconMode);//以图标的形式列出来
	setSpacing(10);//两个图标的间距
	setAcceptDrops(false);//设置是否可以接收拖放
	setDropIndicatorShown(true);//显示拖放的光标
}

ModelWidget::~ModelWidget()
{
}

void ModelWidget::load(const char* xmlfile)
{
	char filePath[1024];
	lyd::FileSystem::getResourcePath(xmlfile, filePath);

	FILE* pFile = fopen(filePath, "rb");
	if (pFile == 0)
	{
		return;
	}
	fseek(pFile, 0, SEEK_END);//游标放到最后
	size_t len = ftell(pFile);//得到文件大小
	fseek(pFile, 0, SEEK_SET);//游标再放到开头

	char* xml = new char[len + 1];//+1是文件结束符
	fread(xml, 1, len, pFile);
	xml[len] = 0;//最后一个置0 结束符
	fclose(pFile);

	try
	{
		rapidxml::xml_document<> doc;
		doc.parse<0>(xml);

		rapidxml::xml_node<>* root = doc.first_node();
		rapidxml::xml_node<>* model = root->first_node();
		for (; model != 0; model = model->next_sibling())
		{
			rapidxml::xml_attribute<>* attrIcon = model->first_attribute("icon");
			rapidxml::xml_attribute<>* attrFile = model->first_attribute("file");
			rapidxml::xml_attribute<>* attrText = model->first_attribute("name");

			char iconPath[1024];
			lyd::FileSystem::getResourcePath(attrIcon->value(), iconPath);

			QPixmap bmp;
			bool ret = bmp.load(iconPath);

			QListWidgetItem* item = new QListWidgetItem(this);
			item->setIcon(QIcon(bmp));//设置图标
			item->setData(Qt::UserRole, QVariant(bmp));//设置用户数据  键值对
			item->setData(Qt::UserRole + 1, attrFile->value());//设置用户数据
			//设置可用 可选择 可拖动
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
			item->setText(attrText->value());
		}
	}
	catch (...)
	{

	}
	delete[] xml;

}

void ModelWidget::startDrag(Qt::DropActions supportedActions)
{
	QListWidgetItem* item = currentItem();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));//得到对应用户数据
	QString location = item->data(Qt::UserRole + 1).toString();//得到对应用户数据

	dataStream << location;//写入流

	QMimeData* mimeData = new QMimeData();
	//设置数据格式 图片 模型 粒子等数据     
	//数据格式的名称    数据
	mimeData->setData("image/x-puzzle-piece", itemData);

	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	//设置图标位置
	drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
	drag->setPixmap(pixmap);//设置图标

	drag->exec(Qt::MoveAction);
}
void ModelWidget::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}
void ModelWidget::dragMoveEvent(QDragMoveEvent* event)
{
	if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
	{
		event->ignore();
	}
}
void ModelWidget::dropEvent(QDropEvent* event)
{
	event->ignore();
}
