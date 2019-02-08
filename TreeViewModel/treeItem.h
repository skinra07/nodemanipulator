#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QJsonValue>

#include "treeviewmodel_global.h"

class TREEVIEWMODEL_EXPORT TreeItem
{
public:
	explicit TreeItem(TreeItem *parentNode);
	~TreeItem();

	void appendChild(TreeItem* child);
	void removeChild(int row);
	void insertChild(int position, TreeItem* child);
	
	void setKey(const QString& key);
	void setValue(const QString& value);
	void setType(const QJsonValue::Type& type);

	QString getKey() const;
	QString getValue() const;
	QJsonValue::Type getType() const;

	int row() const;
	int childCount() const;

	TreeItem *child(int row) const;
	TreeItem *parent() const;
private:
	QString m_key;
	QString m_value;
	QJsonValue::Type m_type;
	QList<TreeItem*> m_childNodes;
	TreeItem* m_parentNode;
};


#endif // TREEITEM_H