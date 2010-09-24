/*
 *   Copyright 2008 Marco Martin <notmart@gmail.com>
 *   Copyright 2009 Aaron Seigo <aseigo@kde.org>
 *   Copyright 2010 Chani Armitage <chani@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "controllerwindow.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QPainter>

#include <kwindowsystem.h>
#include <netwm.h>

#include <Plasma/Containment>
#include <Plasma/Corona>
#include <Plasma/Theme>
#include <Plasma/FrameSvg>
#include <Plasma/Dialog>
#include <Plasma/WindowEffects>

#include "activitymanager/activitymanager.h"
#include "panelview.h"
#include "plasmaapp.h"
#include "widgetsexplorer/widgetexplorer.h"

#include <kephal/screens.h>

ControllerWindow::ControllerWindow(QWidget* parent)
   : QWidget(parent),
     m_location(Plasma::Floating),
     m_layout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
     m_background(new Plasma::FrameSvg(this)),
     m_screen(-1),
     m_corona(0),
     m_view(0),
     m_watchedWidget(0),
     m_activityManager(0),
     m_widgetExplorer(0),
     m_graphicsWidget(0)
{
    Q_UNUSED(parent)

    m_background->setImagePath("dialogs/background");
    m_background->setContainsMultipleImages(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    KWindowSystem::setState(winId(), NET::SkipTaskbar | NET::SkipPager | NET::Sticky | NET::KeepAbove);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocus(Qt::ActiveWindowFocusReason);
    setLocation(Plasma::BottomEdge);

    QPalette pal = palette();
    pal.setBrush(backgroundRole(), Qt::transparent);
    setPalette(pal);

    Plasma::WindowEffects::overrideShadow(winId(), true);


    m_layout->setContentsMargins(0, 0, 0, 0);

    connect(KWindowSystem::self(), SIGNAL(activeWindowChanged(WId)), this, SLOT(onActiveWindowChanged(WId)));
    connect(m_background, SIGNAL(repaintNeeded()), SLOT(backgroundChanged()));
    Kephal::Screens *screens = Kephal::Screens::self();
    connect(screens, SIGNAL(screenResized(Kephal::Screen *, QSize, QSize)),
            this, SLOT(adjustAndSetMaxSize()));
    m_adjustViewTimer = new QTimer(this);
    m_adjustViewTimer->setSingleShot(true);
    connect(m_adjustViewTimer, SIGNAL(timeout()), this, SLOT(syncToGraphicsWidget()));
    adjustAndSetMaxSize();
}

ControllerWindow::~ControllerWindow()
{

    if (m_activityManager) {
        if (m_corona) {
            m_corona->removeOffscreenWidget(m_activityManager);
        }
        //FIXME the qt4.6 comment below applies here too
    }
    if (m_widgetExplorer) {
        if (m_corona) {
            m_corona->removeOffscreenWidget(m_widgetExplorer);
        }

        if (m_widgetExplorer->scene()) {
            //FIXME: causes a crash in Qt 4.6 *sigh*
            //m_widgetExplorer->scene()->removeItem(m_widgetExplorer);
        }
    }

    delete m_activityManager;
    delete m_widgetExplorer;
    delete m_view;
}


void ControllerWindow::adjustAndSetMaxSize()
{
    QSize screenSize = Kephal::ScreenUtils::screenGeometry(Kephal::ScreenUtils::screenId(pos())).size();
    setMaximumSize(screenSize);
    adjustSize();
}

void ControllerWindow::backgroundChanged()
{
    Plasma::Location l = m_location;
    m_location = Plasma::Floating;
    setLocation(l);
    update();
}

void ControllerWindow::setContainment(Plasma::Containment *containment)
{
    if (containment == m_containment.data()) {
        return;
    }

    if (m_containment) {
        disconnect(m_containment.data(), 0, this, 0);
    }

    m_containment = containment;

    if (!containment) {
        return;
    }

    m_corona = containment->corona();
    m_screen = containment->screen();

    if (m_widgetExplorer) {
        m_widgetExplorer->setContainment(containment);
    }
}

Plasma::Containment *ControllerWindow::containment() const
{
    return m_containment.data();
}

void ControllerWindow::setGraphicsWidget(QGraphicsWidget *widget)
{
    if (m_graphicsWidget == widget) {
        return;
    }

    if (m_graphicsWidget) {
        m_graphicsWidget->removeEventFilter(this);
        if (m_graphicsWidget == m_widgetExplorer) {
            m_widgetExplorer->deleteLater();
            m_widgetExplorer = 0;
        } else if (m_graphicsWidget == m_activityManager) {
            m_activityManager->deleteLater();
            m_activityManager = 0;
        }
    }

    m_graphicsWidget = widget;

    if (widget) {
        if (!layout()) {
            QVBoxLayout *lay = new QVBoxLayout(this);
            lay->setMargin(0);
            lay->setSpacing(0);
        }

        if (!m_view) {
            m_view = new QGraphicsView(this);
            m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_view->setFrameShape(QFrame::NoFrame);
            m_view->viewport()->setAutoFillBackground(false);
            layout()->addWidget(m_view);
        }

        m_view->setScene(widget->scene());

        //try to have the proper size -before- showing the dialog
        m_view->centerOn(widget);
        if (widget->layout()) {
            widget->layout()->activate();
        }
        static_cast<QGraphicsLayoutItem *>(widget)->updateGeometry();
        widget->resize(widget->size().expandedTo(widget->effectiveSizeHint(Qt::MinimumSize)));

        syncToGraphicsWidget();

        //adjustSizeTimer->start(150);

        widget->installEventFilter(this);
        adjustSize();

        bool moved = false;
        if (PlasmaApp::isPanelContainment(containment())) {
            // try to align it with the appropriate panel view
            foreach (PanelView * panel, PlasmaApp::self()->panelViews()) {
                if (panel->containment() == containment()) {
                    move(positionForPanelGeometry(panel->geometry()));
                    moved = true;
                    break;
                }
            }
        }

        if (!moved) {
            // set it to the bottom of the screen as we have no better hints to go by
            QRect geom = QApplication::desktop()->availableGeometry(screen());
            setGeometry(geom.x(), geom.bottom() - height(), geom.width(), height());
        }
    } else {
        delete m_view;
        m_view = 0;
    }
}

void ControllerWindow::syncToGraphicsWidget()
{
    m_adjustViewTimer->stop();
    if (m_view && m_graphicsWidget) {
        QSize prevSize = size();

        //set the sizehints correctly:
        int left, top, right, bottom;
        getContentsMargins(&left, &top, &right, &bottom);

        QRect screenRect;
        //Try to use the screenId directly from the containment, because it won't fail.
        //If we dont' have containment. try to get the screen by using QWidget::pos(), but it
        //may fail if syncToGraphicsWidget is called before a real position is set (so pos() will
        //just return 0x0, which may lead to the wrong screen
        if (m_containment) {
            screenRect = Kephal::ScreenUtils::screenGeometry(m_containment.data()->screen());
        } else {
            screenRect = Kephal::ScreenUtils::screenGeometry(Kephal::ScreenUtils::screenId(pos()));
        }
        QSize maxSize = KWindowSystem::workArea().intersect(screenRect).size();

        QSize windowSize;
        if (m_location == Plasma::LeftEdge || m_location == Plasma::RightEdge) {
            windowSize = QSize(qMin(int(m_graphicsWidget->size().width()) + left + right, maxSize.width()), maxSize.height());
            m_graphicsWidget->resize(m_graphicsWidget->size().width(), windowSize.height());
        } else {
            windowSize = QSize(maxSize.width(), qMin(int(m_graphicsWidget->size().height()) + top + bottom, maxSize.height()));
            m_graphicsWidget->resize(windowSize.width(), m_graphicsWidget->size().height());
        }

        resize(windowSize);

        setMinimumSize(windowSize);


        updateGeometry();

        //reposition and resize the view.
        //force a valid rect, otherwise it will take up the whole scene
        QRectF sceneRect(m_graphicsWidget->sceneBoundingRect());

        sceneRect.setWidth(qMax(qreal(1), sceneRect.width()));
        sceneRect.setHeight(qMax(qreal(1), sceneRect.height()));
        m_view->setSceneRect(sceneRect);

        m_view->centerOn(m_graphicsWidget);

    }
}

bool ControllerWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_graphicsWidget &&
        (event->type() == QEvent::GraphicsSceneResize || event->type() == QEvent::GraphicsSceneMove)) {
        m_adjustViewTimer->start(150);
    }

    return QWidget::eventFilter(watched, event);
}

void ControllerWindow::setLocation(const Plasma::Location &loc)
{
    if (m_location == loc) {
        return;
    }

    Plasma::WindowEffects::slideWindow(this, loc);

    m_location = loc;

    switch (loc) {
    case Plasma::LeftEdge:
        m_background->setEnabledBorders(Plasma::FrameSvg::RightBorder);
        m_layout->setDirection(QBoxLayout::TopToBottom);
        setContentsMargins(0, 0, m_background->marginSize(Plasma::RightMargin), 0);
        break;

    case Plasma::RightEdge:
        m_background->setEnabledBorders(Plasma::FrameSvg::LeftBorder);
        m_layout->setDirection(QBoxLayout::TopToBottom);
        setContentsMargins(m_background->marginSize(Plasma::LeftMargin), 0, 0, 0);
        break;

    case Plasma::TopEdge:
        m_background->setEnabledBorders(Plasma::FrameSvg::BottomBorder);
        m_layout->setDirection(QBoxLayout::BottomToTop);
        setContentsMargins(0, 0, 0, m_background->marginSize(Plasma::BottomMargin));
        break;

    case Plasma::BottomEdge:
    default:
        m_background->setEnabledBorders(Plasma::FrameSvg::TopBorder);
        m_layout->setDirection(QBoxLayout::TopToBottom);
        setContentsMargins(0, m_background->marginSize(Plasma::TopMargin), 0, 0);
        break;
    }

    if (m_watchedWidget) {
        //FIXME maybe I should make these two inherit from something
        //or make orientation a slot.
        if (m_watchedWidget == (QGraphicsWidget*)m_widgetExplorer) {
            m_widgetExplorer->setLocation(location());
        } else {
            m_activityManager->setOrientation(orientation());
        }
    }
}

QPoint ControllerWindow::positionForPanelGeometry(const QRect &panelGeom) const
{
    int screen;
    if (m_containment) {
        screen = m_containment.data()->screen();
    } else {
        //guess
        screen = Kephal::ScreenUtils::screenId(QCursor::pos());
    }

    QRect screenGeom = Kephal::ScreenUtils::screenGeometry(screen);

    switch (m_location) {
    case Plasma::LeftEdge:
        return QPoint(panelGeom.right(), screenGeom.top());
        break;
    case Plasma::RightEdge:
        return QPoint(panelGeom.left() - width(), screenGeom.top());
        break;
    case Plasma::TopEdge:
        return QPoint(screenGeom.left(), panelGeom.bottom());
        break;
    case Plasma::BottomEdge:
    default:
        return QPoint(screenGeom.left(), panelGeom.top() - height());
        break;
    }
}

Plasma::Location ControllerWindow::location() const
{
    return m_location;
}

Qt::Orientation ControllerWindow::orientation() const
{
    if (m_location == Plasma::LeftEdge || m_location == Plasma::RightEdge) {
        return Qt::Vertical;
    }

    return Qt::Horizontal;
}


void ControllerWindow::showWidgetExplorer()
{
    if (!m_containment) {
        return;
    }

    if (!m_widgetExplorer) {
        m_widgetExplorer = new Plasma::WidgetExplorer(location());
        m_watchedWidget = m_widgetExplorer;
        m_widgetExplorer->setContainment(m_containment.data());
        m_widgetExplorer->populateWidgetList();
        m_widgetExplorer->setIconSize(KIconLoader::SizeHuge);
        QAction *activityAction = new QAction(KIcon("preferences-activities"), i18n("Activities"), m_widgetExplorer);
        connect(activityAction, SIGNAL(triggered()), this, SLOT(showActivityManager()));
        m_widgetExplorer->addAction(activityAction);

        m_containment.data()->corona()->addOffscreenWidget(m_widgetExplorer);
        m_widgetExplorer->show();

        m_widgetExplorer->setIconSize(KIconLoader::SizeHuge);

        if (orientation() == Qt::Horizontal) {
            m_widgetExplorer->resize(width(), m_widgetExplorer->size().height());
        } else {
            m_widgetExplorer->resize(m_widgetExplorer->size().width(), height());
        }

        setGraphicsWidget(m_widgetExplorer);

        connect(m_widgetExplorer, SIGNAL(closeClicked()), this, SLOT(close()));
    } else {
        m_widgetExplorer->setLocation(location());
        m_widgetExplorer->show();
        m_watchedWidget = m_widgetExplorer;
        setGraphicsWidget(m_widgetExplorer);
    }
}

void ControllerWindow::showActivityManager()
{
    if (!m_activityManager) {
        m_activityManager = new ActivityManager(orientation());
        m_watchedWidget = m_activityManager;

        m_corona->addOffscreenWidget(m_activityManager);
        m_activityManager->show();

        if (orientation() == Qt::Horizontal) {
            m_activityManager->resize(width(), m_activityManager->size().height());
        } else {
            m_activityManager->resize(m_activityManager->size().width(), height());
        }

        m_activityManager->setIconSize(KIconLoader::SizeHuge);

        setGraphicsWidget(m_activityManager);

        connect(m_activityManager, SIGNAL(addWidgetsRequested()), this, SLOT(showWidgetExplorer()));
        connect(m_activityManager, SIGNAL(closeClicked()), this, SLOT(close()));
    } else {
        m_activityManager->setOrientation(orientation());
        m_watchedWidget = m_activityManager;
        m_activityManager->show();
        setGraphicsWidget(m_activityManager);
    }
}

bool ControllerWindow::isControllerViewVisible() const
{
    return m_view && m_view->isVisible();
}

Plasma::FrameSvg *ControllerWindow::background() const
{
    return m_background;
}

int ControllerWindow::screen() const
{
    return m_screen;
}

void ControllerWindow::setScreen(int screen)
{
    m_screen = screen;
}

void ControllerWindow::onActiveWindowChanged(WId id)
{
    Q_UNUSED(id)

    // if the active window isn't a Plasma::View and the widgets explorer is visible,
    // or the activityManager is visible, and we are no longer the active window but still
    // visible on-screen, then close up shop
    if ((m_activityManager ||
         (m_widgetExplorer && !qobject_cast<Plasma::View *>(QApplication::activeWindow()))) &&
        isControllerViewVisible() && !isActiveWindow()) {
        close();
    }
}

void ControllerWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source );

    m_background->paintFrame(&painter);
}

void ControllerWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}

void ControllerWindow::resizeEvent(QResizeEvent * event)
{
    m_background->resizeFrame(size());

    Plasma::WindowEffects::enableBlurBehind(effectiveWinId(), true, m_background->mask());

    qDebug() << "ControllerWindow::resizeEvent" << event->oldSize();

    QWidget::resizeEvent(event);
}

#include "controllerwindow.moc"
