#include <OsmAndCore/Logging.h>
#include <OsmAndCore/CoreResourcesEmbeddedBundle.h>
#include <OsmAndCore.h>
#include <QDebug>
#include <QApplication>

using namespace OsmAnd;
using namespace std;

extern "C" void __CoreResourcesEmbeddedBundle__FakeReferences();

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  __CoreResourcesEmbeddedBundle__FakeReferences();
  shared_ptr<const CoreResourcesEmbeddedBundle> bundle =
    CoreResourcesEmbeddedBundle::loadFromCurrentExecutable();
  InitializeCore(bundle);

  OsmAnd::LogPrintf(OsmAnd::LogSeverityLevel::Info, "hello world from osmand");
  qDebug() << "hello world from qt";
  return 0;
}
