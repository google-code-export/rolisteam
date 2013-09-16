#ifndef CHATBROWSER_H
#define CHATBROWSER_H

#include <QTextBrowser>
#include <QAction>
class ChatBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit ChatBrowser(QWidget *parent = 0);
    
signals:

protected:
        //void contextMenuEvent ( QContextMenuEvent * event );
    
private slots:
    void backGrounChanged();
    void showContextMenu(QPoint pos);


private:
    QAction* m_bgColorAct;
        QColor m_bgColor;
};

#endif // CHATBROWSER_H
