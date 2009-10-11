// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 8; -*-
/* This file is part of the KDE project
   Copyright (C) by Andrew Stanley-Jones
   Copyright (C) 2004  Esben Mose Hansen <kde@mosehansen.dk>
   Copyright (C) 2008 by Dmitry Suzdalev <dimsuz@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef KLIPPER_H
#define KLIPPER_H

#include <QKeyEvent>
#include <QMenu>
#include <QTimer>

#include <KApplication>
#include <KIcon>

#include "urlgrabber.h"

class QClipboard;
class KToggleAction;
class KAboutData;
class KActionCollection;
class URLGrabber;
class ClipboardPoll;
class QTime;
class History;
class QAction;
class QMimeData;
class HistoryItem;
class KlipperSessionManager;

class Klipper : public QObject
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.kde.klipper.klipper")

public Q_SLOTS:
  Q_SCRIPTABLE QString getClipboardContents();
  Q_SCRIPTABLE void setClipboardContents(QString s);
  Q_SCRIPTABLE void clearClipboardContents();
  Q_SCRIPTABLE void clearClipboardHistory();
  Q_SCRIPTABLE QStringList getClipboardHistoryMenu();
  Q_SCRIPTABLE QString getClipboardHistoryItem(int i);
  Q_SCRIPTABLE void showKlipperPopupMenu();
  Q_SCRIPTABLE void showKlipperManuallyInvokeActionMenu();

public:
    Klipper(QObject *parent, const KSharedConfigPtr &config);
    ~Klipper();

    /**
     * Get clipboard history (the "document")
     */
    History* history() { return m_history; }

    URLGrabber* urlGrabber() const { return m_myURLGrabber; }

    static void updateTimestamp();
    static void createAboutData();
    static void destroyAboutData();
    static KAboutData* aboutData();

    void saveSettings() const;

public Q_SLOTS:
    void saveSession();
    void slotHistoryTopChanged();
    void slotConfigure();
    void slotEditData();

protected:
    /**
     * The selection modes
     *
     * Don't use 1, as I use that as a guard against passing
     * a boolean true as a mode.
     */
    enum SelectionMode { Clipboard = 2, Selection = 4 };

    void readProperties(KConfig *);

    /**
     * Loads history from disk.
     */
    bool loadHistory();

    /**
     * Save history to disk
     * @empty save empty history instead of actual history
     */
    void saveHistory(bool empty = false);
    /**
     * @returns the contents of the selection or, if empty, the contents of
     * the clipboard.
     */
    QString clipboardContents( bool *isSelection = 0L );

    void removeFromHistory( const QString& text );
    void setEmptyClipboard();

    void clipboardSignalArrived( bool selectionMode );

    /**
     * Check data in clipboard, and if it passes these checks,
     * store the data in the clipboard history.
     */
    void checkClipData( bool selectionMode );

    /**
     * Enter clipboard data in the history.
     */
    HistoryItem* applyClipChanges( const QMimeData* data );

    void setClipboard( const HistoryItem& item, int mode );
    bool ignoreClipboardChanges() const;

    KSharedConfigPtr config() const { return m_config; }
    bool isApplet() const { return m_config != KGlobal::config(); }

public Q_SLOTS:
    void slotPopupMenu();
    void slotAskClearHistory();

protected Q_SLOTS:
    void showPopupMenu( QMenu * );
    void slotRepeatAction();
    void setURLGrabberEnabled( bool );
    void disableURLGrabber();

private Q_SLOTS:
    void newClipData( bool selectionMode );
    void slotClearClipboard();

    void slotSelectionChanged() {
        clipboardSignalArrived( true );
    }
    void slotClipboardChanged() {
        clipboardSignalArrived( false );
    }

    void slotQuit();
    void slotStartHideTimer();
    void slotStartShowTimer();

    void slotClearOverflow();
    void slotCheckPending();

    void loadSettings();

private:

    QClipboard *m_clip;

    QTime *m_hideTimer;
    QTime *m_showTimer;

    int m_lastClipboard;
    int m_lastSelection;
    History* m_history;
    int m_overflowCounter;
    KToggleAction *m_toggleURLGrabAction;
    QAction* m_clearHistoryAction;
    QAction* m_repeatAction;
    QAction* m_editAction;
    QAction* m_configureAction;
    QAction* m_quitAction;
    QPixmap m_pixmap;
    bool m_bPopupAtMouse :1;
    bool m_bKeepContents :1;
    bool m_bURLGrabber   :1;
    bool m_bReplayActionInHistory :1;
    bool m_bUseGUIRegExpEditor    :1;
    bool m_bNoNullClipboard       :1;
    bool m_bTearOffHandle         :1;
    bool m_bIgnoreSelection       :1;
    bool m_bSynchronize           :1;
    bool m_bSelectionTextOnly     :1;
    bool m_bIgnoreImages          :1;

    /**
     * Avoid reacting to our own changes, using this
     * lock.
     * Don't manupulate this object directly... use the Ignore struct
     * instead
     */
    int m_locklevel;

    URLGrabber *m_myURLGrabber;
    QString m_lastURLGrabberTextSelection;
    QString m_lastURLGrabberTextClipboard;
    KSharedConfigPtr m_config;
    QTimer m_overflowClearTimer;
    QTimer m_pendingCheckTimer;
    bool m_pendingContentsCheck;
    ClipboardPoll* m_poll;
    static KAboutData* m_about_data;

    bool blockFetchingNewData();
    KlipperSessionManager* m_session_managed;
    KActionCollection *m_collection;
};

#endif
