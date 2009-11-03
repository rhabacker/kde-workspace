//-----------------------------------------------------------------------------
//
// KDE Display screen saver setup module
//
// Copyright (c)  Martin R. Jones 1996
// Copyright (C) Chris Howells 2004
//

#ifndef __SCRNSAVE_H__
#define __SCRNSAVE_H__

#include <QWidget>
//Added by qt3to4:
#include <QMouseEvent>
#include <QLabel>
#include <QResizeEvent>
#include <QKeyEvent>
#include <kcmodule.h>

#include "kssmonitor.h"
#include "saverconfig.h"
#include "testwin.h"
#include "advanceddialog.h"
#include "saverlist.h"
#include "ui_screensaver.h"

class QTimer;
class QCheckBox;
class QLabel;
class Q3ListView;
class Q3ListViewItem;
class QPushButton;
class K3Process;
class KIntSpinBox;
class ScreenPreviewWidget;

//===========================================================================
class KScreenSaver : public KCModule, private Ui::ScreenSaver
{
    Q_OBJECT
public:
    KScreenSaver(QWidget *parent, const QVariantList &);
    ~KScreenSaver();

    virtual void load();
    virtual void save();
    virtual void defaults();

    void updateValues();
    void readSettings();

protected Q_SLOTS:
    void slotEnable( bool );
    void slotScreenSaver( Q3ListViewItem* );
    void slotSetup();
    void slotAdvanced();
    void slotTest();
    void slotStopTest();
    void slotTimeoutChanged( int );
    void slotLockTimeoutChanged( int );
    void slotLock( bool );
    void slotSetupDone(K3Process*);
    // when selecting a new screensaver, the old preview will
    // be killed. -- This callback is responsible for restarting the
    // new preview
    void slotPreviewExited(K3Process *);
    void findSavers();
    void slotEnablePlasma(bool enable);
    void slotPlasmaSetup();

protected:
    void writeSettings();
    void getSaverNames();
    void setMonitor();
    void setDefaults();
    void resizeEvent( QResizeEvent * );
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

protected:
    TestWin     *mTestWin;
    K3Process    *mTestProc;
    K3Process    *mSetupProc;
    K3Process    *mPreviewProc;
    KSSMonitor  *mMonitor;
    ScreenPreviewWidget *mMonitorPreview;
    KService::List mSaverServices;
    SaverList   mSaverList;
    QTimer      *mLoadTimer;

    int         mSelected;
    int         mPrevSelected;
    int		mNumLoaded;
    bool        mChanged;
    bool	mTesting;

    // Settings
    int         mTimeout;
    int         mLockTimeout;
    bool        mLock;
    bool        mEnabled;
    QString     mSaver;
    bool        mImmutable;
    bool        mPlasmaEnabled;
};

#endif
