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
};


// test if json data is loaded ok.
TEST_F(TreeViewModelTest, loadJson)
{
	ASSERT_TRUE(_status);
}

// verify the Node A is parent
TEST_F(TreeViewModelTest, treeViewModel_parent)
{
	TreeItem* parent = _model->nodeForIndex(QModelIndex());
	int childCount = parent->childCount();

	TreeItem* child = parent->child(childCount-1);

	int cmpStatus = QString::compare(child->getKey(), QString("A"));
	ASSERT_EQ(cmpStatus, 0);
}

// Verify that A nodes has 2 children
TEST_F(TreeViewModelTest, treeViewModel_childCount)
{
	QModelIndex parent = _model->index(0, 0, QModelIndex());
	int childCount = _model->rowCount(parent);

	ASSERT_EQ(childCount, 2);
}

// Remove Node C
TEST_F(TreeViewModelTest, removeNode)
{
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

int main(int argc, char **argv) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}