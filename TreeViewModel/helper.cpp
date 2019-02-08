#include "helper.h"
#include "treeitem.h"

#include <QObject>
#include <QFile>
#include <QJsonDocument>

#include <QJsonArray>
#include <QJsonObject>


TreeItem* Helper::load(const QByteArray &jsondata)
{
	bool ok = true;

	auto const& jsondoc = QJsonDocument::fromJson(jsondata);

	TreeItem* rootItem = nullptr;

	if (!jsondoc.isNull())
	{
		if (jsondoc.isArray()) {
			rootItem = Helper::parse(QJsonValue(jsondoc.array()));
			rootItem->setType(QJsonValue::Array);

		}
		else 
		{
			rootItem = Helper::parse(QJsonValue(jsondoc.object()));
			rootItem->setType(QJsonValue::Object);
		}
	}
	
	return rootItem;
}

TreeItem* Helper::load(const QString& filename)
{
	
	TreeItem* rootItem = nullptr;

	QFile file(filename);

	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray data = qobject_cast<QIODevice*>(&file)->readAll();
		rootItem = load(data);
		file.close();
	}
	
	return rootItem;
}

TreeItem* Helper::parse(const QJsonValue& value, TreeItem* parent)
{
	TreeItem * rootItem = new TreeItem(parent);
	rootItem->setKey("root");

	if (value.isObject())
	{
		//Get all QJsonValue children
		for (QString key : value.toObject().keys()) {
			QJsonValue v = value.toObject().value(key);
			TreeItem* child = Helper::parse(v, rootItem);
			child->setKey(key);
			child->setType(v.type());
			rootItem->appendChild(child);
		}
	}

	else if (value.isArray())
	{
		//Get all QJsonValue children
		int index = 0;
		for (QJsonValue v : value.toArray()) {

			TreeItem * child = Helper::parse(v, rootItem);
			child->setKey(QString::number(index));
			child->setType(v.type());
			rootItem->appendChild(child);
			++index;
		}
	}
	else
	{
		rootItem->setValue(value.toVariant().toString());
		rootItem->setType(value.type());
	}

	return rootItem;
}

QJsonValue Helper::createJson(TreeItem * item) 
{
	auto type = item->getType();

	int  child_cnt = item->childCount();

	if (QJsonValue::Object == type) 
	{
		QJsonObject jo;
		for (int i = 0; i < child_cnt; ++i)
		{
			auto ch = item->child(i);
			auto key = ch->getKey();
			jo.insert(key, createJson(ch));
		}
		return  jo;
	}
	else if (QJsonValue::Array == type) 
	{
		QJsonArray arr;
		for (int i = 0; i < child_cnt; ++i)
		{
			auto ch = item->child(i);
			arr.append(createJson(ch));
		}
		return arr;
	}
	else 
	{
		QJsonValue val(item->getValue());
		return val;
	}
}

bool Helper::save(const QString& output, TreeItem* rootNode)
{
	bool ok = true;

	if (rootNode == nullptr)
	{
		ok = false;
		return ok;
	}

	auto json = createJson(rootNode);
	QJsonDocument doc;

	if (json.isObject()) {
		doc = QJsonDocument(json.toObject());
	}
	else {
		doc = QJsonDocument(json.toArray());
	}

	QFile jsonfile(output);
	if (!jsonfile.open(QFile::WriteOnly))
	{
		ok = false;
	}
	{
		jsonfile.write(doc.toJson());
	}
	return ok;
}