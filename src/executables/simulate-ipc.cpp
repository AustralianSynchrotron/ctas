#include "../common/common.h"
#include "../common/poptmx.h"
#include "../common/ipc.h"

#define _USE_MATH_DEFINES // for M_PI

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path outIm;                   ///< Output image.
  float dd;                     ///< Pixel size.
  float lambda;                 ///< Wavelength.
  float theta;                  ///< Rotation angle.
  float dist;                   ///< Object-to-detector distance.
  int x;                        ///< X-size of the image.
  int y;                        ///< Y-size of the image.
  float d2b;                     ///< The ration (\f$\beta/\delta\f$).
  bool beverbose;				///< Be verbose flag
  bool SaveInt;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  SaveInt(false),
  dd(1.0),
  lambda(1.0),
  theta(0.0),
  d2b(0.0)
{


  poptmx::OptionTable table
	("test", "TEST");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &outIm, "out", "output", "output image")
    .add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &lambda, 'w', "wavelength", "Wavelength (A)", "", toString(lambda))
	.add(poptmx::OPTION, &dist, 'z', "distance", "Object-to-detector distance (mm)", "")
	.add(poptmx::OPTION, &dd, 'r', "resolution", "Pixel size (micron)", "", toString(dd))
	.add(poptmx::OPTION, &theta, 't', "theta", "Rotation angle (radian)", "", toString(theta))
	.add(poptmx::OPTION, &x, 'x', "xsize", "X size (pixels)", "", "ysize")
  .add(poptmx::OPTION, &y, 'y', "ysize", "Y size (pixels)", "", "xsize")
	.add(poptmx::OPTION, &d2b, 'd', "d2b", "delta/beta ratio (0 for no absorption).", "", toString(d2b))
	.add(poptmx::OPTION, &SaveInt,'i', "int", "Output image(s) as integer.", IntOptionDesc)
	.add_standard_options(&beverbose);

  if ( ! table.parse(argc,argv) )
	exit(0);
  if ( ! table.count() ) {
	table.usage();
	exit(0);
  }
  command = table.name();


  if ( ! table.count(&outIm) )
  	exit_on_error(command, "Missing required argument: "+table.desc(&outIm)+".");

  if ( ! table.count(&dist) )
  	exit_on_error(command, "Missing required option: "+table.desc(&dist)+".");
  if (dist < 0.0)
  	exit_on_error(command, "Negative distance (given by "+table.desc(&dist)+").");
  dist /= 1.0E3;

  if (lambda <= 0.0)
  	exit_on_error(command, "Negative wavelength (given by "+table.desc(&lambda)+").");
  lambda /= 1.0E10;

  if (dd <= 0.0)
  	exit_on_error(command, "Negative pixel size (given by "+table.desc(&dd)+").");
  dd /= 1.0E6;

  if (d2b < 0.0)
  	exit_on_error(command, "Negative ratio beta/delta (given by "+table.desc(&d2b)+").");

  if (table.count(&x) && !table.count(&y))
    y=x;
  if (table.count(&y) && !table.count(&x))
    x=y;
  if ( ! (table.count(&x) + table.count(&y)) )
	  exit_on_error(command, "Missing required argument: "+table.desc(&x)+" or "+table.desc(&y)+".");



}




/// \MAIN{ct}
int main(int argc, char *argv[]) {
  const clargs args(argc, argv) ;
  Shape sh(args.y, args.x);
  Map intensity(sh);
  CMap tif(sh);
  simulateTif( tif, sh, args.d2b, args.theta, args.dd, args.lambda);
  propagate(tif, intensity, args.dd, args.lambda, args.dist);
  SaveImage(args.outIm, intensity, args.SaveInt);
  exit(0);
}



