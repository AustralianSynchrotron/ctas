

#include "../common/ctas.h"
#include "../common/poptmx.h"


using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path in_name;               ///< Name of the input file.
  Path ms_name;               ///< Name of the input file.
  Path out_name;              ///< Name of the output file.
  float angle;
  Crop<2> crp2;                  ///< Crop input projection image
  Crop<3> crp3;                  ///< Crop input projection image
  //Binn bnn;                  ///< binning factor
  bool saveInt;
  bool beverbose;       ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : out_name("proc-<input>")
//  , rad(0)
//  , coeff(1.0)
//  , arf(1)
  , angle(0)
  , saveInt(false)
  , beverbose(false)
{

  poptmx::OptionTable table("Test process.", "Just to test something.");

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &in_name, "input", "Input image.", "The image to work on")
    .add(poptmx::ARGUMENT, &ms_name, "mask", "Mask image.", "")
    //.add(poptmx::ARGUMENT, &out_name, "output", "Output image.", "The image to store results", out_name)

    .add(poptmx::NOTE, "OPTIONS:")
    //.add(poptmx::OPTION, &rad, 'r', "radius", "Some radius.", "Long description of radius.")
    //.add(poptmx::OPTION, &coeff, 'c', "coefficient", "Some number", "")
    //.add(poptmx::OPTION, &arf, 'F', "anarg", "description", "Long description")
    .add(poptmx::OPTION, &angle, 'a', "angle", "Some number", "")
    .add(poptmx::OPTION, &crp2, 0, "crop2", "image crop", "")
    .add(poptmx::OPTION, &crp3, 0, "crop3", "volume crop", CropOptionDesc)
    //.add(poptmx::OPTION, &bnn, 0, "binn", "image binn", "")
    .add(poptmx::OPTION, &saveInt, 'i', "int", "Output image(s) as integer.", IntOptionDesc)
    .add_standard_options(&beverbose)
    .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  command = table.name();

  // <input> : one required argument.
  //if ( ! table.count(&in_name) )
  //  exit_on_error(command, "Missing required argument: "+table.desc(&in_name)+".");
  // <output> : one more argument may or may not exist
  //if ( ! table.count(&out_name) )
  //  out_name = upgrade(in_name, "res-");
  angle *= M_PI / 180.0;

}

#include "../common/projection.h"


/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;




  Map arr;
  ReadImage(args.in_name, arr);
  Map mrr;
  ReadImage(args.ms_name, mrr);
  /*
  Trans trans(arr.shape(), args.angle, Crop(), PointF<2>(), mrr);
  prdn(trans.ish);
  prdn(Shape(trans.afterRotMask.shape()));
  SaveImage("rotmask.tif", trans.afterRotMask, args.saveInt);
  SaveImage("mask.tif", trans.mask, args.saveInt);
  Map oarr;
  trans.process(arr, oarr);
  */
  exit(0);

}
