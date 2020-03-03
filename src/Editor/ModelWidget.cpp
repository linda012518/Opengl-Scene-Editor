#include "ModelWidget.h"
#include "xml\rapidxml.hpp"
#include "FileSystem.h"
#include "qmimedata.h"
#include "qdrag.h"
#include "qevent.h"

ModelWidget::ModelWidget(QWidget *parent) : QListWidget(parent)
{
	setDragEnabled(true);//���ÿ����϶�
	setViewMode(QListView::IconMode);//��ͼ�����ʽ�г���
	setSpacing(10);//����ͼ��ļ��
	setAcceptDrops(false);//�����Ƿ���Խ����Ϸ�
	setDropIndicatorShown(true);//��ʾ�ϷŵĹ��
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
	fseek(pFile, 0, SEEK_END);//�α�ŵ����
	size_t len = ftell(pFile);//�õ��ļ���С
	fseek(pFile, 0, SEEK_SET);//�α��ٷŵ���ͷ

	char* xml = new char[len + 1];//+1���ļ�������
	fread(xml, 1, len, pFile);
	xml[len] = 0;//���һ����0 ������
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
			item->setIcon(QIcon(bmp));//����ͼ��
			item->setData(Qt::UserRole, QVariant(bmp));//�����û�����  ��ֵ��
			item->setData(Qt::UserRole + 1, attrFile->value());//�����û�����
			//���ÿ��� ��ѡ�� ���϶�
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
	QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));//�õ���Ӧ�û�����
	QString location = item->data(Qt::UserRole + 1).toString();//�õ���Ӧ�û�����

	dataStream << location;//д����

	QMimeData* mimeData = new QMimeData();
	//�������ݸ�ʽ ͼƬ ģ�� ���ӵ�����     
	//���ݸ�ʽ������    ����
	mimeData->setData("image/x-puzzle-piece", itemData);

	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	//����ͼ��λ��
	drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
	drag->setPixmap(pixmap);//����ͼ��

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
