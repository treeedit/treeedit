#include <QtTest>

#include "treefile.h"

class TestTreeFile : public QObject {
  Q_OBJECT

public:
  TestTreeFile();
  ~TestTreeFile();

private slots:
  void initTestCase();
  void cleanupTestCase();
  void cleanup();
  void test_root();
  void test_recursive_remove();

private:
  const QString _path = "treefile_test.db";
  std::unique_ptr<TreeFile> _tree_file;
};

TestTreeFile::TestTreeFile() : _tree_file(std::make_unique<TreeFile>(_path)) {}

TestTreeFile::~TestTreeFile() {}

void TestTreeFile::initTestCase() { _tree_file->create(); }

void TestTreeFile::cleanupTestCase() {
  QFile file(_path);
  file.remove();
}

void TestTreeFile::cleanup() { _tree_file->reset(); }

void TestTreeFile::test_root() {
  QCOMPARE(_tree_file->rootCount(), 0);
  QCOMPARE(_tree_file->insert(0, 0), true);
  QString root1 = "root1";
  QCOMPARE(_tree_file->setMarkdown(_tree_file->id(0, 0), root1), true);
  QCOMPARE(_tree_file->last()._markdown, root1);
  QCOMPARE(_tree_file->rootCount(), 1);
  QCOMPARE(_tree_file->integrityCheck(), true);
}

void TestTreeFile::test_recursive_remove() {
  QCOMPARE(_tree_file->insert(0, 0), true); // root1
  QCOMPARE(_tree_file->insert(0, 1), true); // root2
  QCOMPARE(_tree_file->insert(_tree_file->last()._id, 0),
           true); // root2 -> child1
  QCOMPARE(_tree_file->insert(_tree_file->last()._id, 0),
           true);           // root2 -> child1 -> child11
  _tree_file->remove(0, 1); // root2
  QCOMPARE(_tree_file->rootCount(), 1);
  QCOMPARE(_tree_file->integrityCheck(), true);
}

QTEST_MAIN(TestTreeFile)

#include "tst_treefile.moc"
