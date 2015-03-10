
#include <maya/MImage.h>
#include <definition.h>
#include <maya/MSimple.h>
#include <maya/MString.h>
#include <maya/MGlobal.h>
#include <stdio.h>

DeclareSimpleCommand( loockatCube, "", "7.0");

MStatus loockatCube::doIt( const MArgList& args )
{
	MStatus stat = MS::kSuccess;

	MImage salida;
	
	salida.create(800, 600);

	vxCamera cam;

	cam.set(vxVector3d (0,0,0), vxVector3d (0,0,1), 2,  1.2 , 1);

	vxImage buf(800, 600, 4);

	buf.setPixels(salida.pixels());
	
	unsigned char *mapa=salida.pixels();

	int visto=0;

	for (buf.reset(); !buf.isDone(); buf.next())
	{

		vxBox caj(args.asDouble(0),args.asDouble(1),args.asDouble(2),1);
		vxBox caj2(0.2,.7,6,.5);

		if ( caj.inSight( cam.givemeNextRay( buf, args.asDouble(4) ) ) || caj2.inSight( cam.givemeNextRay( buf, args.asDouble(4) ) ) )
		{
			visto++;
			vxColor color = vxShader::getColor(vxVector3d(0,0,0) , vxVector3d(0,1,0));
			buf.setColorAtPixel(color);
		}
	/*
		MString a;
		a.set( cam.givemeRay( buf.getScanXd(), buf.getScanYd() ).getX() );
		MString b;
		b.set( cam.givemeRay( buf.getScanXd(), buf.getScanYd() ).getY() );
		MString c;
		c.set( cam.givemeRay( buf.getScanXd(), buf.getScanYd() ).getZ() );

		MGlobal::displayInfo( a + "    " + b + "    " + c );
	*/
	}
	
	MString msj;
	msj.set(visto);
	MGlobal::displayInfo("visto " + msj  );
	salida.writeToFile(args.asString(3),"tif");


/*	vxBox caj(args.asDouble(0),args.asDouble(1),args.asDouble(2),args.asDouble(3));

	if ( caj.inSight(  vxVector3d(args.asDouble(4) , args.asDouble(5), args.asDouble(6) )) )
		{
			//MGlobal::displayInfo( "Es visto" );
			setResult( true);
		}
	else
		{
			//MGlobal::displayInfo( " --- NO --- " );
			setResult( false);
		}

	setResult( caj.inSight(  vxVector3d(args.asDouble(4) , args.asDouble(5), args.asDouble(6) ) ) );
*/
	return stat;
}
