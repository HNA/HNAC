#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class HnaClient;
class login;
class logout;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private slots:
    void stateChanged();
    void trayActivated(QSystemTrayIcon::ActivationReason);

    void setOpacity(QAction*);
    void showHide();
    void setIsSave(bool);
    void setIsHide(bool);
    void setLanguage(QAction*);
    void about();
    void Exit();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *);
    void moveEvent(QMoveEvent *);
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);

private:
    QSystemTrayIcon *trayIcon;
    QStackedLayout *mainLayout;
    HnaClient *c;
    login *li;
    logout *lo;

    QAction *showHideAction;
    QAction *isSavePassword;
    QAction *isHideOnClose;
    QAction *quitAction;
    QAction *aboutAction;

    QMenu *opacityMenu;
    QMenu *optionsTrayMenu;
    QMenu *languageMenu;

    QSettings *settings;
    QPoint startPos;

    QTranslator translator;

    void setupGui();
    void createTrayIcon();
    void retranslateUI();
};

#endif // MAINWINDOW_H
