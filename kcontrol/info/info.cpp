/* 	
	$Id$
	
	Main Widget for showing system-dependent information.
	(But all functions in THIS FILE should be system independent !)

	(C) 1998-1999 by Helge Deller (deller@gmx.de)
	
	** main.cpp includes this file ! **

	This source-file includes another system-dependet sourcefile called
	info_<systemname>.cpp
	which should define one or more of the following defines to
	indicate, that this information is really available.
	
        #define INFO_CPU_AVAILABLE
	#define INFO_IRQ_AVAILABLE
        #define INFO_DMA_AVAILABLE
	#define INFO_PCI_AVAILABLE
        #define INFO_IOPORTS_AVAILABLE
	#define INFO_SOUND_AVAILABLE
        #define INFO_DEVICES_AVAILABLE
	#define INFO_SCSI_AVAILABLE
        #define INFO_PARTITIONS_AVAILABLE
	#define INFO_XSERVER_AVAILABLE
	
	right now, there is the problem, that also the .desktop-files should 
	depend on the systemname, so that only available .desktop-files will 
	be copied to kde/applnk/Settings/Information !!
*/

#include <qheader.h>

#include <kapp.h>
#include <kglobal.h>
#include <kcharsets.h>
#include <klocale.h>
#include "info.h"		/* include the forward declares... */

#include <X11/Xlib.h>

/* All Functions GetInfo_xyz() can set GetInfo_ErrorString, when a special 
   error-message should be shown to the user....
   If GetInfo_ErrorString is not modified in the function, the default string
   DEFAULT_ERRORSTRING will be used...
*/

static QString *GetInfo_ErrorString;	/* should allways point to:
					    KInfoListWidget::ErrorString */
static bool	sorting_allowed;	/* is sorting allowed by user ? */



#if defined(linux)
# define DEFAULT_ERRORSTRING QString("") /* i18n("Maybe the proc-filesystem is not enabled in Linux-Kernel.") */
#elif defined(hpux) 
# define DEFAULT_ERRORSTRING QString("") 
#else
#define DEFAULT_ERRORSTRING  i18n("Maybe this system is not completely supported yet :-(")
#endif





/* easier to read with such a define ! */
#define I18N_MAX(txt,in,fm,maxw) \
    { int n = fm.width(txt=in); if (n>maxw) maxw=n; }
    
#define PIXEL_ADD	20	// add x Pixel to multicolumns..

#define HEXDIGITS (sizeof(int)*8/4)	/* 4 Byte = 32 Bit = 8 Hex-Digits */

static QString Value( int val, int numbers=1 )
{	return QString("%1").arg(val,numbers);
}

static QString HexStr(unsigned long val, int digits )
{
    QString hexstr;
    int i;
    hexstr = QString("0x%1").arg(val,digits,16/*=HEX*/);
    for (i=hexstr.length()-1; i>0; --i)
     if (hexstr[i]==' ') 
         hexstr[i] = '0';
    return hexstr;
}

static struct _event_table {
    const char *name;
    long value;
} event_table[] = {
    { "KeyPressMask", 		KeyPressMask },
    { "KeyReleaseMask",		KeyReleaseMask },
    { "ButtonPressMask",	ButtonPressMask },
    { "ButtonReleaseMask",	ButtonReleaseMask },
    { "EnterWindowMask",	EnterWindowMask },
    { "LeaveWindowMask",	LeaveWindowMask },
    { "PointerMotionMask",	PointerMotionMask },
    { "PointerMotionHintMask",	PointerMotionHintMask },
    { "Button1MotionMask",	Button1MotionMask },
    { "Button2MotionMask",	Button2MotionMask },
    { "Button3MotionMask",	Button3MotionMask },
    { "Button4MotionMask",	Button4MotionMask },
    { "Button5MotionMask",	Button5MotionMask },
    { "ButtonMotionMask",	ButtonMotionMask },
    { "KeymapStateMask",	KeymapStateMask },
    { "ExposureMask",		ExposureMask },
    { "VisibilityChangeMask",	VisibilityChangeMask },
    { "StructureNotifyMask",	StructureNotifyMask },
    { "ResizeRedirectMask",	ResizeRedirectMask },
    { "SubstructureNotifyMask",	SubstructureNotifyMask },
    { "SubstructureRedirectMask",SubstructureRedirectMask },
    { "FocusChangeMask",	FocusChangeMask },
    { "PropertyChangeMask",	PropertyChangeMask },
    { "ColormapChangeMask",	ColormapChangeMask },
    { "OwnerGrabButtonMask",	OwnerGrabButtonMask },
    { 0L, 0 }};


static QListViewItem* XServer_fill_screen_info( QListViewItem *lBox, QListViewItem *last, 
	    Display *dpy, int scr, int default_scr)
{
    unsigned	width, height;
    double	xres, yres;
    int 	i,
		ndepths,
		*depths;
    Screen 	*s = ScreenOfDisplay(dpy,scr);  /* opaque structure */
    QListViewItem *item;
    
    /*
     * there are 2.54 centimeters to an inch; so there are 25.4 millimeters.
     *
     *     dpi = N pixels / (M millimeters / (25.4 millimeters / 1 inch))
     *         = N pixels / (M inch / 25.4)
     *         = N * 25.4 pixels / M inch
     */
     
    xres = ((double)(DisplayWidth(dpy,scr) *25.4)/DisplayWidthMM(dpy,scr) );
    yres = ((double)(DisplayHeight(dpy,scr)*25.4)/DisplayHeightMM(dpy,scr));
    
    item = new QListViewItem(lBox,last, i18n("Screen # %1").arg((int)scr,-1),
		(scr==default_scr) ? i18n("(Default Screen)") : QString::null );
    item->setExpandable(true);
    if (scr==default_scr)
	item->setOpen(true);
    last = new QListViewItem(item, i18n("Dimensions"),
		i18n("%1 x %2 Pixel (%3 x %4 mm)")
		.arg( (int)DisplayWidth(dpy,scr) )
		.arg( (int)DisplayHeight(dpy,scr) )
		.arg( (int)DisplayWidthMM(dpy,scr) )
		.arg( (int)DisplayHeightMM (dpy,scr) ));
    
    last = new QListViewItem(item, last, i18n("Resolution"), 
		i18n("%1 x %2 dpi")
		.arg( (int)(xres+0.5) )
		.arg( (int)(yres+0.5) ));
    
    ndepths = 0;
    depths  = 0;
    depths = XListDepths (dpy, scr, &ndepths);
    if (depths) {
	QString txt;
    
        for (i = 0; i < ndepths; i++) {	
            txt = txt + Value(depths[i]);
            if (i < ndepths - 1)
                txt = txt + QString(", ");
        }
    
        last = new QListViewItem(item, last, i18n("Depths (%1)").arg(ndepths,-1), txt);
        XFree((char *) depths);
    }

    last = new QListViewItem(item, last, i18n("Root Window ID"), 
		HexStr((unsigned long)RootWindow(dpy,scr),HEXDIGITS));
    last = new QListViewItem(item, last, i18n("Depth of Root Window"), 
		(DisplayPlanes (dpy, scr) == 1)
		?	i18n("%1 plane").arg(DisplayPlanes(dpy,scr))   /*singular*/
		:	i18n("%1 planes").arg(DisplayPlanes(dpy,scr)));/*plural*/
    last = new QListViewItem(item, last, i18n("Number of Colormaps"),
		i18n("minimum %1, maximum %2")
		    .arg((int)MinCmapsOfScreen(s)).arg((int)MaxCmapsOfScreen(s)));
    last = new QListViewItem(item, last, i18n("Default Colormap"),
		Value((int)DefaultColormap(dpy,scr)));
    last = new QListViewItem(item, last, i18n("Default Number of Colormap Cells"),
		Value((int)DisplayCells(dpy, scr)));
    last = new QListViewItem(item, last, i18n("Preallocated Pixels"),
		i18n("black %1, White %2").arg(BlackPixel(dpy,scr)).arg(WhitePixel(dpy,scr)));

    QString YES(i18n("Yes"));
    QString NO(i18n("No"));
    last = new QListViewItem(item, last, i18n("Options"),
		i18n("backing-store: %1, save-unders: %2")
		.arg( (DoesBackingStore(s) == NotUseful) ? NO :
		      ((DoesBackingStore(s) == Always) ? YES : i18n("When mapped")) )
		.arg(  DoesSaveUnders(s) ? YES : NO ));

    XQueryBestSize (dpy, CursorShape, RootWindow(dpy,scr), 65535, 65535,
		    &width, &height);
    last = new QListViewItem(item, last, i18n("Largest Cursor"),
		(width == 65535 && height == 65535)
		? i18n("unlimited") : QString("%1 x %2").arg(width).arg(height));

    last = new QListViewItem(item, last, i18n("Current Input Event Mask"),
		HexStr((unsigned long)EventMaskOfScreen(s),HEXDIGITS));
    item = last;
    struct _event_table *etp;
    for (etp=event_table; etp->name; etp++) {
	if (EventMaskOfScreen(s) & etp->value)
	    item = new QListViewItem(last, item,
		i18n("Event = %1").arg(HexStr(etp->value,HEXDIGITS)),
		etp->name );
    }

    return item;
}

QString Order( int order ) 
{
    if (order==LSBFirst) return i18n("LSBFirst"); else
    if (order==MSBFirst) return i18n("MSBFirst"); else 
	return i18n("Unknown Order %1").arg(order);
}

bool GetInfo_XServer_Generic( QListView *lBox )
{
    /* Many parts of this source are taken from the X11-program "xdpyinfo" */

    int i,n;
    long req_size;
    
    Display *dpy;
    XPixmapFormatValues *pmf;

    QString str,txt;
    QListViewItem *last, *item, *next;

    dpy = XOpenDisplay(0);
    if (!dpy)
        return false;

    lBox->addColumn(i18n("Information") );
    lBox->addColumn(i18n("Value") );
    sorting_allowed = false;
			
    next = new QListViewItem(lBox, i18n("X-Server Information"));
    next->setOpen(true);
    
    last = new QListViewItem(next, i18n("Name of the Display"), 
		DisplayString(dpy));

    last = new QListViewItem(next, last, i18n("Vendor String"), QString(ServerVendor(dpy)));
    last = new QListViewItem(next, last, i18n("Vendor Release Number"),
		Value((int)VendorRelease(dpy)));

    last = new QListViewItem(next, last, i18n("Version Number"),
		QString("%1.%2").arg((int)ProtocolVersion(dpy))
    		                  .arg((int)ProtocolRevision(dpy)));

    last = item = new QListViewItem(next, last, i18n("Available Screens"));
    last->setOpen(true);
    last->setExpandable(true);
    for (i=0; i<ScreenCount(dpy); i++) {
        item = XServer_fill_screen_info(last, item, dpy, i, (int)DefaultScreen(dpy));
	if (i==0) item->setOpen(true);
    }

    pmf = XListPixmapFormats(dpy, &n);
    last = item = new QListViewItem(next, last, i18n("Supported Pixmap Formats"));
    if (pmf) {
	last->setExpandable(true);
	for (i=0; i<n; i++) {
	    item = new QListViewItem(last, item, i18n("Pixmap Format #%1").arg(i+1),
			i18n("%1 BPP, Depth: %2 Bit, scanline_pad: %3)").
			arg(pmf[i].bits_per_pixel).arg(pmf[i].depth).
			arg(pmf[i].scanline_pad));
	}
	XFree ((char *)pmf);
    }

    req_size = XExtendedMaxRequestSize(dpy);
    if (!req_size) req_size = XMaxRequestSize(dpy);
    last = new QListViewItem(next, last, i18n("Maximum Request Size"),
		QString("%1 Byte").arg(req_size*4));
    last = new QListViewItem(next, last, i18n("Motion Buffer Size"),
		QString("%1 Byte").arg(XDisplayMotionBufferSize(dpy)));

    last = item = new QListViewItem(next, last, i18n("Bitmap"));
    last->setExpandable(true);
    item = new QListViewItem(last, item, i18n("Unit"), 
		Value(BitmapUnit(dpy)) );
    item = new QListViewItem(last, item, i18n("Order"),
		Order(BitmapBitOrder(dpy)));
    item = new QListViewItem(last, item, i18n("Padding"),
		Value(BitmapPad(dpy)));

    last = new QListViewItem(next, last, i18n("Image Byte Order"),
		Order(ImageByteOrder(dpy)));


    XCloseDisplay (dpy);
    return true;
}



/*
***************************************************************************
***************************************************************************
***************************************************************************
*/

#include <qlayout.h>
#include <qcollection.h>
#include <qwidget.h>
#include <qwidgetlist.h>


#define SCREEN_XY_OFFSET 20

void KInfoListWidget::defaults()
{  
    bool ok = false;

    delete lBox;
    lBox  = new QListView(this);

    if (lBox) {
	lBox->setFont(KGlobal::generalFont()); /* default font */
        lBox->setAllColumnsShowFocus(true);
        setMinimumSize( 200,6*SCREEN_XY_OFFSET );
        lBox->setGeometry(SCREEN_XY_OFFSET,SCREEN_XY_OFFSET,
                          width() -2*SCREEN_XY_OFFSET,
                          height()-2*SCREEN_XY_OFFSET);
        
	/*  Delete the user-visible ErrorString, before calling the 
	    retrieve-function. If the function wants the widget to show
	    another string, then it change *GetInfo_ErrorString ! */
        ErrorString = i18n("Sorry, no %1-information available !").arg(title)
		    + QString("\n\n") + DEFAULT_ERRORSTRING;
	GetInfo_ErrorString = &ErrorString;  /* save the adress of ErrorString */
	
	sorting_allowed = true; 	/* the functions may set that */
        lBox->setSorting(-1);   	/* No Sorting per default */
        
        if (getlistbox)
            ok = (*getlistbox)(lBox);	/* retrieve the information */

        if (lBox->header()->count()<=1) 
            lBox->addColumn(title);	/* set default title */
        if (ok)
            lBox->show();

	/* is the user allowed to use sorting ? */
        lBox->header()->setClickEnabled(sorting_allowed); 
	lBox->setShowSortIndicator(sorting_allowed);
    }

    if (!ok) {
        if (lBox) {
            delete lBox;
            lBox = 0;
        }	    
        if (NoInfoText)
            NoInfoText->setText(ErrorString);
        else
            NoInfoText = new QLabel(ErrorString,this);
        NoInfoText->setAutoResize(true);
        NoInfoText->setAlignment(AlignCenter); //  | WordBreak);
        NoInfoText->move( width()/2,height()/2 ); // -120 -30
        NoInfoText->adjustSize();
    }
}


KInfoListWidget::KInfoListWidget(QWidget *parent, const char *name, 
                                 bool _getlistbox(QListView *lbox))
    : KCModule(parent, name)
{   
    getlistbox 	= _getlistbox;
    title	= name;
    lBox 	= 0;
    NoInfoText  = 0;
    GetInfo_ErrorString = 0;
    defaults();

    setButtons(Ok|Help);
}


void KInfoListWidget::resizeEvent( QResizeEvent *re )
{
    QSize size = re->size();
    if (lBox)
        lBox->setGeometry(SCREEN_XY_OFFSET,SCREEN_XY_OFFSET,
                          size.width() -2*SCREEN_XY_OFFSET,
                          size.height()-2*SCREEN_XY_OFFSET);
    if (NoInfoText) {
        QSize s = NoInfoText->sizeHint();
    	NoInfoText->move(   (size.width()-s.width())/2,
                            (size.height()-s.height())/2 );
    }
}

/*
***************************************************************************
**  Include system-specific code					 **
***************************************************************************
*/

#ifdef linux
#include "info_linux.cpp"
#elif sgi || sun
#include "info_sgi.cpp"
#elif __FreeBSD__
#include "info_fbsd.cpp"
#elif hpux
#include "info_hpux.cpp"
#elif __svr4__
#include "info_svr4.cpp"
#else
#include "info_generic.cpp"	/* Default for unsupportet systems.... */
#endif

/*
***************************************************************************
**  End of: Include system-specific code				 **
***************************************************************************
*/
