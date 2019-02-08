#ifndef ITEMTREEMODEL_H
#define ITEMTREEMODEL_H

#include <QAbstractItemModel>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

#include "treeviewmodel_global.h"

class TreeItem;

class TREEVIEWMODEL_EXPORT ItemTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit ItemTreeModel(QObject *parent = nullptr);
	~ItemTreeModel();

	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	//bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

	// For rearranging nodes
	Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;
	Qt::DropActions supportedDragActions() const Q_DECL_OVERRIDE;
	QStringList mimeTypes() const Q_DECL_OVERRIDE;
	QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
	bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent);

	bool load(const QString& filename);
	bool load(const QByteArray &jsondata);
	bool save(const QString& output);
	TreeItem * nodeForIndex(const QModelIndex &index) const;
	void removeNode(TreeItem *node);
	
	bool removeNode(const QModelIndex &idx);

	void clear();

private:
	TreeItem *m_rootNode;
	QStringList m_headers;
};
#endif // ITEMTREEMODEL_H
