#include "treeItem.h"

TreeItem::TreeItem(TreeItem *parent) :
	m_parentNode(parent)
{
}

TreeItem::~TreeItem()
{
	qDeleteAll(m_childNodes);
}

void TreeItem::appendChild(TreeItem *item_node)
{
	m_childNodes.append(item_node);
}

void TreeItem::removeChild(int row)
{
	m_childNodes.removeAt(row);
}

// TODO, Fix to deal with QJsonValue::Array
void TreeItem::insertChild(int position, TreeItem *child)
{
	m_childNodes.insert(position, child);
	this->m_value = QString("");
	this->m_type = QJsonValue::Object;
	child->m_parentNode = this;
}

void TreeItem::setKey(const QString &key)
{
	m_key = key;
}

void TreeItem::setValue(const QString &value)
{
	m_value = value;
}

void TreeItem::setType(const QJsonValue::Type &type)
{
	m_type = type;
}

QString TreeItem::getKey() const
{
	return m_key;
}

QString TreeItem::getValue() const
{
	return m_value;
}

QJsonValue::Type TreeItem::getType() const
{
	return m_type;
}

int TreeItem::row() const
{
	if (m_parentNode != nullptr)
	{
		return m_parentNode->m_childNodes.indexOf(const_cast<TreeItem*>(this));
	}

	return 0;
}

int TreeItem::childCount() const
{
	return m_childNodes.count();
}

TreeItem *TreeItem::child(int row) const
{
	return m_childNodes.value(row);
}

TreeItem* TreeItem::parent() const
{
	return m_parentNode;
}