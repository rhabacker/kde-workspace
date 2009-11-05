/*
Copyright (c) 2007 Zack Rusin <zack@kde.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */
#include "webapplet.h"

#include "webpage.h"

#include <QPainter>
#include <QWebView>
#include <QWebFrame>
#include <QWebPage>
#include <QFile>

#include <Plasma/Applet>
#include <Plasma/Package>
#include <Plasma/WebView>

using namespace Plasma;

class WebApplet::Private
{
public:
    Private()
        : page(0)
    {
    }

    void init(WebApplet *q)
    {
        loaded = false;

        Plasma::Applet *applet = q->applet();
        applet->setAcceptsHoverEvents(true);

        page = new Plasma::WebView(applet);
        QObject::connect(page, SIGNAL(loadFinished(bool)),
                         q, SLOT(loadFinished(bool)));
        QObject::connect(page->page(), SIGNAL(frameCreated(QWebFrame *)),
                         q, SLOT(connectFrame(QWebFrame *)));
        q->connectFrame(page->mainFrame());

        page->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
        page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

        QPalette palette = page->palette();
        palette.setColor(QPalette::Base, Qt::transparent);
        page->page()->setPalette(palette);
    }

    Plasma::WebView *page;
    bool loaded;
};

WebApplet::WebApplet(QObject *parent, const QVariantList &args)
    : AppletScript(parent),
      d(new Private)
{
    Q_UNUSED(args)
}

WebApplet::~WebApplet()
{
    delete d;
}

bool WebApplet::init()
{
    d->init(this);

    QString webpage;
    webpage = package()->filePath("mainscript");

    if (webpage.isEmpty()) {
        kDebug() << "fail! no page";
        delete d->page;
        d->page = 0;
        return false;
    }
    KUrl url(package()->filePath("html"));
    kDebug() << webpage << package()->path() << url;
    d->page->mainFrame()->setHtml(dataFor(webpage), url);
    return true;
}

void WebApplet::paintInterface(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               const QRect &contentsRect)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(contentsRect)
}

Plasma::WebView* WebApplet::view() const
{
    return d->page;
}

void WebApplet::loadFinished(bool success)
{
    d->loaded = success;
}

void WebApplet::connectFrame(QWebFrame *frame)
{
    connect(frame, SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(initJsObjects()));
}

void WebApplet::initJsObjects()
{
}

QByteArray WebApplet::dataFor(const QString &str)
{
    QFile f(str);
    f.open(QIODevice::ReadOnly);
    QByteArray data = f.readAll();
    f.close();
    return data;
}

Plasma::WebView* WebApplet::page()
{
    return d->page;
}

bool WebApplet::loaded()
{
    return d->loaded;
}

#include "webapplet.moc"
