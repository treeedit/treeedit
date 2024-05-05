#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "treefile.h"

TreeFile::TreeFile(const QString &path) {
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(path);

    if (_db.open()) {
        qDebug() << "Database: connection ok";
    } else {
        qDebug() << "Error: connection with database fail";
    }
}

TreeFile::~TreeFile() { _db.close(); }

bool TreeFile::create() {
    QSqlQuery query1;
    if (!query1.exec(
            "create table if not exists tree (id integer primary key "
            "autoincrement, parent integer, row integer, markdown text)")) {
        qDebug() << Q_FUNC_INFO << query1.lastError().text();
        Q_ASSERT(false);
        return false;
    }

    QSqlQuery query2;
    if (!query2.exec(
            "create index if not exists idx_tree_parent on tree (parent)")) {
        qDebug() << Q_FUNC_INFO << query2.lastError().text();
        Q_ASSERT(false);
        return false;
    }

    QSqlQuery query3;
    if (!query3.exec("create index if not exists idx_tree_parent_row on tree "
                     "(parent, row)")) {
        qDebug() << Q_FUNC_INFO << query3.lastError().text();
        Q_ASSERT(false);
        return false;
    }

    QSqlQuery query4;
    if (!query4.exec("create view if not exists tree_sorted as select * from "
                     "tree order by parent, row")) {
        qDebug() << Q_FUNC_INFO << query4.lastError().text();
        Q_ASSERT(false);
        return false;
    }

    return true;
}

bool TreeFile::reset() {
    QSqlQuery query;
    if (!query.exec("delete from tree")) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
        return false;
    }

    return true;
}

bool TreeFile::integrityCheck() const {
    // Checks for duplicates in pairs of parent and row.
    QSqlQuery query1;
    query1.prepare("select count(*) as count, count(distinct parent || row) as "
                   "different from tree");

    if (query1.exec() && query1.first()) {
        auto ok_count = false, ok_different = false;
        auto count =
            query1.value(query1.record().indexOf("count")).toInt(&ok_count);
        auto different =
            query1.value(query1.record().indexOf("different")).toInt(&ok_different);

        if (ok_count && ok_different && count != different) {
            return false;
        }
    }

    // Checks for parentless children.
    QSqlQuery query2;
    query2.prepare(
        "with recursive traversal(id, parent, level) as (select id, parent, 0 "
        "from tree where parent = 0 union all select tree2.id, tree2.parent, "
        "traversal2.level + 1 from tree tree2 join traversal traversal2 on "
        "tree2.parent = traversal2.id) select (select count(*) from tree) as "
        "count, (select count(*) from traversal) as different");

    if (query2.exec() && query2.first()) {
        auto ok_count = false, ok_different = false;
        auto count =
            query2.value(query2.record().indexOf("count")).toInt(&ok_count);
        auto different =
            query1.value(query1.record().indexOf("different")).toInt(&ok_different);

        if (ok_count && ok_different && count != different) {
            return false;
        }
    }

    return true;
}

int TreeFile::id(int parent, int row) const {
    QSqlQuery query;
    query.prepare(
        "select id from tree where parent = (:parent) and row = (:row)");
    query.bindValue(":parent", parent);
    query.bindValue(":row", row);

    if (query.exec() && query.first()) {
        auto ok_id = false;
        auto id = query.value(query.record().indexOf("id")).toInt(&ok_id);

        if (ok_id) {
            return id;
        }
    }

    return 0;
}

int TreeFile::rootCount() const {
    QSqlQuery query;
    query.prepare("select count(*) as count from tree where parent = 0");

    if (query.exec() && query.first()) {
        bool ok_count = false;
        int count = query.value(query.record().indexOf("count")).toInt(&ok_count);

        return count;
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
    }

    return 0;
}

int TreeFile::childCount(int id) const {
    QSqlQuery query;
    query.prepare("select count(*) as count from tree where parent = (:id)");
    query.bindValue(":id", id);

    if (query.exec() && query.first()) {
        bool ok_count = false;
        int count = query.value(query.record().indexOf("count")).toInt(&ok_count);

        return count;
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
    }

    return 0;
}

TreeItem TreeFile::root(int row) const {
    QSqlQuery query;
    query.prepare("select * from tree where parent = 0 and row = (:row)");
    query.bindValue(":row", row);

    if (query.exec() && query.first()) {
        auto ok_id = false, ok_parent = false, ok_row = false;
        auto id = query.value(query.record().indexOf("id")).toInt(&ok_id);
        auto parent =
            query.value(query.record().indexOf("parent")).toInt(&ok_parent);
        auto row = query.value(query.record().indexOf("row")).toInt(&ok_row);
        auto markdown = query.value(query.record().indexOf("markdown")).toString();

        if (ok_id && ok_parent && ok_row) {
            return {id, parent, row, markdown};
        }
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
    }

    auto error = QStringLiteral("error");

    return {0, 0, 0, error};
}

TreeItem TreeFile::item(int id) const {
    QSqlQuery query;
    query.prepare("select * from tree where id = (:id)");
    query.bindValue(":id", id);

    if (query.exec() && query.first()) {
        auto ok_id = false, ok_parent = false, ok_row = false;
        auto id = query.value(query.record().indexOf("id")).toInt(&ok_id);
        auto parent =
            query.value(query.record().indexOf("parent")).toInt(&ok_parent);
        auto row = query.value(query.record().indexOf("row")).toInt(&ok_row);
        auto markdown = query.value(query.record().indexOf("markdown")).toString();

        if (ok_id && ok_parent && ok_row) {
            return {id, parent, row, markdown};
        }
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
    }

    auto error = QStringLiteral("error");

    return {0, 0, 0, error};
}

TreeItem TreeFile::item(int parent, int row) const {
    QSqlQuery query;
    query.prepare("select * from tree where parent = (:parent) and row = (:row)");
    query.bindValue(":parent", parent);
    query.bindValue(":row", row);

    if (query.exec() && query.first()) {
        auto ok_id = false, ok_parent = false, ok_row = false;
        auto id = query.value(query.record().indexOf("id")).toInt(&ok_id);
        auto parent =
            query.value(query.record().indexOf("parent")).toInt(&ok_parent);
        auto row = query.value(query.record().indexOf("row")).toInt(&ok_row);
        auto markdown = query.value(query.record().indexOf("markdown")).toString();

        if (ok_id && ok_parent && ok_row) {
            return {id, parent, row, markdown};
        }
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
    }

    auto error = QStringLiteral("error");

    return {0, 0, 0, error};
}

TreeItem TreeFile::last() const {
    QSqlQuery query;
    query.prepare("select * from tree order by id desc limit 1");

    if (query.exec() && query.first()) {
        auto ok_id = false, ok_parent = false, ok_row = false;
        auto id = query.value(query.record().indexOf("id")).toInt(&ok_id);
        auto parent =
            query.value(query.record().indexOf("parent")).toInt(&ok_parent);
        auto row = query.value(query.record().indexOf("row")).toInt(&ok_row);
        auto markdown = query.value(query.record().indexOf("markdown")).toString();

        if (ok_id && ok_parent && ok_row) {
            return {id, parent, row, markdown};
        }
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
    }

    auto error = QStringLiteral("error");

    return {0, 0, 0, error};
}

QString TreeFile::markdown(int id) const {
    QSqlQuery query;
    query.prepare("select markdown from tree where id = (:id)");
    query.bindValue(":id", id);

    if (query.exec() && query.first()) {
        auto markdown = query.value(query.record().indexOf("markdown")).toString();

        return markdown;
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
    }

    return QString();
}

bool TreeFile::setMarkdown(int id, QString &value) {
    QSqlQuery query;
    query.prepare("update tree set markdown = (:markdown) where id = (:id)");
    query.bindValue(":id", id);
    query.bindValue(":markdown", value);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        Q_ASSERT(false);
    }

    return false;
}

bool TreeFile::insert(int parent, int row) {
    QSqlQuery query1;
    query1.prepare(
        "with iterator as (select *, row_number() over (order by row asc) + "
        "(:row) as row2 from tree where parent = (:parent) and row >= (:row)) "
        "update tree set row = (select row2 from iterator where iterator.id = "
        "tree.id) where parent = (:parent) and row >= (:row)");
    query1.bindValue(":parent", parent);
    query1.bindValue(":row", row);

    QSqlQuery query2;
    query2.prepare("insert into tree (parent, row, markdown) values ((:parent), "
                   "(:row), (:markdown))");
    query2.bindValue(":parent", parent);
    query2.bindValue(":row", row);
    query2.bindValue(":markdown", "");

    if (query1.exec() && query2.exec()) {
        return true;
    } else {
        qDebug() << Q_FUNC_INFO << query1.lastError().text()
                 << query2.lastError().text();
        Q_ASSERT(false);
    }

    return false;
}

bool TreeFile::remove(int parent, int row) {
    QSqlQuery query1;
    query1.prepare(
        "with recursive traversal(id, parent, level) as (select id, parent, 0 "
        "from tree where parent = (:parent) and row = (:row) union all select "
        "tree2.id, tree2.parent, traversal2.level + 1 from tree tree2 join "
        "traversal traversal2 on tree2.parent = traversal2.id) delete from tree "
        "where id in (select id from traversal)");
    query1.bindValue(":parent", parent);
    query1.bindValue(":row", row);

    QSqlQuery query2;
    query2.prepare("with iterator as (select *, row_number() over (order by row "
                   "asc) - 1 as row2 from tree where parent = (:parent)) update "
                   "tree set row = (select row2 from iterator where iterator.id "
                   "= tree.id) where parent = (:parent)");
    query2.bindValue(":parent", parent);

    if (query1.exec() && query2.exec()) {
        return true;
    } else {
        qDebug() << Q_FUNC_INFO << query1.lastError().text()
                 << query2.lastError().text();
        Q_ASSERT(false);
    }

    return false;
}

bool TreeFile::moveItem(int sourceParent, int sourceRow, int destinationParent,
                        int destinationRow) {
    QSqlQuery query;

    if (sourceParent == destinationParent) {

        // Swaps the `sourceRow` with the `destinationRow` while keeping the parent
        // the same.
        query.prepare(
            "update tree set row = case row when :sourceRow then (:destinationRow) "
            "when (:destinationRow) then (:sourceRow) end where parent = "
            "(:sourceParent) "
            "and row in ((:sourceRow), (:destinationRow))");
        query.bindValue(":sourceParent", sourceParent);
        query.bindValue(":sourceRow", sourceRow);
        query.bindValue(":destinationRow", destinationRow);
    } else {
        // TODO: use case for promote etc.
    }

    if (query.exec()) {
        return true;
    } else {
        qDebug() << Q_FUNC_INFO << query.lastError().text()
                 << query.lastError().text();
        Q_ASSERT(false);
    }

    return false;
}
