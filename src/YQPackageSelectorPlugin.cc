/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|						     (c) SuSE Linux AG |
\----------------------------------------------------------------------/

  File:		YQPackageSelectorPlugin.cc

  Author:	Stefan Hundhammer <sh@suse.de>

  Textdomain	"packages-qt"

/-*/

#include "YQPackageSelectorPlugin.h"
#define YUILogComponent "qt-ui"
#include "YUILog.h"
#include "pkg/YQPackageSelector.h"
#include "pkg/YQPatternSelector.h"
#include "pkg/YQSimplePatchSelector.h"
#include "YUIException.h"

#define PLUGIN_BASE_NAME "qt_pkg"


YQPackageSelectorPlugin::YQPackageSelectorPlugin()
    : YPackageSelectorPlugin( PLUGIN_BASE_NAME )
{
    if ( success() )
    {
	yuiMilestone() << "Loaded " << PLUGIN_BASE_NAME
		       << " plugin successfully from " << pluginLibFullPath()
		       << endl;
    }
    else
	YUI_THROW( YUIPluginException( PLUGIN_BASE_NAME ) );
}


YQPackageSelectorPlugin::~YQPackageSelectorPlugin()
{
    // NOP
}


YQPackageSelector *
YQPackageSelectorPlugin::createPackageSelector( YWidget * parent, long modeFlags )
{
    if ( error() )
	return 0;
    
    YQPackageSelector * packageSelector = 0;
    
    try
    {
	packageSelector = new YQPackageSelector( parent, modeFlags );
    }
    catch (const std::exception & e)
    {
	yuiError() << "Caught std::exception: " << e.what() << endl;
	yuiError() << "This is a libzypp problem. Do not file a bug against the UI!" << endl;
    }
    catch (...)
    {
	yuiError() << "Caught unspecified exception." << endl;
	yuiError() << "This is a libzypp problem. Do not file a bug against the UI!" << endl;
    }

    YUI_CHECK_NEW( packageSelector );
    
    return packageSelector;
}


YQPatternSelector *
YQPackageSelectorPlugin::createPatternSelector( YWidget * parent, long modeFlags )
{
    if ( error() )
	return 0;
    
    YQPatternSelector * patternSelector = 0;
    
    try
    {
	patternSelector = new YQPatternSelector( parent, modeFlags );
    }
    catch (const std::exception & e)
    {
	yuiError() << "Caught std::exception: " << e.what() << endl;
	yuiError() << "This is a libzypp problem. Do not file a bug against the UI!" << endl;
    }
    catch (...)
    {
	yuiError() << "Caught unspecified exception." << endl;
	yuiError() << "This is a libzypp problem. Do not file a bug against the UI!" << endl;
    }

    YUI_CHECK_NEW( patternSelector );
    
    return patternSelector;
}


YQSimplePatchSelector *
YQPackageSelectorPlugin::createSimplePatchSelector( YWidget * parent, long modeFlags )
{
    if ( error() )
	return 0;
    
    YQSimplePatchSelector * simplePatchSelector = 0;
    
    try
    {
	simplePatchSelector = new YQSimplePatchSelector( parent, modeFlags );
    }
    catch (const std::exception & e)
    {
	yuiError() << "Caught std::exception: " << e.what() << endl;
	yuiError() << "This is a libzypp problem. Do not file a bug against the UI!" << endl;
    }
    catch (...)
    {
	yuiError() << "Caught unspecified exception." << endl;
	yuiError() << "This is a libzypp problem. Do not file a bug against the UI!" << endl;
    }

    YUI_CHECK_NEW( simplePatchSelector );
    
    return simplePatchSelector;
}

