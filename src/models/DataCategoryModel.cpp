#include "DataCategoryModel.h"

DataCategoryModel::DataCategoryModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // Initialize default categories
    m_categories = {
        {"Documents", "qrc:/icons/documents.svg", 0, 0, true},
        {"Configurations", "qrc:/icons/settings.svg", 0, 0, true},
        {"Applications", "qrc:/icons/applications.svg", 0, 0, false},
        {"Emails", "qrc:/icons/email.svg", 0, 0, true},
        {"Browser Data", "qrc:/icons/browser.svg", 0, 0, true}
    };
}

int DataCategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_categories.count();
}

QVariant DataCategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_categories.count())
        return QVariant();

    const DataCategory &category = m_categories.at(index.row());

    switch (role) {
    case NameRole:
        return category.name;
    case IconRole:
        return category.icon;
    case FileCountRole:
        return category.fileCount;
    case TotalSizeRole:
        return category.totalSize;
    case SelectedRole:
        return category.selected;
    default:
        return QVariant();
    }
}

bool DataCategoryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_categories.count())
        return false;

    if (role == SelectedRole) {
        m_categories[index.row()].selected = value.toBool();
        emit dataChanged(index, index, {SelectedRole});
        return true;
    }

    return false;
}

Qt::ItemFlags DataCategoryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> DataCategoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "categoryName";
    roles[IconRole] = "categoryIcon";
    roles[FileCountRole] = "fileCount";
    roles[TotalSizeRole] = "totalSize";
    roles[SelectedRole] = "isSelected";
    return roles;
}

void DataCategoryModel::toggleSelection(int index)
{
    if (index >= 0 && index < m_categories.count()) {
        m_categories[index].selected = !m_categories[index].selected;
        emit dataChanged(this->index(index), this->index(index), {SelectedRole});
    }
}

void DataCategoryModel::selectAll()
{
    for (int i = 0; i < m_categories.count(); ++i) {
        m_categories[i].selected = true;
    }
    emit dataChanged(index(0), index(m_categories.count() - 1), {SelectedRole});
}

void DataCategoryModel::deselectAll()
{
    for (int i = 0; i < m_categories.count(); ++i) {
        m_categories[i].selected = false;
    }
    emit dataChanged(index(0), index(m_categories.count() - 1), {SelectedRole});
}

QList<FileInfo> DataCategoryModel::getSelectedCategories() const
{
    QList<FileInfo> selectedFiles;
    for (const auto &file : m_allFiles) {
        for (const auto &category : m_categories) {
            if (category.name == file.category && category.selected) {
                selectedFiles.append(file);
                break;
            }
        }
    }
    return selectedFiles;
}

void DataCategoryModel::updateCategory(const QString &name, int fileCount, qint64 totalSize)
{
    for (int i = 0; i < m_categories.count(); ++i) {
        if (m_categories[i].name == name) {
            m_categories[i].fileCount = fileCount;
            m_categories[i].totalSize = totalSize;
            emit dataChanged(index(i), index(i));
            break;
        }
    }
}
