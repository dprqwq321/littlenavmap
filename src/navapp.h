/*****************************************************************************
* Copyright 2015-2020 Alexander Barthel alex@littlenavmap.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef NAVAPPLICATION_H
#define NAVAPPLICATION_H

#include "gui/application.h"

#include "common/mapflags.h"
#include "fs/fspaths.h"

class AircraftPerfController;
class AircraftTrack;
class AirportQuery;
class AirwayTrackQuery;
class WaypointTrackQuery;
class TrackController;
class AirspaceController;
class ConnectClient;
class DatabaseManager;
class ElevationProvider;
class InfoController;
class InfoQuery;
class LogdataController;
class LogdataSearch;
class MainWindow;
class MapPaintWidget;
class MapQuery;
class MapWidget;
class OnlinedataController;
class OptionsDialog;
class ProcedureQuery;
class QMainWindow;
class QSplashScreen;
class Route;
class RouteAltitude;
class RouteController;
class SearchController;
class StyleHandler;
class UpdateHandler;
class UserdataController;
class UserdataIcons;
class UserdataSearch;
class VehicleIcons;
class WeatherReporter;
class WebController;
class WindReporter;
class MapMarkHandler;
class MapAirportHandler;
class MapDetailHandler;
class TrackManager;
class MapThemeHandler;
class QAction;

namespace atools {
namespace util {
class Properties;
}
namespace gui {
class TabWidgetHandler;
}

namespace geo {
class Pos;
class Rect;
}

namespace fs {

namespace scenery {
class LanguageJson;
}

namespace perf {
class AircraftPerf;
}
namespace weather {
class Metar;
}
namespace sc {
class SimConnectUserAircraft;
class SimConnectAircraft;
class SimConnectData;
}

namespace userdata {
class UserdataManager;
class LogdataManager;
}

namespace common {
class MagDecReader;
class MoraReader;
}

namespace db {
class DatabaseMeta;
}
}

namespace sql {
class SqlDatabase;
}
}

namespace Ui {
class MainWindow;
}

/*
 * Facade that keeps most important handler, window and query classes for static access.
 * Initialized and deinitialized in main window.
 *
 * Not all getters refer to aggregated values but are rather delegates that help to minimize dependencies.
 *
 * See the delegated methods for documentation.
 */
class NavApp :
  public atools::gui::Application
{
  Q_DECLARE_TR_FUNCTIONS(NavApp)

public:
  NavApp(int& argc, char **argv, int flags = ApplicationFlags);
  virtual ~NavApp() override;

  NavApp(const NavApp& other) = delete;
  NavApp& operator=(const NavApp& other) = delete;

  static NavApp *navAppInstance();

  /* Creates all aggregated objects */
  static void init(MainWindow *mainWindowParam);
  static void initQueries();

  /* Needs map widget first */
  static void initElevationProvider();

  /* Deletes all aggregated objects */
  static void deInit();

  static void checkForUpdates(int channelOpts, bool manuallyTriggered, bool forceDebug);

  static void optionsChanged();
  static void preDatabaseLoad();
  static void postDatabaseLoad();

  static Ui::MainWindow *getMainUi();

  /* true if startup is completed and main window is visible */
  static bool isMainWindowVisible();
  static void setMainWindowVisible();

  static bool isFetchAiAircraft();
  static bool isFetchAiShip();
  static bool isConnected();
  static bool isConnectedActive();
  static bool isNetworkConnect();
  static bool isXpConnect();
  static bool isSimConnect();
  static bool isConnectedAndAircraft();
  static bool isConnectedAndAircraftFlying();
  static bool isUserAircraftValid();

  /* Check for availability in database */
  static bool isMoraAvailable();
  static bool isHoldingsAvailable();
  static bool isAirportMsaAvailable();
  static bool isGlsAvailable();

  static float getTakeoffFlownDistanceNm();
  static QDateTime getTakeoffDateTime();

  static const atools::fs::sc::SimConnectUserAircraft& getUserAircraft();
  static const atools::fs::sc::SimConnectData& getSimConnectData();
  static const atools::geo::Pos& getUserAircraftPos();

  static void updateAllMaps();

  static const QVector<atools::fs::sc::SimConnectAircraft>& getAiAircraft();

  static map::MapTypes getShownMapFeatures();
  static map::MapAirspaceFilter getShownMapAirspaces();

  static AirportQuery *getAirportQuerySim();
  static AirportQuery *getAirportQueryNav();

  /* Get the map query for the current GUI MapWidget. This means that its cache contents are related to the GUI map.
   * For other instances of MapPaintWidget use the MapPaintWidget::getMapQuery() */
  static MapQuery *getMapQueryGui();

  /* Get the track query for the current GUI MapWidget. This means that its cache contents are related to the GUI map. */
  static AirwayTrackQuery *getAirwayTrackQueryGui();

  /* Get the track query for the current GUI MapWidget. This means that its cache contents are related to the GUI map. */
  static WaypointTrackQuery *getWaypointTrackQueryGui();

  static atools::geo::Pos getAirportPos(const QString& ident);

  static InfoQuery *getInfoQuery();
  static ProcedureQuery *getProcedureQuery();
  static const Route& getRouteConst();
  static Route& getRoute();
  static void updateRouteCycleMetadata();

  /* Get a generic route string */
  static QString getRouteStringDefaultOpts();
  static const atools::geo::Rect& getRouteRect();

  static const RouteAltitude& getAltitudeLegs();
  static float getGroundBufferForLegFt(int legIndex);

  static float getRouteCruiseSpeedKts();
  static float getRouteCruiseAltFt();
  static float getRouteCruiseAltFtWidget();

  static bool isRouteEmpty();

  /* Currently selected simulator database */
  static atools::fs::FsPaths::SimulatorType getCurrentSimulatorDb();
  static bool isAirportDatabaseXPlane(bool navdata);
  static QString getCurrentSimulatorBasePath();
  static QString getSimulatorBasePath(atools::fs::FsPaths::SimulatorType type);
  static QString getSimulatorBasePathBest(const QVector<atools::fs::FsPaths::SimulatorType>& types);
  static QString getSimulatorFilesPathBest(const QVector<atools::fs::FsPaths::SimulatorType>& types);
  static bool hasSimulator(atools::fs::FsPaths::SimulatorType type);
  static bool hasAnyMsSimulator();

  static bool hasAnyXplaneSimulator();
  static bool hasXplane11Simulator();
  static bool hasXplane12Simulator();

  /* Selected navdatabase in menu */
  static bool isNavdataAll();
  static bool isNavdataOff();
  static bool isNavdataMixed();

  static OptionsDialog *getOptionsDialog();

  /* Get full path to language dependent "Flight Simulator X Files" or "Flight Simulator X-Dateien",
   * etc. Returns the documents path if FS files cannot be found. */
  static QString getCurrentSimulatorFilesPath();

  /* Get the short name (FSX, FSXSE, P3DV3, P3DV2) of the currently selected simulator. */
  static QString getCurrentSimulatorShortName();
  static QString getCurrentSimulatorName();
  static bool hasSidStarInDatabase();
  static bool hasRouteTypeInDatabase();
  static bool hasDataInDatabase();

  static void logDatabaseMeta();

  /* Simulator scenery data */
  static atools::sql::SqlDatabase *getDatabaseSim();

  /* External update from navaids or same as above */
  static atools::sql::SqlDatabase *getDatabaseNav();

  static atools::fs::userdata::UserdataManager *getUserdataManager();
  static UserdataIcons *getUserdataIcons();
  static UserdataSearch *getUserdataSearch();

  static atools::fs::userdata::LogdataManager *getLogdataManager();
  static LogdataSearch *getLogdataSearch();

  static atools::sql::SqlDatabase *getDatabaseTrack();

  static atools::sql::SqlDatabase *getDatabaseUser();
  static atools::sql::SqlDatabase *getDatabaseUserAirspace();
  static atools::sql::SqlDatabase *getDatabaseLogbook();
  static atools::sql::SqlDatabase *getDatabaseOnline();

  static ElevationProvider *getElevationProvider();

  static WeatherReporter *getWeatherReporter();
  static atools::fs::weather::Metar getAirportWeather(const QString& airportIcao, const atools::geo::Pos& airportPos);
  static map::MapWeatherSource getAirportWeatherSource();
  static WindReporter *getWindReporter();

  static void updateWindowTitle();
  static void updateErrorLabel();
  static void setStatusMessage(const QString& message, bool addToLog = false);

  /* Get main window in different variations to avoid including it */
  static QWidget *getQMainWidget();
  static QMainWindow *getQMainWindow();
  static MainWindow *getMainWindow();
  static void addDialogToDockHandler(QDialog *dialog);
  static void removeDialogFromDockHandler(QDialog *dialog);
  static QList<QAction *> getMainWindowActions();

  /* true if tooltips in menus are visible */
  static bool isMenuToolTipsVisible();

  static MapWidget *getMapWidgetGui();
  static MapPaintWidget *getMapPaintWidgetGui();
  static RouteController *getRouteController();
  static atools::gui::TabWidgetHandler *getRouteTabHandler();
  static const InfoController *getInfoController();
  static QFont getTextBrowserInfoFont();
  static MapThemeHandler *getMapThemeHandler();

  static DatabaseManager *getDatabaseManager();

  /* MSFS translations from table "translation" */
  static const atools::fs::scenery::LanguageJson& getLanguageIndex();

  static ConnectClient *getConnectClient();

  /* Can be null while compiling database */
  static const atools::fs::db::DatabaseMeta *getDatabaseMetaSim();
  static const atools::fs::db::DatabaseMeta *getDatabaseMetaNav();

  static QString getDatabaseAiracCycleSim();
  static QString getDatabaseAiracCycleNav();

  /* True if online data and ATC centers are available */
  static bool hasOnlineData();

  static QString getOnlineNetworkTranslated();
  static bool isOnlineNetworkActive();

  static bool isAircraftTrackEmpty();
  static const AircraftTrack& getAircraftTrack();

  static const AircraftTrack& getAircraftTrackLogbook();
  static void deleteAircraftTrackLogbook();

  static void initSplashScreen();

  /* Called by main application once startup is done */
  static void finishSplashScreen();

  /* Remove splash when showing error messages, etc. to avoid overlay */
  static void closeSplashScreen();

  /* Main window close event about to exit */
  static bool isCloseCalled();
  static void setCloseCalled(bool value = true);

  /* Main window destructor called - appears later than isCloseCalled() */
  static bool isShuttingDown();
  static void setShuttingDown(bool value = true);

  /* true if map window is maximized */
  static bool isFullScreen();

  static float getMagVar(const atools::geo::Pos& pos, float defaultValue = 0.f);

  static UpdateHandler *getUpdateHandler();

  static UserdataController *getUserdataController();
  static LogdataController *getLogdataController();
  static OnlinedataController *getOnlinedataController();
  static AircraftPerfController *getAircraftPerfController();
  static SearchController *getSearchController();
  static const atools::fs::perf::AircraftPerf& getAircraftPerformance();

  static TrackController *getTrackController();
  static bool hasTracks();
  static TrackManager *getTrackManager();

  static AirspaceController *getAirspaceController();
  static bool hasAnyAirspaces();

  static atools::fs::common::MagDecReader *getMagDecReader();

  static atools::fs::common::MoraReader *getMoraReader();

  static VehicleIcons *getVehicleIcons();

  /* Not entirely reliable since other modules might be initialized later */
  static bool isLoadingDatabase();

  static QString getCurrentGuiStyleDisplayName();
  static bool isCurrentGuiStyleNight();

  static StyleHandler *getStyleHandler();

  static map::MapWeatherSource getMapWeatherSource();
  static bool isMapWeatherShown();

  static const QString& getCurrentRouteFilepath();
  static const QString& getCurrentAircraftPerfFilepath();
  static const QString& getCurrentAircraftPerfName();
  static const QString& getCurrentAircraftPerfAircraftType();

  static WebController *getWebController();
  static MapPaintWidget *getMapPaintWidgetWeb();

  static MapMarkHandler *getMapMarkHandler();
  static MapAirportHandler *getMapAirportHandler();
  static MapDetailHandler *getMapDetailHandler();

  static void showFlightPlan();
  static void showAircraftPerformance();
  static void showLogbookSearch();
  static void showUserpointSearch();

  /* Command line options */
  static QString getStartupOption(const QString& key);
  static void addStartupOption(const QString& key, const QString& value);

private:
  static void initApplication();
  static void readMagDecFromDatabase();

  /* Database query helpers and caches */
  static AirportQuery *airportQuerySim, *airportQueryNav;
  static InfoQuery *infoQuery;
  static ProcedureQuery *procedureQuery;
  static ElevationProvider *elevationProvider;

  /* Most important handlers */
  static ConnectClient *connectClient;
  static DatabaseManager *databaseManager;
  static atools::fs::common::MagDecReader *magDecReader;

  /* minimum off route altitude from nav database */
  static atools::fs::common::MoraReader *moraReader;
  static UserdataController *userdataController;
  static MapMarkHandler *mapMarkHandler;
  static MapAirportHandler *mapAirportHandler;
  static MapDetailHandler *mapDetailHandler;
  static LogdataController *logdataController;
  static OnlinedataController *onlinedataController;
  static TrackController *trackController;
  static AircraftPerfController *aircraftPerfController;
  static AirspaceController *airspaceController;

  /* Main window is not aggregated */
  static MainWindow *mainWindow;

  static atools::fs::db::DatabaseMeta *databaseMetaSim;
  static atools::fs::db::DatabaseMeta *databaseMetaNav;
  static QSplashScreen *splashScreen;

  static UpdateHandler *updateHandler;
  static VehicleIcons *vehicleIcons;
  static StyleHandler *styleHandler;

  static WebController *webController;
  static atools::util::Properties *startupOptions;

  static bool loadingDatabase;
  static bool shuttingDown;
  static bool closeCalled;
  static bool mainWindowVisible;
};

#endif // NAVAPPLICATION_H
