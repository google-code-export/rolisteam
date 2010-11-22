#ifndef USERLISTDELEGATE_H
#define USERLISTDELEGATE_H

#include <QAbstractItemDelegate>

class UserListDelegate : public QAbstractItemDelegate
{

public:
    explicit UserListDelegate(QObject *parent = 0);

protected:
    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const ;
    virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

public slots:

};

#endif // USERLISTDELEGATE_H
