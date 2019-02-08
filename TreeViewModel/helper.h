#ifndef ITEMTREEMODELIMPL_H
#define ITEMTREEMODELIMPL_H

#include "treeviewmodel_global.h"

#include <QString>
#include <QJsonValue>

class TreeItem;

class TREEVIEWMODEL_EXPORT Helper
{
public:
	static TreeItem* load(const QString &filename);
	static TreeItem* load(const QByteArray &jsondata);
	static TreeItem* parse(const QJsonValue& value, TreeItem * parent = nullptr);

	static QJsonValue createJson(TreeItem * item);
	static bool save(const QString& output, TreeItem* rootNode);
};
#endif // ITEMTREEMODELIMPL_H
