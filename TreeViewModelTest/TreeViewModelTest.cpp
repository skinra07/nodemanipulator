// TreeViewModelTest.cpp : Defines the entry point for the console application.
//

#include "..\TreeViewModel\treeviewmodel.h"
#include "..\TreeViewModel\\itemtreemodel.h"
#include "..\TreeViewModel\\treeItem.h"

#include <QString>
#include <QItemSelectionModel>
#include <QDebug>

#include <gtest\gtest.h>


class TreeViewModelTest : public ::testing::Test
{
public:
	virtual void SetUp()
	{
		_model = new ItemTreeModel(nullptr);
		loadJson();
	}

	virtual void TearDown() 
	{
		_model = nullptr;
		delete _model;
	}

	void loadJson()
	{
		std::string json = R"({
						"A":
						 {
							"B":"B value",
							"C":"C value"
						 }
						})";

		_status = _model->load(QByteArray::fromStdString(json));
	}

	ItemTreeModel* _model = nullptr;
	bool _status = false;
	bool _skiptest = false;
};


// test if json data is loaded ok.
TEST_F(TreeViewModelTest, loadJson)
{
	if (_skiptest)
		return;

	qDebug() << "Testing loadJson " << "\n";

	ASSERT_TRUE(_status);
}

// verify the Node A is parent
TEST_F(TreeViewModelTest, treeViewModel_parent)
{
	if (_skiptest)
		return;

	qDebug() << "Testing treeViewModel_parent " << "\n";

	TreeItem* parent = _model->nodeForIndex(QModelIndex());
	int childCount = parent->childCount();

	TreeItem* child = parent->child(childCount-1);

	int cmpStatus = QString::compare(child->getKey(), QString("A"));
	ASSERT_EQ(cmpStatus, 0);
}

// Verify that A nodes has 2 children
TEST_F(TreeViewModelTest, treeViewModel_childCount)
{
	if (_skiptest)
		return;

	qDebug() << "Testing treeViewModel_childCount " << "\n";

	QModelIndex parent = _model->index(0, 0, QModelIndex());
	int childCount = _model->rowCount(parent);

	ASSERT_EQ(childCount, 2);
}

// Remove Node C
TEST_F(TreeViewModelTest, removeNode)
{
	if (_skiptest)
		return;

	qDebug() << "Testing removeNode " << "\n";

	QModelIndex parent = _model->index(0, 0, QModelIndex());
	QVariant data_b = _model->index(0, 0, parent).data();
	QVariant data_c = _model->index(1, 0, parent).data();

	// let remove C
	QModelIndex idx_c = _model->index(1, 0, parent);
	_model->removeNode(idx_c);

	// Now verify that it parent has only 1 child.
	int childCount = _model->rowCount(parent);

	ASSERT_EQ(childCount, 1);
}

// This make C as child of B.
TEST_F(TreeViewModelTest, MoveNode)
{
	if (_skiptest)
		return;

	qDebug() << "Testing MoveNode " << "\n";

	QModelIndex parent = _model->index(0, 0, QModelIndex());
	QModelIndex idx_b = _model->index(0, 0, parent);
	QVariant data_b = idx_b.data();

	// let move C
	QModelIndex idx_c = _model->index(1, 0, parent);
	QVariant data_c = idx_c.data();

	//QList<QModelIndex> _list
	QModelIndexList _list;
	_list.append(idx_c);
	QMimeData* mimeData = _model->mimeData(_list);
	_model->dropMimeData(mimeData, Qt::DropAction::MoveAction, -1, -1, idx_b);

	// Now verify if C is child of B.
	QModelIndex idx_ua = _model->index(0, 0, QModelIndex());
	QVariant data_ua = idx_ua.data();
	QModelIndex idx_ub = _model->index(0, 0, idx_ua);
	QVariant data_ub = idx_ub.data();

	// check childcount from B.
	TreeItem* bnode = _model->nodeForIndex(idx_ub);
	ASSERT_EQ(bnode->childCount(), 1);

	QModelIndex idx_uc = _model->index(0, 0, idx_ub);
	QVariant data_uc = idx_uc.data();

	// Check if child is actually 'C' node
	TreeItem* cnode = _model->nodeForIndex(idx_uc);

	int cmpStatus = QString::compare(cnode->getKey(), QString("C"));

	ASSERT_EQ(cmpStatus, 0);

}

int main(int argc, char **argv) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}