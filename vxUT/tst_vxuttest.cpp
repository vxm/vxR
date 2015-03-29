#include <QString>
#include <QtTest>

class VxUTTest : public QObject
{
		Q_OBJECT
		
	public:
		VxUTTest();
		
	private Q_SLOTS:
		void initTestCase();
		void cleanupTestCase();
		void testCase1();
		void testCase1_data();
};

VxUTTest::VxUTTest()
{
}

void VxUTTest::initTestCase()
{
}

void VxUTTest::cleanupTestCase()
{
}

void VxUTTest::testCase1()
{
	QFETCH(QString, data);
	QVERIFY2(true, "Failure");
}

void VxUTTest::testCase1_data()
{
	QTest::addColumn<QString>("data");
	QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(VxUTTest)

#include "tst_vxuttest.moc"
