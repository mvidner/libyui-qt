/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:	      YQPkgStatusFilterView.cc

  Author:     Stefan Hundhammer <sh@suse.de>

  Textdomain "packages-qt"

/-*/

#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qvgroupbox.h>

#define y2log_component "qt-pkg"
#include <ycp/y2log.h>

#include <Y2PM.h>
#include <y2pm/PMManager.h>

#include "YQPkgStatusFilterView.h"
#include "YQPackageSelector.h"
#include "YQi18n.h"
#include "utf8.h"
#include "YUIQt.h"


#define SPACING			6	// between subwidgets
#define MARGIN			4	// around the widget


YQPkgStatusFilterView::YQPkgStatusFilterView( QWidget * parent )
    : QVBox( parent )
{
    setMargin( MARGIN );
    setSpacing( SPACING );

    addVStretch( this );

#if 0
    // Headline
    QLabel * label = new QLabel( _( "Changes Overview" ), this );
    CHECK_PTR( label );
    label->setFont( YUIQt::yuiqt()->headingFont() );
#endif


    //
    // Packages with what status to show
    //

    QVGroupBox * gbox = new QVGroupBox( _( "Show packages with status" ), this );
    CHECK_PTR( gbox );

    _showDel		= addStatusCheckBox( gbox, _( "Delete"		), true );
    _showInstall	= addStatusCheckBox( gbox, _( "Install"		), true );
    _showUpdate		= addStatusCheckBox( gbox, _( "Update"		), true );
    _showAutoDel	= addStatusCheckBox( gbox, _( "Auto-delete"	), true );
    _showAutoInstall	= addStatusCheckBox( gbox, _( "Auto-install"	), true );
    _showAutoUpdate	= addStatusCheckBox( gbox, _( "Auto-update"	), true );
    _showTaboo		= addStatusCheckBox( gbox, _( "Taboo"		), true );

    addVSpacing( gbox, 8 );

    _showKeepInstalled	= addStatusCheckBox( gbox, _( "Keep"		), false );
    _showNoInst		= addStatusCheckBox( gbox, _( "Don't install"	), false );

    addVStretch( this );


    // Box for refresh button
    QHBox * hbox = new QHBox( this );
    CHECK_PTR( hbox );

    addHStretch( hbox );

    // Refresh button
    _refreshButton = new QPushButton( _( "&Refresh List" ), hbox );
    CHECK_PTR( _refreshButton );
    addHStretch( hbox );

    connect( _refreshButton,	SIGNAL( clicked() ),
	     this,	    	SLOT  ( filter()  ) );

    for ( int i=0; i < 6; i++ )
	addVStretch( this );
}


YQPkgStatusFilterView::~YQPkgStatusFilterView()
{
    // NOP
}



QCheckBox *
YQPkgStatusFilterView::addStatusCheckBox( QWidget *		parent,
					  const QString &	label,
					  bool			initiallyChecked )
{
    QCheckBox * checkBox = new QCheckBox( label, parent );
    CHECK_PTR( checkBox );
    checkBox->setChecked( initiallyChecked );

    connect( checkBox,	SIGNAL( clicked() ),
	     this,	SLOT  ( filter()  ) );

    return checkBox;
}


QSize
YQPkgStatusFilterView::minimumSizeHint() const
{
    return QSize( 0, 0 );
}


void
YQPkgStatusFilterView::filterIfVisible()
{
    if ( isVisible() )
	filter();
}


void
YQPkgStatusFilterView::filter()
{
    emit filterStart();

    PMManager::PMSelectableVec::const_iterator it = Y2PM::packageManager().begin();

    while ( it != Y2PM::packageManager().end() )
    {
	PMSelectablePtr selectable = *it;

	bool match =
	    check( selectable->candidateObj() ) ||
	    check( selectable->installedObj() );

	// If there is neither an installed nor a candidate package, check
	// any other instance.

	if ( ! match			  &&
	     ! selectable->candidateObj() &&
	     ! selectable->installedObj()   )
	    check( selectable->theObject() );

	++it;
    }

    emit filterFinished();
}


bool
YQPkgStatusFilterView::check( PMPackagePtr pkg )
{
    bool match = false;

    if ( ! pkg )
	return false;

    switch ( pkg->getSelectable()->status() )
    {
	case PMSelectable::S_Taboo:		match = _showTaboo->isChecked();		break;
	case PMSelectable::S_Del:		match = _showDel->isChecked();			break;
	case PMSelectable::S_Update:		match = _showUpdate->isChecked();		break;
	case PMSelectable::S_Install:		match = _showInstall->isChecked();		break;
	case PMSelectable::S_AutoDel:		match = _showAutoDel->isChecked();		break;
	case PMSelectable::S_AutoInstall:	match = _showAutoInstall->isChecked();		break;
	case PMSelectable::S_AutoUpdate:	match = _showAutoUpdate->isChecked();		break;
	case PMSelectable::S_KeepInstalled:	match = _showKeepInstalled->isChecked();	break;
	case PMSelectable::S_NoInst:		match = _showNoInst->isChecked();		break;

	    // Intentionally omitting 'default' branch so the compiler can
	    // catch unhandled enum states
    }

    if ( match )
	emit filterMatch( pkg );

    return match;
}



#include "YQPkgStatusFilterView.moc.cc"