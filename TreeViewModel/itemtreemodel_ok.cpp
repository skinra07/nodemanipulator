#include "itemtreemodel.h"
#include "treeItem.h"
#include "helper.h"

#include <QCoreApplication>
#include <QStringList>
#include <QMimeData>
#include <QDataStream>
#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

static const char s_treeNodeMimeType[] = "application/x-treenode";

ItemTreeModel::ItemTreeModel(QObject *parent) :
	QAbstractItemModel(parent)
{
	m_rootNode = new TreeItem(nullptr);
	m_headers.append("key");
	m_headers.append("value");
}

ItemTreeModel::~ItemTreeModel()
{
	clear();
} 

void ItemTreeModel::clear()
{
	m_headers.clear();

	if (m_rootNode != nullptr)
	{
		m_rootNode = nullptr;
		delete m_rootNode;
	}
}

bool ItemTreeModel::load(const QString& filename)
{
	bool ok = false;

	m_rootNode = Helper::load(filename);

	if (m_rootNode != nullptr)
	{
		beginResetModel();
		ok = true;
		endResetModel();
	}

	return ok;
}

bool ItemTreeModel::load(const QByteArray &jsondata)
{
	bool ok = false;

	beginResetModel();

	m_rootNode = Helper::load(jsondata);

	endResetModel();

	if (m_rootNode != nullptr)
	{
		ok = true;
	}

	return ok;
}

bool ItemTreeModel::save(const QString& output)
{
	return (Helper::save(output, this->m_rootNode));
}

//returns a pointer to the "index"
TreeItem* ItemTreeModel::nodeForIndex(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return m_rootNode;
	}
	else
	{
		return static_cast<TreeItem*>(index.internalPointer());
	}
}

void ItemTreeModel::removeNode(TreeItem *node)
{
	const int row = node->row();
	QModelIndex idx = createIndex(row, 0, node);
	beginRemoveRows(idx.parent(), row, row);
	node->parent()->removeChild(row);
	endRemoveRows();
}

bool ItemTreeModel::removeNode(const QModelIndex &idx)
{
	TreeItem *node = nodeForIndex(idx);
	const int row = node->row();

	bool success = true;

	beginRemoveRows(idx.parent(), row, row);
	node->parent()->removeChild(row);
	endRemoveRows();

	return success;
}

// For this specific Json model, you will always has two column. key and value.
int ItemTreeModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

//returns the mime type
QStringList ItemTreeModel::mimeTypes() const
{
	return QStringList() << s_treeNodeMimeType;
}

//receives a list of model indexes list
QMimeData *ItemTreeModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData;
	QByteArray data; 

	QDataStream stream(&data, QIODevice::WriteOnly);
	QList<TreeItem*> nodes;

	//go through list of indexes, get the TreeItem, and add it to QList
	foreach(const QModelIndex &index, indexes) 
	{
		TreeItem *node = nodeForIndex(index);
		if (!nodes.contains(node))
		{
			nodes << node;
		}
	}

	// Now write all TreeItems to stream as long long. Later we will read it this info as lon long using reinterpret_cast.
	stream << QCoreApplication::applicationPid();

	stream << nodes.count();

	foreach(TreeItem *node, nodes) 
	{
		stream << reinterpret_cast<qlonglong>(node);
	}

	// set the above info as  application/x-treenode type
	mimeData->setData(s_treeNodeMimeType, data);

	return mimeData;
}

//TODO remove reference to senderID.
bool ItemTreeModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	Q_ASSERT(action == Qt::MoveAction);
	Q_UNUSED(column);

	//test if the data type is the good one
	if (!mimeData->hasFormat(s_treeNodeMimeType)) 
	{
		return false;
	}

	// get the data containing TreeItems
	QByteArray data = mimeData->data(s_treeNodeMimeType);

	QDataStream stream(&data, QIODevice::ReadOnly);
	qint64 senderPid;
	stream >> senderPid;

	if (senderPid != QCoreApplication::applicationPid()) 
	{
		qDebug() << " SenderPID is differenet then applicationPid. = " << senderPid << "\n";
		// Let's not cast pointers that come from another process...
		return false;
	}

	TreeItem *parentNode = nodeForIndex(parent);
	Q_ASSERT(parentNode);

	int count;
	stream >> count;

	if (row == -1) 
	{
		// valid index means: drop onto item. I chose that this should insert
		// a child item, because this is the only way to create the first child of an item...
		// This explains why Qt calls it parent: unless you just support replacing, this
		// is really the future parent of the dropped items.
		if (parent.isValid())
		{
			qDebug() << " Here row is -1 with parent being valid";
			row = 0;
		}
		else
		{
			// invalid index means: append at bottom, after last toplevel
			row = rowCount(parent);
			qDebug() << " Here row is -1 with invalid index = " << row << "\n";
		}
	}

	for (int i = 0; i < count; ++i) 
	{
		// Decode data from the QMimeData
		qlonglong nodePtr;
		stream >> nodePtr;

		TreeItem *node = reinterpret_cast<TreeItem *>(nodePtr);

		// Adjust destination row for the case of moving an item
		// within the same parent, to a position further down.
		// Its own removal will reduce the final row number by one.
		if (node->row() < row && parentNode == node->parent())
		{
			--row;
		}

		// Remove from old position
		removeNode(node);

		// Insert at new position
		qDebug() << "Inserting into row = " << row << "\n";
		beginInsertRows(parent, row, row);
		parentNode->insertChild(row, node);
		endInsertRows();
		++row;
	}
	return true;
}

Qt::DropActions ItemTreeModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

Qt::DropActions ItemTreeModel::supportedDragActions() const
{
	return Qt::MoveAction;
}

QVariant ItemTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	TreeItem *item = nodeForIndex(index);

	if (role == Qt::DisplayRole) 
	{
		if (index.column() == 0)
		{
			return QString("%1").arg(item->getKey());
		}

		if (index.column() == 1)
		{
			return QString("%1").arg(item->getValue());
		}
	}
	else if (Qt::EditRole == role) 
	{
		if (index.column() == 1) 
		{
			return QString("%1").arg(item->getValue());
		}
	}

	return QVariant();
	
}

Qt::ItemFlags ItemTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::ItemIsDropEnabled;
	}

	return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant ItemTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return m_headers.value(section);
	}

	return QVariant();
}

QModelIndex ItemTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	TreeItem *parentNode = nodeForIndex(parent);

	TreeItem *childNode = parentNode->child(row);

	if (childNode)
	{
		return createIndex(row, column, childNode);
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex ItemTreeModel::parent(const QModelIndex &index) const
{
	TreeItem *childNode = nodeForIndex(index);

	if (childNode == m_rootNode)
	{
		return QModelIndex();
	}

	TreeItem *parentNode = childNode->parent();

	if (parentNode == m_rootNode)
	{
		return QModelIndex();
	}

	return createIndex(parentNode->row(), 0, parentNode);
}

int ItemTreeModel::rowCount(const QModelIndex &parent) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	TreeItem *parentNode = nodeForIndex(parent);

	return parentNode->childCount();
}

//TODO: DO we need it?
bool ItemTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int col = index.column();

	if (Qt::EditRole == role) 
	{
		if (col == 1) 
		{
			TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

			item->setValue(value.toString());

			emit dataChanged(index, index, { Qt::EditRole });

			return true;
		}
	}

	return false;
}