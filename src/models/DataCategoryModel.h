#ifndef DATACATEGORYMODEL_H
#define DATACATEGORYMODEL_H

#include <QAbstractListModel>
#include <QList>

struct DataCategory {
    QString name;
    QString icon;
    int fileCount;
    qint64 totalSize;
    bool selected;
};

struct FileInfo {
    QString path;
    qint64 size;
    QString category;
};

class DataCategoryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum CategoryRoles {
        NameRole = Qt::UserRole + 1,
        IconRole,
        FileCountRole,
        TotalSizeRole,
        SelectedRole
    };

    explicit DataCategoryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void toggleSelection(int index);
    Q_INVOKABLE void selectAll();
    Q_INVOKABLE void deselectAll();
    QList<FileInfo> getSelectedCategories() const;

public slots:
    void updateCategory(const QString &name, int fileCount, qint64 totalSize);

private:
    QList<DataCategory> m_categories;
    QList<FileInfo> m_allFiles;
};

#endif // DATACATEGORYMODEL_H
