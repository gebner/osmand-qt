#include <OsmAndCore/Logging.h>
#include <OsmAndCore/CoreResourcesEmbeddedBundle.h>
#include <OsmAndCore.h>
#include <QDebug>

using namespace OsmAnd;
using namespace std;

int main(int argc, char *argv[]) {
  shared_ptr<const CoreResourcesEmbeddedBundle> bundle =
    CoreResourcesEmbeddedBundle::loadFromLibrary("OsmAnd_shared");
  InitializeCore(bundle);

  OsmAnd::LogPrintf(OsmAnd::LogSeverityLevel::Info, "hello world from osmand");
  qDebug() << "hello world from qt";
  return 0;
}
