#ifndef SGISTYLE_H
#define SGISTYLE_H

#include <kstyle.h>

class QPushButton;

class KSgiStyle : public KStyle
{

	public:
	
		KSgiStyle();
		~KSgiStyle();
	
    virtual void polish(QWidget *w);
    virtual void unPolish(QWidget *w);

    bool eventFilter(QObject *obj, QEvent *ev);
		
		void drawFullShadeButton (QPainter *p, int x, int y, int w, int h,
                    const QColorGroup &g, bool sunken = FALSE,
                    const QBrush *fill = 0);
		void drawPartShadeButton (QPainter *p, int x, int y, int w, int h,
                    const QColorGroup &g, bool sunken = FALSE,
                    const QBrush *fill = 0);								
		void drawButton(QPainter *p, int x, int y, int w, int h,
                    const QColorGroup &g, bool sunken = FALSE,
                    const QBrush *fill = 0 );
		void drawPushButton(QPushButton *btn, QPainter *p);
		QRect buttonRect (int x, int y, int w, int h);
	/*	void drawButtonMask(QPainter *p, int x, int y, int w, int h);
		*/
    void drawBevelButton(QPainter *p, int x, int y, int w, int h,
                         const QColorGroup &g, bool sunken = FALSE,
                         const QBrush *fill = 0 );

		QSize indicatorSize() const;
		void drawIndicator(QPainter *p, int x, int y, int w, int h,
										const QColorGroup &g, int state, bool down=FALSE, 
										bool enabled = TRUE);
		
		QSize exclusiveIndicatorSize() const;
		void drawExclusiveIndicator(QPainter *p, int x, int y, int w, int h,
										const QColorGroup &g, bool on, bool down=FALSE, 
										bool enabled = TRUE);

    void drawComboButton(QPainter *p, int x, int y, int w, int h,
                         const QColorGroup &g, bool sunken = FALSE,
                         bool editable = FALSE, bool enabled = TRUE,
                         const QBrush *fill = 0 );
		QRect comboButtonRect (int x, int y, int w, int h);
		QRect comboButtonFocusRect (int x, int y, int w, int h);
		
		int sliderLength() const;
   	void drawSliderGroove(QPainter *p, int x, int y, int w, int h,
                          const QColorGroup &g, QCOORD, Orientation);
		void	drawSlider(QPainter *p, int x, int y, int w, int h,
                    const QColorGroup &g, Orientation orient, bool, bool);

		void drawCheckMark( QPainter *p, int x, int y, int w, int h,
									const QColorGroup &g, bool act, bool dis );
									
		void drawScrollBarControls(QPainter *p, const QScrollBar *sb,
                                       int sliderStart, uint controls,
                                       uint activeControl);
		void scrollBarMetrics(const QScrollBar *sb, int &sliderMin,
                                  int &sliderMax, int &sliderLength,
                                  int &buttonDim);			
		QStyle::ScrollControl scrollBarPointOver(const QScrollBar *sb, 
																							int sliderStart, const QPoint &p);
		void	drawScrollBarArrow(QPainter *p, Qt::ArrowType type, int x,
                                  int y, const QColorGroup &g);
																	
		int 	splitterWidth() const;
		void	drawSplitter (QPainter *p, int x, int y, int w, int h,
												const QColorGroup &g, Orientation);
																	
																	
	private:
	
		QWidget*	highlightWidget;
		QPixmap*	checkPix;

	
};

#endif
